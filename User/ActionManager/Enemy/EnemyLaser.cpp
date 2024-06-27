#include "EnemyLaser.h"
#include "PlayerCharacter.h"



EnemyLaser::EnemyLaser(EnemyActionManager* ActionManager)
{
	ActionManager_ = ActionManager;
	isNowShot_ = true;
}

EnemyLaser::~EnemyLaser()
{

	ChargeReset();
}

void EnemyLaser::Initialize(FbxObject3d* object, PlayerCharacter* player)
{

	object_ = object;
	player_ = player;
}

void EnemyLaser::Update()
{
	if ( isShot_ == false )
	{
		chargeCount--;

		enemyPos = object_->GetPosition();
		playerPos = player_->GetFbxObject3d()->GetPosition();
		Distance_ = playerPos - enemyPos ;
		object_->PlayAnimation(FBXAnimetion::Charge,false);
		if ( chargeCount <= 0 )
		{
			speed_ = 3.0f;
			isShot_ = true;
		}
	}

	if ( isShot_ == true )
	{

		Distance_.nomalize();

		Distance_ *= speed_;
		if ( bulletSize_ < MaxBulletSize_ )
		{
			bulletSize_++;
			std::unique_ptr<Laser> newBullet = std::make_unique<Laser>();
			newBullet->Initialize(object_->worldTransform.translation_,Distance_,COLLISION_ATTR_ENEMYLASERS);

			newBullet->SetPlayer(player_);

			BulletManager::GetInstance()->AddLaser(std::move(newBullet));
			isNowShot_ = false;
		}

	}
}

void EnemyLaser::Draw()
{
}

void EnemyLaser::Distance(Vector3 start, Vector3 goal)
{
	Distance_ = { goal.x - start.x, goal.y - start.y, goal.z - start.z };
}

void EnemyLaser::DistanceTwoPoints(Vector3 start, Vector3 goal)
{
	Distance(start, goal);
	Distance_.z = sqrtf(pow(Distance_.x, 2.0f) + pow(Distance_.z, 2.0f));
}

void EnemyLaser::ChargeReset()
{
	chargeCount = 120.0f;
	MaxBulletSize_ = 1;
	bulletSize_ = 0;
	isShot_ = false; 
}
