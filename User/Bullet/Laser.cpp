#include "Laser.h"


Model* Laser::model_ = nullptr;

void Laser::Initialize(const Vector3& position,const Vector3& velocity,const unsigned short attribute)
{

	isDead_ = false;
	bulletO_ = Object3d::Create();
	bulletO_->SetModel(model_);
	bulletO_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	bulletO_->worldTransform.translation_ = position;
	velocity_ = velocity;
	attribute_ = attribute;

	angle_ = ( atan2(velocity.x,velocity.z) + MyMath::PI / 2 );
	bulletO_->worldTransform.rotation_.y = ( angle_ + MyMath::PI / 2 );


	SPHERE_COLISSION_NUM = 1;
	sphere.resize(SPHERE_COLISSION_NUM);
	spherePos.resize(SPHERE_COLISSION_NUM);
	// 当たり判定の初期化
	for ( int i = 0; i < SPHERE_COLISSION_NUM; i++ )
	{
		sphere[ i ] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[ i ]);
		spherePos[ i ] = bulletO_->GetPosition();
		sphere[ i ]->SetBasisPos(&spherePos[ i ]);
		sphere[ i ]->SetRadius(4.0f);
		sphere[ i ]->SetAttribute(attribute_);
		sphere[ i ]->Update();
	}
	livingTimer = 60.0f;
}

void Laser::Update()
{
	LaserShot();
	CheckCollision();
	bulletO_->Update();
	if ( isDead_ == true )
	{
		for ( int i = 0; i < SPHERE_COLISSION_NUM; i++ )
		{
			CollisionManager::GetInstance()->RemoveCollider(sphere[ i ]);
		}
	}

	bulletO_->worldTransform.UpdateMatWorld();
}

void Laser::Draw()
{
	bulletO_->Draw();
}

void Laser::LaserShot()
{
	livingTimer--;
	if ( livingTimer <= 0 )
	{
		isDead_ = true;
		livingTimer = 60.0f;
	}

	bulletO_->worldTransform.translation_ += velocity_;
}

void Laser::CheckCollision()
{
	if ( hitDeley > 0 )
	{ // 毎フレームヒットを防止
		hitDeley--;
	}

	for ( int i = 0; i < SPHERE_COLISSION_NUM; i++ )
	{

		if ( hitDeley <= 0 && sphere[i]->GetIsHit() == true )
		{

			// 弾の属性が敵の弾で当たった対象が自機だった時
			if ( sphere[ i ]->GetAttribute() == COLLISION_ATTR_ENEMYLASERS
				&& sphere[ i ]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_PLAYERS )
			{
				isDead_ = true;
				livingTimer = 120.0f;
				hitDeley = 4;
				break;
			}
			if ( sphere[ i ]->GetAttribute() == COLLISION_ATTR_ENEMYLASERS
				&& sphere[ i ]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_PLAYERBOMBBULLETS )
			{
				isDead_ = true;
				livingTimer = 120.0f;
				hitDeley = 4;
				break;
			}
		}
	}
	
	for ( int i = 0; i < SPHERE_COLISSION_NUM; i++ )
	{
		spherePos[ i ] = bulletO_->GetPosition();
		sphere[ i ]->Update();
	}

}
