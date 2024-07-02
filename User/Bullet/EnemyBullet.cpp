#include "EnemyBullet.h"
#include "PlayerCharacter.h"
#include "ImguiManager.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity, const Vector3& Rotate)
{
	isDead_ = false;
	bulletO_ = Object3d::Create();
	bulletO_->SetModel(model);

	bulletO_->worldTransform.translation_ = position;

	bulletO_->SetColor({ 1.0f,0.0f,0.0f,1.0f });

	velocity_ = velocity;

	bulletO_->worldTransform.rotation_ = Rotate;

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
		sphere[i]->SetAttribute(COLLISION_ATTR_ENEMYBULLETS);
		sphere[i]->Update();

	}

	
	livingTimer = 120.0f;
}

void EnemyBullet::Update()
{
	

	Shot();
	CheckCollision();
	bulletO_->Update();

	bulletO_->worldTransform.UpdateMatWorld();

}

void EnemyBullet::Draw()
{
	bulletO_->Draw();
}

void EnemyBullet::Shot()
{
	livingTimer--;
	if (livingTimer <= 0)
	{
		SetisDead(true);
		livingTimer = 120.0f;

	}
	bulletO_->worldTransform.translation_ += velocity_;
}

void EnemyBullet::CheckCollision()
{
	if (hitDeley > 0) {	//毎フレームヒットを防止
		hitDeley--;

	}

	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		if (hitDeley <= 0 && sphere[i]->GetIsHit() == true)
		{
			if (sphere[i]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_PLAYERS)
			{
				isDead_ = true;
				livingTimer = 120.0f;
				hitDeley = 4;
				break;
			}
			if (sphere[i]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_PLAYERBULLETS)
			{
				isDead_ = true;
				livingTimer = 120.0f;
				hitDeley = 4;
				break;
			}
			if ( sphere[ i ]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_PLAYERBOMBBULLETS )
			{
				isDead_ = true;
				livingTimer = 120.0f;
				hitDeley = 4;
				break;
			}
		}

	}
	if (isDead_ == true)
	{
		for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
		{
			CollisionManager::GetInstance()->RemoveCollider(sphere[i]);

		}
	}
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		spherePos[i] = bulletO_->GetPosition();
		sphere[i]->Update();
	}
}

void EnemyBullet::Reset()
{
	isDead_ = true;
	livingTimer = 120.0f;

	if (isDead_ == true) {
		for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
			CollisionManager::GetInstance()->RemoveCollider(sphere[i]);
		}
	}
	
}
