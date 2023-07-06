#include "Player.h"
#include "Enemy.h"

void Player::Initialize(DirectXCommon* dxCommon, Input* input,GamePad* gamePad, Enemy* enemy)
{
	dxCommon_ = dxCommon;
	input_ = input;
	gamePad_ = gamePad;

	//�f�o�C�X�̃Z�b�g
	FbxObject3d::SetDevice(dxCommon_->GetDevice());
	//�O���t�B�b�N�X�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();

	////�G��Fbx�ǂݍ���
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

	////FBX�����蔻�菉����
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
	//�f�X�t���O�������������폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->GetIsDead(); });

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	Move();

	if (gamePad_->ButtonInput(A) || input_->PushKey(DIK_SPACE))
	{
		if (isShot == false)
		{
			isShot = true;
		}
		Shot();
	}

	

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
		player_.translation_.z += 0.1f;
		playerO_->SetPosition(player_.translation_);
	}
	if (gamePad_->StickInput(L_DOWN)|| input_->PushKey(DIK_S))
	{
		player_.translation_.z -= 0.1f;
		playerO_->SetPosition(player_.translation_);
	}
	if (gamePad_->StickInput(L_LEFT) || input_->PushKey(DIK_A))
	{
		player_.translation_.x -= 0.1f;
		playerO_->SetPosition(player_.translation_);
	}
	if (gamePad_->StickInput(L_RIGHT) || input_->PushKey(DIK_D))
	{
		player_.translation_.x += 0.1f;
		playerO_->SetPosition(player_.translation_);
	}
}

void Player::Shot()
{

	const float speed = 0.5f;

	

	bulletSize = bullets_.size();

	Vector3 playerPos;
	Vector3 enemyPos;
	Vector3 distance;
	bulletTimer--;


	playerPos = playerO_->worldTransform.translation_;

	enemyPos = enemy_->GetPosition();

	distance = enemyPos - playerPos  ;

	distance.nomalize();

	distance *= speed;

	
	if (bulletSize < 10)
	{
		if (bulletTimer <= 0)
		{
			if (isShot == true)
			{
				

				// �e�𐶐���������
				std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
				newBullet->Initialize(bulletM_, playerO_->worldTransform.translation_,distance);

				bulletTimer = 30.0f;

				//
				newBullet->SetEnemy(enemy_);

  				bullets_.push_back(std::move(newBullet));
			}

		}
		
	}
	
	
	
}
