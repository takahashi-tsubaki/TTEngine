#include "Player.h"
#include "Enemy.h"
#include "ImguiManager.h"
void Player::Initialize(DirectXCommon* dxCommon, Input* input,GamePad* gamePad, Enemy* enemy)
{
	dxCommon_ = dxCommon;
	input_ = input;
	gamePad_ = gamePad;

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

	playerM_ = Model::CreateFromOBJ("cube");

	playerO_->SetModel(playerM_);

	wtf.translation_ = { 0,0,-50 };
	playerO_->SetScale({ 2,2,2 });
	playerO_->SetPosition(wtf.translation_);

	bulletM_ = Model::CreateFromOBJ("cube");
	//playerFbxO_->SetPosition(player_.translation_);

	enemy_ = enemy;

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
}

void Player::Update()
{
	if (Hp_ <= 0)
	{
		SetisDead(true);
	}

	if (input_->TriggerKey(DIK_R))
	{
		SetHp(10);
		SetisDead(false);
		enemy_->SetHp(30);
		enemy_->SetisDead(false);
	}

	oldPos = wtf.translation_;
	CheckHitCollision();

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
		Move();
	}

	Shot();
	//行列の更新など
	playerO_->UpdateMatrix();

#pragma region オブジェクト同士の押し出し処理
//	class PlayerQueryCallBack : public QueryCallback
//	{
//	public:
//		PlayerQueryCallBack(Sphere* sphere) : sphere(sphere) {};
//
//		bool OnQueryHit(const QueryHit& info)
//		{
//			rejectDir = info.reject;
//			rejectDir.nomalize();
//
//			上方向と排斥方向の角度差のコサイン値
//			float cos = rejectDir.dot(up);
//
//			地面判定しきい値角度
//			const float threshold = cosf(XMConvertToRadians(30.0f));
//			角度差によって天井または地面と判定される場合を除いて
//			if (-threshold < cos && cos < threshold)
//			{
//				押し出す
//				sphere->center += info.reject;
//				move += info.reject;
//			}
//			return true;
//		}
//		void SphereQuery();
//
//		ワールドの上方向
//		const Vector3 up = { 0,1,0 };
//		排斥方向
//		Vector3 rejectDir;
//		クエリーに使用する球
//		Sphere* sphere = nullptr;
//		排斥による移動量
//		Vector3 move = {};
//
//	};
//
//
//	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
//	{
//		PlayerQueryCallBack callback(sphere[i]);
//
//		球と地形の交差を全探索する
//		CollisionManager::GetInstance()->QuerySphere(*sphere[i], &callback);
//
//		wtf.translation_.x += callback.move.x;
//		wtf.translation_.y += callback.move.y;
//		wtf.translation_.z += callback.move.z;
//
//		playerO_->SetPosition(wtf.translation_);
//		playerO_->UpdateMatrix();
//		sphere[i]->Update();
//	}
#pragma endregion 

	
	
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
		playerO_->Draw();
		/*playerFbxO_->Draw(dxCommon_->GetCommandList());*/
	}

}

void Player::Move()
{
	/*if (input_->PushKey(DIK_A))
	{
		player_.translation_.x -= 0.1f;
		playerFbxO_->SetPosition(player_.translation_);
	}
	if (input_->PushKey(DIK_D))
	{
		player_.translation_.x += 0.1f;
		playerFbxO_->SetPosition(player_.translation_);
	}
	if (input_->PushKey(DIK_W))
	{
		player_.translation_.z += 0.1f;
		playerFbxO_->SetPosition(player_.translation_);
	}
	if (input_->PushKey(DIK_S))
	{
		player_.translation_.z -= 0.1f;
		playerFbxO_->SetPosition(player_.translation_);
	}*/

	/*if (input_->PushKey(DIK_A))
	{
		player_.translation_.x -= 0.1f;
		playerO_->SetPosition(player_.translation_);
	}
	if (input_->PushKey(DIK_D))
	{
		player_.translation_.x += 0.1f;
		playerO_->SetPosition(player_.translation_);
	}
	if (input_->PushKey(DIK_W))
	{
		player_.translation_.z += 0.1f;
		playerO_->SetPosition(player_.translation_);
	}
	if (input_->PushKey(DIK_S))
	{
		player_.translation_.z -= 0.1f;
		playerO_->SetPosition(player_.translation_);
	}*/

	if (gamePad_->StickInput(L_UP) || input_->PushKey(DIK_W))
	{
		//enemyPos = enemy_->GetPosition();
		//distance = enemy_->GetPosition() - playerO_->GetPosition();
		//distance.nomalize();
		//distance *= 0.5f;
		//wtf.translation_ += distance;
		wtf.translation_.z += 0.5f;
		playerO_->SetPosition(wtf.translation_);
	}
	if (gamePad_->StickInput(L_DOWN)|| input_->PushKey(DIK_S))
	{
		wtf.translation_.z -= 0.5f;
		playerO_->SetPosition(wtf.translation_);
	}
	if (gamePad_->StickInput(L_LEFT) || input_->PushKey(DIK_A))
	{
		wtf.translation_.x -= 0.5f;
		playerO_->SetPosition(wtf.translation_);
	}
	if (gamePad_->StickInput(L_RIGHT) || input_->PushKey(DIK_D))
	{
		wtf.translation_.x += 0.5f;
		playerO_->SetPosition(wtf.translation_);
	}
}

void Player::Shot()
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

	enemyPos = enemy_->GetPosition();

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
	if (gamePad_->ButtonInput(A) || input_->PushKey(DIK_SPACE))
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

			/*coolTimer--;*/
			/*if (coolTimer < 0)
			{
				bulletSize = 0;
				coolTimer = 60.0f;
				pushTimer = 24.0f;
				pressTimer = 0.0f;
			}*/
		}
	}
	//離したとき
	if (gamePad_->ButtonOffTrigger(A) || input_->ReleaseKey(DIK_SPACE))
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

void Player::CheckHitCollision()
{
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		if (sphere[i]->GetIsHit() == true)
		{
			if (sphere[i]->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ENEMYS)
			{
				wtf.translation_ = oldPos;
				playerO_->SetPosition(wtf.translation_);
				break;
			}
		}

	}

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

	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		if (hitDeley <= 0 && sphere[i]->GetIsHit() == true)
		{
			if (sphere[i]->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ENEMYBULLETS)
			{
				Hp_ -= 1;
				hitDeley = 5;
				//SetIsHit(true);

				break;
			}
		}

	}
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		spherePos[i] = playerO_->GetPosition();
		sphere[i]->Update();
	}
}

