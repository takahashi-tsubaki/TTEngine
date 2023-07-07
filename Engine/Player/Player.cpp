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

	player_.translation_ = { 0,0,-50 };
	playerO_->SetPosition(player_.translation_);

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

}

void Player::Update()
{
	//デスフラグが立った球を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->GetIsDead(); });

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}


	if (bulletType != BulletType::RapidShot)
	{
		Move();
	}



	Shot();
	

	ImGui::Begin("BulletSize");
	ImGui::SetWindowPos({ 600 , 400 });
	ImGui::SetWindowSize({ 500,300 });
	ImGui::InputInt("BulletSize", &bulletSize);
	ImGui::InputFloat("pushTimer",&pushTimer);
	ImGui::InputFloat("pressTimer", &pressTimer);
	ImGui::InputInt("MAX", &MAX_BULLET);
	ImGui::End();
	

	playerO_->Update();
	/*playerFbxO_->Update();*/
}

void Player::Draw()
{
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw();
	}
	playerO_->Draw();
	/*playerFbxO_->Draw(dxCommon_->GetCommandList());*/
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
		player_.translation_.z += 0.5f;
		playerO_->SetPosition(player_.translation_);
	}
	if (gamePad_->StickInput(L_DOWN)|| input_->PushKey(DIK_S))
	{
		player_.translation_.z -= 0.5f;
		playerO_->SetPosition(player_.translation_);
	}
	if (gamePad_->StickInput(L_LEFT) || input_->PushKey(DIK_A))
	{
		player_.translation_.x -= 0.5f;
		playerO_->SetPosition(player_.translation_);
	}
	if (gamePad_->StickInput(L_RIGHT) || input_->PushKey(DIK_D))
	{
		player_.translation_.x += 0.5f;
		playerO_->SetPosition(player_.translation_);
	}
}

void Player::Shot()
{
#pragma region 弾の移動処理
	float speed = 0.5f;
	if (bulletType == BulletType::OneShot)
	{
		speed = 1.0f;
	}
	else if (bulletType == BulletType::RapidShot)
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

					if (bulletType == BulletType::OneShot)
					{
						bulletTimer = 15.0f;
					}
					else if (bulletType == BulletType::RapidShot)
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
			bulletType = BulletType::None;
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
			bulletType = BulletType::OneShot;
			oneShot = true;
		}
		else
		{
			bulletType = BulletType::RapidShot;
		}
		if (isShot == false)
		{
			isShot = true;
		}
	}

}
