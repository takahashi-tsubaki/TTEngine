#include "Shot.h"
#include "Character.h"
Shot::Shot(PlayerActionManager* ActionManager) {

	ActionManager_ = ActionManager;
	StaticInitialize();
	//bullets_ = bullet;
}

Shot::~Shot() {
	Reset();
	
}

void Shot::StaticInitialize() { bulletM_ = Model::CreateFromOBJ("bullet"); }

void Shot::Initialize(FbxObject3d* object, EnemyCharacter* enemy)
{
	object_ = object;
	enemy_ = enemy;
	bulletSizeUpTimer_ = 10.0f;
	
}

void Shot::Update(Input* input, GamePad* gamePad)
{




	speed_ = 0.5f;
	enemyPos = enemy_->GetFbxObject3d()->GetPosition();
	playerPos = object_->GetPosition();
	Distance_ = enemyPos - playerPos;
	Distance_.nomalize();

	Distance_ *= speed_;

	if (gamePad->ButtonInput(A) || input->PushKey(DIK_SPACE))
	{
		keyPressTimer_++;
	}
	//単発か連続かの
	if ( keyPressTimer_ < 5.0f )
	{
		shotType_ = ShotType::ONE;
	}
	else
	{
		shotType_ = ShotType::RAPID;
	}

	if (shotType_ == ShotType::RAPID)
	{
		if (gamePad->ButtonInput(A) || input->PushKey(DIK_SPACE)) {
			bulletSizeUpTimer_-- ;

		}
		if ( bulletSizeUpTimer_ <= 0.0f )
		{
			if ( MaxBulletSize_ < 20 )
			{
				MaxBulletSize_++;
			}

			bulletSizeUpTimer_ = 10.0f;
		}
	}
	else
	{
		MaxBulletSize_ = 1;
	}

	if ( gamePad->ButtonOffTrigger(A) || input->ReleaseKey(DIK_SPACE) )
	{
		if ( isShot == false )
		{
			isShot = true;
		}
	}
	if ( isShot == true )
	{
		bulletCoolTimer_--;
		// 射撃アニメーションの再生
		object_->PlayAnimation(FBXAnimetion::ShoT, false);
		if (bulletSize_ < MaxBulletSize_)
		{
			if (bulletCoolTimer_ <= 0)
			{
				bulletSize_++;
				std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
				newBullet->Initialize(object_->worldTransform.translation_, Distance_);

				if (shotType_ == ShotType::ONE) {
					bulletCoolTimer_ = 15.0f;
				} else if (shotType_ == ShotType::RAPID) {
					bulletCoolTimer_ = 5.0f;
				}

				newBullet->SetEnemy(enemy_);

				BulletManager::GetInstance()->AddBullet(std::move(newBullet));
			}

			
		}
		if (bulletSize_ >= MaxBulletSize_)
		{
			MaxBulletSize_ = 0;
			bulletSize_ = 0;
			shotType_ = ShotType::Nasi;
			bulletCoolTimer_ = 0.0f;
			keyPressTimer_ = 0.0f;
			isShot = false;
		}
	}

}

void Shot::Draw()
{

}

void Shot::Reset() {

	speed_ = 0.5f;
	scale_ = 0.01f;
	bulletSizeUpTimer_ = 10.0f;
	MaxBulletSize_ = 0;
	bulletSize_ = 0;
	shotType_ = ShotType::Nasi;
	bulletCoolTimer_ = 0.0f;
	keyPressTimer_ = 0.0f;
	isShot = false;
}



//void Shot::Distance(Vector3 start, Vector3 goal) {}
//
//void Shot::DistanceTwoPoints(Vector3 start, Vector3 goal) {}
