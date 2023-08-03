#include "Enemy.h"
#include "ImguiManager.h"
#include "Player.h"
void Enemy::Initialize(DirectXCommon* dxCommon, Player* player)
{

	dxCommon_ = dxCommon;

	player_ = player;

	//デバイスのセット
	FbxObject3d::SetDevice(dxCommon_->GetDevice());
	//グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	////敵のFbx読み込み
	//enemyFbxM_.reset(FbxLoader::GetInstance()->LoadModelFromFile("boss_prot4"));
	//enemyFbxO_ = std::make_unique<FbxObject3d>();
	//enemyFbxO_->Initialize();
	//enemyFbxO_->SetModel(enemyFbxM_.get());
	//enemyFbxO_->PlayAnimetion(5);

	//enemyFbxO_->SetPosition(enemy_.translation_);

	enemyO_ = Object3d::Create();

	enemyM_ = Model::CreateFromOBJ("cube");

	enemyO_->SetModel(enemyM_);

	wtf.translation_ = { 0,0,0 };
	//enemyO_->SetScale({2,2,2});
	enemyO_->SetPosition(wtf.translation_);

	//弾のモデルをセット
	bulletM_ = Model::CreateFromOBJ("cube");

	sphere.resize(SPHERE_COLISSION_NUM);
	spherePos.resize(SPHERE_COLISSION_NUM);

	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		spherePos[i] = enemyO_->GetPosition();
		sphere[i]->SetBasisPos(&spherePos[i]);
		sphere[i]->SetRadius(1.0f);
		sphere[i]->SetAttribute(COLLISION_ATTR_ENEMYS);
		sphere[i]->Update();
		////test
		//coliderPosTest_[i] = Object3d::Create();
		//coliderPosTest_[i]->SetModel(hpModel_.get());
		//coliderPosTest_[i]->SetPosition(sphere[i]->center);
		//coliderPosTest_[i]->SetScale({ sphere[i]->GetRadius(),sphere[i]->GetRadius() ,sphere[i]->GetRadius() });
		//coliderPosTest_[i]->SetRotate({ 0,0,0 });
		//coliderPosTest_[i]->Update();

	}

}

void Enemy::Update()
{

	wtf = enemyO_->GetWorldTransform();

	playerPos = player_->GetObject3d()->GetWorldTransform().translation_;
	enemyPos = enemyO_->worldTransform.translation_;
	distance = { playerPos.x - enemyPos.x,
				enemyPos.y,
				playerPos.z - enemyPos.z };

	angle = (atan2(distance.x, distance.z) + MyMath::PI / 2);
	enemyO_->worldTransform.rotation_.y = (angle + MyMath::PI / 2);


	if (Hp_ <= 0)
	{
		SetisDead(true);

	}
	else
	{
		if (player_->GetisDead() == false)
		{
			Attack();

			//Move();
		}

	}

	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->GetIsDead(); });

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}


	if (GetisDead() == true)
	{
		for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
		{
			bullet->SetisDead(true);
		}
	}

	GetIsHit();

	enemyO_->UpdateMatrix();

	
#pragma region オブジェクト同士の押し出し処理
	class EnemyQueryCallBack : public QueryCallback
	{
	public:
		EnemyQueryCallBack(Sphere* sphere) : sphere(sphere) {};

		bool OnQueryHit(const QueryHit& info)
		{
			rejectDir = info.reject;
			rejectDir.nomalize();

			//上方向と排斥方向の角度差のコサイン値
			float cos = rejectDir.dot(up);

			//地面判定しきい値角度
			const float threshold = cosf(XMConvertToRadians(30.0f));
			//角度差によって天井または地面と判定される場合を除いて
			if (-threshold < cos && cos < threshold)
			{
				//押し出す
				sphere->center += info.reject;
				move += info.reject;
			}
			return true;
		}
		void SphereQuery();

		//ワールドの上方向
		const Vector3 up = { 0,1,0 };
		//排斥方向
		Vector3 rejectDir;
		//クエリーに使用する球
		Sphere* sphere = nullptr;
		//排斥による移動量
		Vector3 move = {};

	};


	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		EnemyQueryCallBack callback(sphere[i]);

		//球と地形の交差を全探索する
		CollisionManager::GetInstance()->QuerySphere(*sphere[i], &callback);
		if (sphere[i]->GetIsHit() == true)
		{
			if (sphere[i]->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_PLAYERS)
			{
				enemyO_->worldTransform.translation_.x += callback.move.x;
				enemyO_->worldTransform.translation_.y += callback.move.y;
				enemyO_->worldTransform.translation_.z += callback.move.z;
			}
		}
		
		//sphere[i]->Update();
	}

	CheckHitCollision();

#pragma endregion 

	enemyO_->Update();

	/*ImGui::Begin("enemyRotate");

	ImGui::SetWindowPos({ 600 , 200 });
	ImGui::SetWindowSize({ 200,100 });
	ImGui::InputFloat3("x,y,z", &enemyO_->worldTransform.rotation_.x);

	ImGui::End();*/
}

void Enemy::Draw()
{
	if (GetisDead() == false)
	{
		for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
		{
			bullet->Draw();
		}
		if (GetisDead() == false)
		{
			enemyO_->Draw();
		}
	}
	


}

void Enemy::Action()
{

}

void Enemy::CheckHitCollision()
{
	oldPos = wtf.translation_;

	Vector3 distance;

	if (hitDeley > 0) {	//毎フレームヒットを防止
		enemyO_->SetColor({ 0,0,1,1 });
		hitDeley--;
	}
	else
	{
		
		enemyO_->SetColor({ 1,1,1,1 });
	}

	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		if (hitDeley <= 0 && sphere[i]->GetIsHit() == true)
		{
			if (sphere[i]->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_PLAYERBULLETS)
			{
				Hp_ -= 1;
				hitDeley = 5;
				SetIsHit(true);

				break;
			}
		}

	}
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		if (sphere[i]->GetIsHit() == true)
		{
			if (sphere[i]->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_PLAYERS)
			{
				wtf.translation_ = oldPos;
				enemyO_->SetPosition(wtf.translation_);
				break;
			}
		}
		
	}
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) 
	{
		if (GetisDead() == true)
		{
			CollisionManager::GetInstance()->RemoveCollider(sphere[i]);
			//こいつはいらない
			/*sphere[i]->GetCollisionInfo().collider->RemoveAttribute(COLLISION_ATTR_PLAYERBULLETS);*/
		}
		spherePos[i] = enemyO_->GetPosition();
		sphere[i]->Update();
	}
}

void Enemy::Attack()
{
	Vector3 playerPos;
	Vector3 enemyPos;
	Vector3 distance;
	float speed = 0.5f;

	playerPos = player_->GetObject3d()->GetWorldTransform().translation_;

	enemyPos = enemyO_->worldTransform.translation_;

	distance = playerPos - enemyPos;

	distance.nomalize();

	distance *= speed;

	/*Vector3 begieP1 = {0,10,-30};
	Vector3 begieP2 = { 0,-30,-10 };

	Vector3 a = a.lerp(playerPos,begieP1, timeRate);
	Vector3 b = b.lerp(begieP1, begieP2, timeRate);
	Vector3 c = c.lerp(begieP2, enemyPos, timeRate);

	Vector3 d = d.lerp(a, b, timeRate);
	Vector3 e = e.lerp(b, c, timeRate);

	distance = distance.lerp(d,e, timeRate);*/


#pragma endregion

#pragma region 
	srand(time(nullptr));


	//１秒に１回の間隔で抽選を行う
	ShotflameCount++;
	if (ShotflameCount > 60)
	{
		//射撃していないなら
		if (isShot == false)
		{
			bulletType = rand() % 2 + 1;
			
			isShot = true;
		}
		if (bulletType == EnemyBulletType::RAPIDSHOT)
		{
			ShotflameCount = 0.0f;
		}
		else if (bulletType == EnemyBulletType::ONESHOT)
		{
			ShotflameCount = 30.0f;
		}
	}

	if (bulletType == EnemyBulletType::RAPIDSHOT)
	{
		//弾の最大値を決定
		MAX_BULLET = rand() % 20 + 1;
		rapidShot = true;

	}
	else if(bulletType == EnemyBulletType::ONESHOT)
	{
		MAX_BULLET = 1;
		oneShot = true;
	}


	if (oneShot == true)
	{
		rapidShot = false;
		//弾の最大個数
		MAX_BULLET = 1;
		pushTimer = 15.0f;//押してる時間
		pressTimer = 0.0f;//連射用の時間
		oneShot = false;
	}
	else if (rapidShot == true)
	{
		oneShot = false;
		if (pressTimer < 0)
		{
			//チャージしている弾の個数がMAX以下だった場合
			if (rapidCount < MAX_BULLET )
			{
				rapidCount++;
			}
			pressTimer = 6.0f;
		}
	}
	if (isShot == true)
	{
		bulletTimer--;

		//弾が最大個数以下だった時
		if (bulletSize < MAX_BULLET)
		{
			if (bulletTimer <= 0)
			{
				if (isShot == true)
				{
					bulletSize++;
					// 弾を生成し初期化
					std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
					newBullet->Initialize(bulletM_, enemyO_->worldTransform.translation_, distance,enemyO_->worldTransform.rotation_);

					if (bulletType == EnemyBulletType::ONESHOT)
					{
						bulletTimer = 15.0f;
					}
					else if (bulletType == EnemyBulletType::RAPIDSHOT)
					{
						bulletTimer = 5.0f;
					}

					//
					newBullet->SetPlayer(player_);

					bullets_.push_back(std::move(newBullet));
				}

			}
		}
		if (bulletSize >= MAX_BULLET)
		{
			MAX_BULLET = 0;
			bulletSize = 0;
			rapidCount = 0;
			bulletType = EnemyBulletType::NONE;
			coolTimer = 60.0f;
			pressTimer = 0.0f;
			isShot = false;
		}
	}
	
}

void Enemy::Vanish()
{
}

void Enemy::Move()
{
	//１秒に一回実行する1
	MoveflameCount++;
	if (MoveflameCount < 30)
	{
		srand(time(nullptr));
		moveActionNum = rand() % 4+1;
		MoveflameCount = 0;
	}
	
	if (moveActionNum == 1)
	{
		isLeft = true;
		isRight = false;
		isApproach = false;
	}
	else if(moveActionNum == 2)
	{
		isLeft = false;
		isRight = true;
		isApproach = false;
	}
	else if(moveActionNum == 3)
	{
		isLeft = false;
		isRight = false;
		isApproach = true;
	}
	else
	{
		isLeft = false;
		isRight = false;
		isApproach = false;
	}

	if (isLeft == true)
	{
		wtf.translation_.x -= 0.5f;
		enemyO_->SetPosition(wtf.translation_);
	}
	if (isRight == true)
	{
		wtf.translation_.x += 0.5f;
		enemyO_->SetPosition(wtf.translation_);
	}
	if (isApproach == true)
	{
		enemyPos = enemyO_->GetPosition();
		distance = player_->GetPosition()-enemyO_->GetPosition()  ;
		distance.nomalize();
		distance *= 0.5f;
		wtf.translation_ += distance;
		enemyO_->SetPosition(wtf.translation_);
	}
}

void Enemy::Step()
{
}

void Enemy::Reset()
{
	playerPos = { 0,0,0 };
	enemyPos = { 0,0,0 };
	distance = { 0,0,0 };

	angle;
	isAlive_ = false;
	isAttack = false;
	isVanish = false;
	isMove = false;

	isLeft = false;
	isRight = false;
	isApproach = false;

	actionNum = 0;
	moveActionNum = 0;

	MoveflameCount = 0;

	isShot = false;
	oneShot = false;
	rapidShot = false;

	MAX_BULLET = 0;
	bulletSize = 0;
	bulletType = EnemyBulletType::NONE;

	bulletTimer = 0.0f;
	//連射制限のためのクールタイム
	coolTimer = 60.0f;
	//ボタンを押してる時間
	pushTimer = 15.0f;
	//長押ししている時間
	pressTimer = 0.0f;

	ShotflameCount = 0;


	rapidCount = 0;

	//長押しフラグ
	isCharge = false;

	isHit_ = false;

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
		{
			CollisionManager::GetInstance()->RemoveCollider(sphere[i]);
		}
		bullet->Reset();
	}

	
}
