#include "Player.h"
#include "Enemy.h"
#include "ImguiManager.h"
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

	wtf.translation_ = { 0,0,-50 };
	//playerO_->SetScale({ 2,2,2 });
	playerO_->SetPosition(wtf.translation_);

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
	startCount = clock() / 1000;
}

void Player::Update()
{
	//�J�����̊p�x�̍X�V
	moveAngle();

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


	//�f�X�t���O�������������폜
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
	////�s��̍X�V�Ȃ�
	playerO_->UpdateMatrix();




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

	velocity_ = { 0 , 0 , 0 };

	if (gamePad_->StickInput(L_UP) || input_->PushKey(DIK_W))
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

	if (gamePad_->StickInput(L_DOWN)|| input_->PushKey(DIK_S))
	{
		//wtf.translation_.z -= 0.5f;
		//playerO_->SetPosition(wtf.translation_);
		velocity_ += { 0 , 0 , moveSpeed * -1 };

	}
	if (gamePad_->StickInput(L_LEFT) || input_->PushKey(DIK_A))
	{
		//wtf.translation_.x -= 0.5f;
		//playerO_->SetPosition(wtf.translation_);
		velocity_ += { moveSpeed * -1 , 0 , 0 };
	}

	if (gamePad_->StickInput(L_RIGHT) || input_->PushKey(DIK_D))
	{
		//wtf.translation_.x += 0.5f;
		//playerO_->SetPosition(wtf.translation_);

		velocity_ += { moveSpeed , 0 , 0 };
	}
	playerO_->worldTransform.rotation_ = cameraAngle;

	/*playerO_->worldTransform.UpdateMatWorld();*/

	velocity_ = MyMath::MatVector(velocity_, playerO_->worldTransform.matWorld_);

	playerO_->worldTransform.translation_ += velocity_;


	ImGui::Begin("playerPos");

	ImGui::SetWindowPos({ 200 , 200 });
	ImGui::SetWindowSize({ 200,100 });
	ImGui::InputFloat3("x", &playerO_->worldTransform.translation_.x);
	ImGui::InputFloat3("x", &playerO_->worldTransform.rotation_.x);
	/*ImGui::InputFloat3("z", &playerO_->worldTransform.translation_.z);*/

	ImGui::End();

}

void Player::Shot()
{
	nowCount++;

	elapsedCount = nowCount - startCount;
	float elapsedTime = static_cast<float> (elapsedCount) / 1000000.0f;

	timeRate = min(elapsedTime / maxTime, 1.0f);
#pragma region �e�̈ړ�����
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

	//�������Ƃ�
	if (gamePad_->ButtonInput(A) || input_->PushKey(DIK_SPACE))
	{
		pushTimer--;
		pressTimer--;
		
		//�A�˂̎�
		if (pushTimer < 0)
		{
			rapidShot = true;
		}
	}
	

	if (oneShot == true)
	{
		rapidShot = false;
		//�e�̍ő��
		MAX_BULLET = 1;
		pushTimer = 15.0f;//�����Ă鎞��
		pressTimer = 0.0f;//�A�˗p�̎���
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

		//�e���ő���ȉ���������
		if (bulletSize < MAX_BULLET)
		{
			if (bulletTimer <= 0)
			{
				if (isShot == true)
				{
					bulletSize++;
					// �e�𐶐���������
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
	//�������Ƃ�
	if (gamePad_->ButtonOffTrigger(A) || input_->ReleaseKey(DIK_SPACE))
	{
		//�P���̎�
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

#pragma region �I�u�W�F�N�g���m�̉����o������
	class PlayerQueryCallBack : public QueryCallback
	{
	public:
		PlayerQueryCallBack(Sphere* sphere) : sphere(sphere) {};

		bool OnQueryHit(const QueryHit& info)
		{
			rejectDir = info.reject;
			rejectDir.nomalize();

			//������Ɣr�˕����̊p�x���̃R�T�C���l
			float cos = rejectDir.dot(up);

			//
			const float threshold = cosf(XMConvertToRadians(30.0f));
			//�p�x���ɂ���ēV��܂��͒n�ʂƔ��肳���ꍇ��������
			if (-threshold < cos && cos < threshold)
			{
				//�����o��
				sphere->center += info.reject;
				move += info.reject;
			}
			return true;
		}
		void SphereQuery();

		//���[���h�̏����
		const Vector3 up = { 0,1,0 };
		//�r�˕���
		Vector3 rejectDir;
		//�N�G���[�Ɏg�p���鋅
		Sphere* sphere = nullptr;
		//�r�˂ɂ��ړ���
		Vector3 move = {};

	};


	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		PlayerQueryCallBack callback(sphere[i]);

		//���ƒn�`�̌�����S�T������
		CollisionManager::GetInstance()->QuerySphere(*sphere[i], &callback);

		if (sphere[i]->GetIsHit() == true)
		{
			if (sphere[i]->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ENEMYS)
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

	if (hitDeley > 0) {	//���t���[���q�b�g��h�~
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

void Player::moveAngle()
{
	cameraAngle.y = 
		atan2(playerO_->GetCamera()->GetTarget().x - playerO_->GetCamera()->GetEye().x,
			  playerO_->GetCamera()->GetTarget().z - playerO_->GetCamera()->GetEye().z);
}

