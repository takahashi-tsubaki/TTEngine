#include "BombBullet.h"
#include "EnemyCharacter.h"
#include "ImguiManager.h"

Model* BombBullet::model_ = nullptr;	

BombBullet::BombBullet() {  }

BombBullet::~BombBullet() {}

void BombBullet::Initialize(
	const Vector3& position,const Vector3& velocity,const unsigned short attribute) {


	isDead_ = false;
	bulletO_ = Object3d::Create();
	bulletO_->SetModel(model_);

	bulletO_->worldTransform.translation_ = position;
	bulletO_->worldTransform.scale_ = { 1,1,1 };
	bulletO_->color_ = { 0,0,0,1 };
	if ( attribute == COLLISION_ATTR_PLAYERBULLETS )
	{
		bulletO_->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
	}

	if ( attribute == COLLISION_ATTR_ENEMYBULLETS )
	{
		bulletO_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	}
	velocity_ = velocity;

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
		sphere[ i ]->SetRadius(1.0f);
		sphere[ i ]->SetAttribute(attribute);
		sphere[ i ]->Update();
		////test
		// coliderPosTest_[i] = Object3d::Create();
		// coliderPosTest_[i]->SetModel(hpModel_.get());
		// coliderPosTest_[i]->SetPosition(sphere[i]->center);
		// coliderPosTest_[i]->SetScale({ sphere[i]->GetRadius(),sphere[i]->GetRadius()
		// ,sphere[i]->GetRadius() }); coliderPosTest_[i]->SetRotate({ 0,0,0 });
		// coliderPosTest_[i]->Update();
	}

	//particle_ = std::make_unique<ParticleManager>();
	//particle_->SetDrawBlendMode(1);
	//particle_->Initialize();
	//particle_->LoadTexture("sprite/particle.png");
	//particle_->Update();

	livingTimer = 45.0f;
}

void BombBullet::Update() {

	Shot();
	CheckCollision();
	bulletO_->Update();
	//particle_->Update();
	if ( isDead_ == true )
	{
		for ( int i = 0; i < SPHERE_COLISSION_NUM; i++ )
		{

			CollisionManager::GetInstance()->RemoveCollider(sphere[ i ]);
			// こいつはいらない
			/*sphere[i]->GetCollisionInfo().collider->RemoveAttribute(COLLISION_ATTR_PLAYERBULLETS);*/
		}
	}

	bulletO_->worldTransform.UpdateMatWorld();
}

void BombBullet::Draw() {
	bulletO_->Draw();
}

void BombBullet::Shot() {

	livingTimer--;
	bulletO_->worldTransform.scale_.x -= 0.013f;
	bulletO_->worldTransform.scale_.y -= 0.013f;
	bulletO_->worldTransform.scale_.z -= 0.013f;
	if ( livingTimer <= 0 )
	{
		isDead_ = true;
		livingTimer = 45.0f;
	}

	bulletO_->worldTransform.translation_ += velocity_;
}

void BombBullet::CheckCollision() {

	if ( hitDeley > 0 )
	{ // 毎フレームヒットを防止
		hitDeley--;
	}

	for ( int i = 0; i < SPHERE_COLISSION_NUM; i++ )
	{

		if ( hitDeley <= 0 && sphere[ i ]->GetIsHit() == true )
		{
//弾の属性が自機の弾で当たった対象が敵だった時
			if ( sphere[ i ]->GetAttribute() == COLLISION_ATTR_PLAYERBULLETS
				&& sphere[ i ]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_ENEMYS )
			{
				isDead_ = true;
				livingTimer = 120.0f;
				//particle_->RandParticle(sphere[i]->GetCollisionInfo().inter_);
				hitDeley = 4;
				break;
			}
			// 弾の属性が自機の弾で当たった対象が敵の弾だった時
			if ( sphere[ i ]->GetAttribute() == COLLISION_ATTR_PLAYERBULLETS &&
				sphere[ i ]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_ENEMYBULLETS )
			{
				isDead_ = true;
				livingTimer = 120.0f;
				//particle_->RandParticle(sphere[i]->GetCollisionInfo().inter_);
				hitDeley = 4;
				break;
			}
			if ( sphere[ i ]->GetAttribute() == COLLISION_ATTR_PLAYERBOMBBULLETS
				&& sphere[ i ]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_ENEMYS )
			{
				isDead_ = true;
				livingTimer = 120.0f;
				//particle_->RandParticle(sphere[i]->GetCollisionInfo().inter_);
				hitDeley = 4;
				break;
			}
		}
	}
	for ( int i = 0; i < SPHERE_COLISSION_NUM; i++ )
	{
		if ( hitDeley <= 0 && sphere[ i ]->GetIsHit() == true )
		{
			// 弾の属性が敵の弾で当たった対象が自機だった時
			if ( sphere[ i ]->GetAttribute() == COLLISION_ATTR_ENEMYBULLETS
				&& sphere[ i ]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_PLAYERS )
			{
				isDead_ = true;
				livingTimer = 120.0f;
				//particle_->RandParticle(sphere[i]->GetCollisionInfo().inter_);
				hitDeley = 4;
				break;
			}
			// 弾の属性が敵の弾で当たった対象が自機の弾だった時
			if ( sphere[ i ]->GetAttribute() == COLLISION_ATTR_ENEMYBULLETS
				&& sphere[ i ]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_PLAYERBULLETS )
			{
				isDead_ = true;
				livingTimer = 120.0f;
				//particle_->RandParticle(sphere[i]->GetCollisionInfo().inter_);
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

	/*ImGui::Begin("livingbullet");
	ImGui::SetWindowPos({ 800 , 400 });
	ImGui::SetWindowSize({ 500,100 });
	ImGui::InputFloat("livindBullet",&livingTimer);

	ImGui::End();*/
}

void BombBullet::Reset() {
	livingTimer = 120.0f;

	isDead_ = true;
	if ( isDead_ == true )
	{
		for ( int i = 0; i < SPHERE_COLISSION_NUM; i++ )
		{
			CollisionManager::GetInstance()->RemoveCollider(sphere[ i ]);
			// こいつはいらない
			/*sphere[i]->GetCollisionInfo().collider->RemoveAttribute(COLLISION_ATTR_PLAYERBULLETS);*/
		}
	}
}
