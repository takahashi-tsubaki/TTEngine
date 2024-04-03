#include "EnemyShot.h"

EnemyShot::EnemyShot(EnemyActionManager* ActionManager, int actionNum) {

	ActionManager_ = ActionManager;
	actionNum_ = actionNum;
}

EnemyShot::~EnemyShot()
{
	Reset();
}

void EnemyShot::StaticInitialize() {}

void EnemyShot::Initialize(FbxObject3d* object, PlayerCharacter* player)
{
	object_ = object;
	player_ = player;

}

void EnemyShot::Update()
{
	speed_ = 1.0f;
	enemyPos = object_->GetPosition();
	playerPos = player_->GetFbxObject3d()->GetPosition();
	Distance_ = playerPos - enemyPos ;
;
	if (actionNum_ >= 7)
	{
		shotType_ = EnemyShotType::ONESHOT;
		if ( isShot == false )
		{
			isShot = true;
		}
	}
	else
	{
		shotType_ = EnemyShotType::RAPIDSHOT;
		if (isShot == false)
		{
			isShot = true;
		}
	}

	if ( shotType_ == EnemyShotType::ONESHOT )
	{
		MaxBulletSize_ = 1;
	}
	else if(shotType_ == EnemyShotType::RAPIDSHOT)
	{
		MaxBulletSize_ = 5;
		speed_ = 2.0f;
	}

	Distance_.nomalize();

	Distance_ *= speed_;

	if (isShot == true)
	{
		bulletCoolTimer_--;
		// 射撃アニメーションの再生
		object_->PlayAnimation(FBXAnimetion::ShoT, false);
		if (bulletSize_ < MaxBulletSize_)
		{
			if ( bulletCoolTimer_ <= 0 )
			{
				bulletSize_++;
				std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
				newBullet->Initialize(
				    object_->worldTransform.translation_, Distance_, COLLISION_ATTR_ENEMYBULLETS);

				if (shotType_ == EnemyShotType::ONESHOT) {
					bulletCoolTimer_ = 30.0f;
				} else if (shotType_ == EnemyShotType::RAPIDSHOT) {
					bulletCoolTimer_ = 15.0f;
				}

				newBullet->SetPlayer(player_);

				BulletManager::GetInstance()->AddBullet(std::move(newBullet));
			}
		}
		if (bulletSize_ >= MaxBulletSize_)
		{
			koutyokuTimer_--;
		}
		if ( koutyokuTimer_ < 0 )
		{
			bulletSize_ = 0;
			MaxBulletSize_ = 0;

			shotType_ = EnemyShotType::NONE;
			bulletCoolTimer_ = 0.0f;
			koutyokuTimer_ = 120.0f;
			isNowShot_ = false;

			isShot = false; // こいつのリセット処理は一番最後

		}
	}


}

void EnemyShot::Draw() {}

void EnemyShot::Reset()
{
	speed_ = 0.5f;
	scale_ = 0.01f;
	bulletSizeUpTimer_ = 10.0f;
	MaxBulletSize_ = 0;
	bulletSize_ = 0;
	shotType_ = EnemyShotType::NONE;
	bulletCoolTimer_ = 0.0f;
	isShot = false;
}
