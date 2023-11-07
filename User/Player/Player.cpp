#include "Player.h"
#include "Enemy.h"
#include "ImguiManager.h"
#include "Affin.h"
#include "Ease.h"
void Player::Initialize(DirectXCommon* dxCommon, Enemy* enemy)
{
	dxCommon_ = dxCommon;

	
	//デバイスのセット
	FbxObject3d::SetDevice(dxCommon_->GetDevice());
	//グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	////敵のFbx読み込み
	//playerFbxM_.reset(FbxLoader::GetInstance()->LoadModelFromFile("boss_prot4"));
	//playerFbxO_ = std::make_unique<FbxObject3d>();
	//playerFbxO_->Initialize();
	//playerFbxO_->SetModel(playerFbxM_.get());

	playerO_ = Object3d::Create();

	playerM_ = Model::CreateFromOBJ("human");

	playerO_->SetModel(playerM_);

	wtf.translation_ = { 0,0,-50 };
	//playerO_->SetScale({ 2,2,2 });
	playerO_->SetPosition(wtf.translation_);
	//playerO_->SetColor({1,0,0,1});

	bulletM_ = Model::CreateFromOBJ("cube");
	//playerFbxO_->SetPosition(player_.translation_);

	enemy_ = enemy;

	//パーティクル
	particle_ = std::make_unique<ParticleManager>();
	particle_->Initialize();
	particle_->LoadTexture("sprite/particle.png");
	particle_->Update();

	////FBX当たり判定初期化
	//for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	//{
	//	sphere[i] = new SphereCollider;
	//	CollisionManager::GetInstance()->AddCollider(sphere[i]);
	//	spherePos[i] = playerFbxO_.get()->bonesMat[i].GetWorldPos();
	//	sphere[i]->SetBasisPos(&spherePos[i]);
	//	sphere[i]->SetRadius(1.0f);
	//	sphere[i]->SetAttribute(COLLISION_ATTR_ALLIES);
	//	sphere[i]->Update();
	//	//test
	//	coliderPosTest_[i] = Object3d::Create();
	//	coliderPosTest_[i]->SetModel(hpModel_.get());
	//	coliderPosTest_[i]->SetPosition(sphere[i]->center);
	//	coliderPosTest_[i]->SetScale({ sphere[i]->GetRadius(),sphere[i]->GetRadius() ,sphere[i]->GetRadius() });
	//	coliderPosTest_[i]->SetRotate({ 0,0,0 });
	//	coliderPosTest_[i]->Update();

	//}
	sphere.resize(SPHERE_COLISSION_NUM);
	spherePos.resize(SPHERE_COLISSION_NUM);

	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		spherePos[i] = playerO_->GetPosition();
		sphere[i]->SetBasisPos(&spherePos[i]);
		sphere[i]->SetRadius(1.0f);
		sphere[i]->SetAttribute(COLLISION_ATTR_PLAYERS);
		sphere[i]->Update();
		////test
		//coliderPosTest_[i] = Object3d::Create();
		//coliderPosTest_[i]->SetModel(hpModel_.get());
		//coliderPosTest_[i]->SetPosition(sphere[i]->center);
		//coliderPosTest_[i]->SetScale({ sphere[i]->GetRadius(),sphere[i]->GetRadius() ,sphere[i]->GetRadius() });
		//coliderPosTest_[i]->SetRotate({ 0,0,0 });
		//coliderPosTest_[i]->Update();

	}
	startCount = clock() / 1000;

	blowAwayPos = playerO_->GetPosition();
	//playerO_->SetRotation({180, 0, 0});
}

void Player::Update(Input* input, GamePad* gamePad)
{
	//カメラの角度の更新
	moveAngle();

	if (Hp_ <= 0)
	{
		SetisDead(true);

		
		GameOverAnime();
	}
	else
	{
		transNormal = {0, 0.5f, -5};

		transNormal = MyMath::bVelocity(transNormal, playerO_->worldTransform.matWorld_);
	}
	
	oldPos = wtf.translation_;


	//デスフラグが立った球を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->GetIsDead(); });

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	if (GetisDead() == true)
	{
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
		{
			bullet->SetisDead(true);
		}
	}

	if (bulletType != PlayerBulletType::RapidShot)
	{
		Move(input, gamePad);
	}

	Shot(input, gamePad);
	Vanish(input, gamePad);
	////行列の更新など
	//playerO_->UpdateMatrix();

	particle_->Update();


	CheckHitCollision();
	
	//ImGui::Begin("BulletSize");
	//ImGui::SetWindowPos({ 800 , 100 });
	//ImGui::SetWindowSize({ 500,300 });
	//ImGui::InputInt("plyaerHp ", &Hp_);
	//ImGui::InputInt("enemyHp", &enemy_->Hp_);
	///*ImGui::SetWindowPos({ 800 , 100 });
	//ImGui::SetWindowSize({ 500,300 });
	//ImGui::InputInt("BulletSize", &bulletSize);
	//ImGui::InputFloat("pushTimer",&pushTimer);
	//ImGui::InputFloat("pressTimer", &pressTimer);
	//ImGui::InputInt("MAX", &MAX_BULLET);*/

	//ImGui::End();
	
	/*ImGui::Begin("Vanish");

	ImGui::SliderFloat("Gauge",&VanishGauge, -400.0f, 400.0f);
	ImGui::InputFloat("Timer", &vanishTimer);
	ImGui::End();*/


	/*ImGui::Begin("Camera");

	ImGui::SetWindowPos({ 200 , 200 });
	ImGui::SetWindowSize({ 500,100 });

	ImGui::SliderFloat("eye:x", &playerO_->worldTransform.translation_.x, -400.0f, 400.0f);
	ImGui::SliderFloat("eye:z", &playerO_->worldTransform.translation_.z, -400.0f, 400.0f);

	ImGui::End();*/

	playerO_->Update();
	/*playerFbxO_->Update();*/
}

void Player::Draw()
{
	if (GetisDead() == false)
	{
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
		{
			bullet->Draw();
		}
		

		/*playerFbxO_->Draw(dxCommon_->GetCommandList());*/
	}
	if ( blowAwayCount <90)
	{
		if (scale_.x > 0.2)
		{
			playerO_->Draw();
		}
	}
}

void Player::Move(Input* input, GamePad* gamePad)
{

	velocity_ = { 0 , 0 , 0 };

	if (gamePad->StickInput(L_UP) || input->PushKey(DIK_W))
	{
		//enemyPos = enemy_->GetPosition();
		//distance = enemy_->GetPosition() - playerO_->GetPosition();
		//distance.nomalize();
		//distance *= 0.5f;
		//wtf.translation_ += distance;
		//wtf.translation_.z += 0.5f;
		//playerO_->SetPosition(wtf.translation_);

		velocity_ += { 0 , 0 , moveSpeed };
	}

	if (gamePad->StickInput(L_DOWN)|| input->PushKey(DIK_S))
	{
		//wtf.translation_.z -= 0.5f;
		//playerO_->SetPosition(wtf.translation_);
		velocity_ += { 0 , 0 , moveSpeed * -1 };

	}
	if (gamePad->StickInput(L_LEFT) || input->PushKey(DIK_A))
	{
		//wtf.translation_.x -= 0.5f;
		//playerO_->SetPosition(wtf.translation_);
		velocity_ += { moveSpeed * -1 , 0 , 0 };
	}

	if (gamePad->StickInput(L_RIGHT) || input->PushKey(DIK_D))
	{
		//wtf.translation_.x += 0.5f;
		//playerO_->SetPosition(wtf.translation_);

		velocity_ += { moveSpeed , 0 , 0 };
	}
	if (GetisDead()==false)
	{
		playerO_->worldTransform.rotation_ = cameraAngle;
	}
	

	playerO_->worldTransform.UpdateMatWorld();

	velocity_ = MyMath::MatVector(velocity_, playerO_->worldTransform.matWorld_);

	playerO_->worldTransform.translation_ += velocity_;


	//ImGui::Begin("playerPos");

	//ImGui::SetWindowPos({ 200 , 200 });
	//ImGui::SetWindowSize({ 200,100 });
	//ImGui::InputFloat3("x", &playerO_->worldTransform.translation_.x);
	//ImGui::InputFloat3("x", &playerO_->worldTransform.rotation_.x);

	//ImGui::End();

}

void Player::Shot(Input* input, GamePad* gamePad)
{
	nowCount++;

	elapsedCount = nowCount - startCount;
	float elapsedTime = static_cast<float> (elapsedCount) / 1000000.0f;

	timeRate = min(elapsedTime / maxTime, 1.0f);
#pragma region 弾の移動処理
	float speed = 0.5f;
	if (bulletType == PlayerBulletType::OneShot)
	{
		speed = 1.0f;
	}
	else if (bulletType == PlayerBulletType::RapidShot)
	{
		speed = 2.0f;
	}
	

	Vector3 playerPos;
	Vector3 enemyPos;
	Vector3 distance;


	playerPos = playerO_->worldTransform.translation_;

	enemyPos = enemy_->GetObject3d()->GetWorldTransform().translation_;

	distance = enemyPos - playerPos;

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

	//押したとき
	if (gamePad->ButtonInput(A) || input->PushKey(DIK_SPACE))
	{
		pushTimer--;
		pressTimer--;
		
		//連射の時
		if (pushTimer < 0)
		{
			rapidShot = true;
		}
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
			if (MAX_BULLET < 20)
			{
				MAX_BULLET++;
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
					std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
					newBullet->Initialize(bulletM_, playerO_->worldTransform.translation_, distance);

					if (bulletType == PlayerBulletType::OneShot)
					{
						bulletTimer = 15.0f;
					}
					else if (bulletType == PlayerBulletType::RapidShot)
					{
						bulletTimer = 5.0f;
					}
					
					//
					newBullet->SetEnemy(enemy_);

					bullets_.push_back(std::move(newBullet));
				}

			}
		}
		if (bulletSize >= MAX_BULLET)
		{
			MAX_BULLET = 0;
			bulletSize = 0;
			bulletType = PlayerBulletType::None;
			coolTimer = 60.0f;
			pushTimer = 15.0f;
			pressTimer = 0.0f;
			isShot = false;
		}
	}
	//離したとき
	if (gamePad->ButtonOffTrigger(A) || input->ReleaseKey(DIK_SPACE))
	{
		//単発の時
		if (pushTimer >= 0)
		{
			bulletType = PlayerBulletType::OneShot;
			oneShot = true;
		}
		else
		{
			bulletType = PlayerBulletType::RapidShot;
		}
		if (isShot == false)
		{
			isShot = true;
		}
	}

}

void Player::Vanish(Input* input, GamePad* gamePad)
{
	playerPos_ = playerO_->GetPosition();
	//敵が攻撃をしてきたとき
	if (enemy_->GetisShot() == true)
	{
		if (isTimerSet == false)
		{
			//回避できるタイマーをセット
			vanishTimer = 30.0f;
			isTimerSet = true;
		}
		
		//タイマーが0以上の時
		if (vanishTimer > 0)
		{
			//特定の操作をしたら
			if (gamePad->ButtonOffTrigger(B) || input->TriggerKey(DIK_0))
			{
				//回避ゲージが満タンの時
				if (VanishGauge == 3.0f)
				{
					//回避していなかったとき
					if (isVanising == false)
					{
						VanishGauge = 0.0f;
						VanishPos = { playerPos_.x , playerPos_.y, playerPos_.z *-1 };
						playerO_->SetPosition(VanishPos);
						isVanising = true;
					}
				}
			}

		}
		else
		{
			isTimerSet = false;
			vanishTimer = 30.0f;
		}
		
	}
	if (vanishTimer > 0)
	{
		vanishTimer--;
	}
	//回避したら
	if (isVanising == true)
	{
		VanishGauge += 0.1f;
		if (VanishGauge >= 3.0f)
		{
			VanishGauge = 3.0f;
		}
	}
	//回避ゲージが満タンなら
	if (VanishGauge == 3.0f)
	{
		//回避が可能になる
		if (isVanising == true)
		{
			isVanising = false;
		}
	}
}

void Player::Damage()
{}

void Player::RemoveAttribute()
{
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {

		CollisionManager::GetInstance()->RemoveCollider(sphere[i]);
		// こいつはいらない
		/*sphere[i]->GetCollisionInfo().collider->RemoveAttribute(COLLISION_ATTR_PLAYERBULLETS);*/
	}
}

void Player::TitleAnime()
{
	CheckHitCollision();

	srand(( unsigned int ) time(nullptr));

	if ( animevanish == false )
	{
			
		scale_.x -= reduction;
		playerO_->SetScale(scale_);
		if ( playerO_->GetScale().x < 0 )
		{
			vstanCount--;
		
			playerO_->SetScale({ 0,0,0 });

		}

		if ( vstanCount <= 0 )
		{
			randPosX = rand() % 81 - 40;
			randPosZ = rand() % 81 - 40;
			animevanish = true;
		}
	}

	if ( animevanish == true )
	{
		playerO_->SetPosition({ ( float ) randPosX,0,( float ) randPosZ });
		scale_.x += expansion;
		playerO_->SetScale(scale_);
		if ( playerO_->GetScale().x >= 1 )
		{
			animevanish = false;
			vstanCount = 30.0f;
			playerO_->SetScale({ 1,1,1 });

		}
		
	}

	playerO_->Update();
}

void Player::GameOverAnime()
{



	blowAwayCount++;
	
	//水平投射をしながら自機を下に落下させる
	Affin::HorizontalProjection(playerO_->worldTransform, transNormal, 1.0f, blowAwayCount);

	playerO_->SetPosition(playerO_->worldTransform.translation_);

}

void Player::CheckHitCollision()
{

#pragma region オブジェクト同士の押し出し処理
	class PlayerQueryCallBack : public QueryCallback
	{
	public:
		PlayerQueryCallBack(Sphere* sphere) : sphere(sphere) {};

		bool OnQueryHit(const QueryHit& info)
		{
			rejectDir = info.reject;
			rejectDir.nomalize();

			//上方向と排斥方向の角度差のコサイン値
			float cos = rejectDir.dot(up);

			//
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
		//void SphereQuery();

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
		PlayerQueryCallBack callback(sphere[i]);

		//球と地形の交差を全探索する
		CollisionManager::GetInstance()->QuerySphere(*sphere[i], &callback);

		if (sphere[i]->GetIsHit() == true)
		{
			//当たったものの属性が敵だった時
			if (sphere[i]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_ENEMYS)
			{
				playerO_->worldTransform.translation_.x += callback.move.x;
				playerO_->worldTransform.translation_.y += callback.move.y;
				playerO_->worldTransform.translation_.z += callback.move.z;
				break;
			}
		}

	}
#pragma endregion 

	oldPos = wtf.translation_;

	Vector3 distance;

	if (hitDeley > 0) {	//毎フレームヒットを防止
		playerO_->SetColor({ 0,0,1,1 });
		hitDeley--;
	}
	else
	{
		playerO_->SetColor({ 1,1,1,1 });
	}

	//当たり判定の処理
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		if (hitDeley <= 0 && sphere[i]->GetIsHit() == true)
		{
			//当たったものの属性が敵の弾だった時
			if (sphere[i]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_ENEMYBULLETS)
			{
				Hp_ -= 1;
				hitDeley = 5;
				particle_->RandParticle(sphere[i]->GetCollisionInfo().inter_);
				//SetIsHit(true);

				break;
			}
		}

	}
	if (GetisDead() == true)
	{
		for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {

			CollisionManager::GetInstance()->RemoveCollider(sphere[i]);
			//こいつはいらない
			/*sphere[i]->GetCollisionInfo().collider->RemoveAttribute(COLLISION_ATTR_PLAYERBULLETS);*/
		}
	}
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {

		spherePos[i] = playerO_->GetPosition();
		sphere[i]->Update();
	}
}

void Player::moveAngle()
{
	cameraAngle.y = 
		atan2(playerO_->GetCamera()->GetTarget().x - playerO_->GetCamera()->GetEye().x,
			  playerO_->GetCamera()->GetTarget().z - playerO_->GetCamera()->GetEye().z);
}

void Player::Reset()
{
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Reset();

	}

		//デスフラグが立った球を削除
	bullets_.remove_if([ ] (std::unique_ptr<PlayerBullet>& bullet){return bullet->GetIsDead();});

	for ( int i = 0; i < SPHERE_COLISSION_NUM; i++ )
	{

		CollisionManager::GetInstance()->RemoveCollider(sphere[ i ]);
		//こいつはいらない
		/*sphere[i]->GetCollisionInfo().collider->RemoveAttribute(COLLISION_ATTR_PLAYERBULLETS);*/
	}
	ResetAttribute();

	playerO_->SetColor({1, 1, 1, 1});

	playerO_->SetPosition({0,0,-50});
	playerO_->SetRotation({0,0,0});
	oldPos = { 0,0,0 };
	playerPos_ = { 0,0,0 };
	enemyPos_ = { 0,0,0 };
	distance_ = { 0,0,0 };

	Hp_ = 10;
	isDead_ = false;
	faceAngle_ = { 0 , 0 , 0 };
	cameraAngle = { 0,0,0 };
	velocity_ = { 0,0,0 };

	hitDeley = 0;

	VanishGauge = 3.0f;
	isVanising = false;
	VanishPos;

	isShot = false;
	//単発
	oneShot = false;
	//連射
	rapidShot = false;
	//弾の最大個数
	MAX_BULLET = 0;
	//現在の弾の個数
	bulletSize = 0;
	//弾のタイプ
	bulletType = PlayerBulletType::None;
	//弾と弾の間隔時間
	bulletTimer = 0.0f;
	//連射制限のためのクールタイム
	coolTimer = 60.0f;
	//ボタンを押してる時間
	pushTimer = 15.0f;
	//長押ししている時間
	pressTimer = 0.0f;

	//ベジエにしようとしたやつ
	startCount = 0;
	nowCount = 0;
	elapsedCount = 0;
	maxTime = 5.0f;				//全体時間[s]
	timeRate;						//何％時間が進んだか

	particle_->Reset();
	blowAwayCount = 0;
	blowAwayPos = playerO_->GetPosition();

}

void Player::ResetAttribute()
{
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		spherePos[i] = playerO_->GetPosition();
		sphere[i]->SetBasisPos(&spherePos[i]);
		sphere[i]->SetRadius(1.0f);
		sphere[i]->SetAttribute(COLLISION_ATTR_PLAYERS);
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

