#include "PlayerBullet.h"
#include "EnemyCharacter.h"
#include "ImguiManager.h"

PlayerBullet::PlayerBullet() { bulletM_ = Model::CreateFromOBJ("bullet"); }

PlayerBullet::~PlayerBullet() {}

void PlayerBullet::Initialize(const Vector3& position, const Vector3& velocity) {
	isDead_ = false;
	bulletO_ = Object3d::Create();
	bulletO_->SetModel(bulletM_);

	bulletO_->worldTransform.translation_ = position;

	bulletO_->SetColor({ 1.0f,1.0f,0.0f,1.0f });

	velocity_ = velocity;

	angle_ = (atan2(position.x, position.z) + MyMath::PI / 2);
	bulletO_->worldTransform.rotation_.y = (angle_ + MyMath::PI / 2);

	sphere.resize(SPHERE_COLISSION_NUM);
	spherePos.resize(SPHERE_COLISSION_NUM);
	//当たり判定の初期化
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		spherePos[i] = bulletO_->GetPosition();
		sphere[i]->SetBasisPos(&spherePos[i]);
		sphere[i]->SetRadius(1.0f);
		sphere[i]->SetAttribute(COLLISION_ATTR_PLAYERBULLETS);
		sphere[i]->Update();
		////test
		//coliderPosTest_[i] = Object3d::Create();
		//coliderPosTest_[i]->SetModel(hpModel_.get());
		//coliderPosTest_[i]->SetPosition(sphere[i]->center);
		//coliderPosTest_[i]->SetScale({ sphere[i]->GetRadius(),sphere[i]->GetRadius() ,sphere[i]->GetRadius() });
		//coliderPosTest_[i]->SetRotate({ 0,0,0 });
		//coliderPosTest_[i]->Update();

	}
	livingTimer = 120.0f;
}

void PlayerBullet::Update()
{



	Shot();
	CheckCollision();
	bulletO_->Update();

	if (isDead_ == true)
	{
		for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
		{

			CollisionManager::GetInstance()->RemoveCollider(sphere[i]);
			//こいつはいらない
			/*sphere[i]->GetCollisionInfo().collider->RemoveAttribute(COLLISION_ATTR_PLAYERBULLETS);*/

		}
	}
	
	

	bulletO_->worldTransform.UpdateMatWorld();
}

void PlayerBullet::Draw()
{
	bulletO_->Draw();
}

void PlayerBullet::Shot()
{


	livingTimer--;
	if (livingTimer <= 0)
	{
		isDead_ = true;
		livingTimer = 120.0f;
		
	}

	bulletO_->worldTransform.translation_ += velocity_;
}

void PlayerBullet::CheckCollision()
{

	if (hitDeley > 0) {	//毎フレームヒットを防止
		hitDeley--;
		
	}



	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		if (hitDeley <= 0 && sphere[i]->GetIsHit() == true)
		{
			if (sphere[i]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_ENEMYS)
			{
				isDead_ = true;
				livingTimer = 120.0f;
				hitDeley = 4;
				break;
			}
			if (sphere[i]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_ENEMYBULLETS)
			{
				isDead_ = true;
				livingTimer = 120.0f;
				hitDeley = 4;
				break;
			}
		}



	}
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		spherePos[i] = bulletO_->GetPosition();
		sphere[i]->Update();
	}

	
	

	/*ImGui::Begin("livingbullet");
	ImGui::SetWindowPos({ 800 , 400 });
	ImGui::SetWindowSize({ 500,100 });
	ImGui::InputFloat("livindBullet",&livingTimer);

	ImGui::End();*/

}

void PlayerBullet::Reset()
{
	livingTimer = 120.0f;

	isDead_ = true;
	if (isDead_ == true)
	{
		for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
		{
			CollisionManager::GetInstance()->RemoveCollider(sphere[i]);
			//こいつはいらない
			/*sphere[i]->GetCollisionInfo().collider->RemoveAttribute(COLLISION_ATTR_PLAYERBULLETS);*/

		}
	}
	
}
