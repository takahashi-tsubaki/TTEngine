#include "EnemyHomingShot.h"

EnemyHomingShot::EnemyHomingShot(EnemyActionManager* ActionManager, int actionNum) {
	ActionManager_ = ActionManager;
	actionNum_ = actionNum;
}

EnemyHomingShot::~EnemyHomingShot() { Reset(); }

void EnemyHomingShot::Initialize(FbxObject3d* object, PlayerCharacter* player) {
	object_ = object;
	player_ = player;

	//koutyokuTimer_ = 300.0f;
	MaxBulletSize_ = 2;
	isShot = true;

}

void EnemyHomingShot::Update() {
	speed_ = 0.75f;
	enemyPos = object_->GetPosition();
	playerPos = player_->GetFbxObject3d()->GetPosition();
	Distance_ = playerPos - enemyPos ;

	Distance_.nomalize();

	Distance_ *= speed_;

	if ( isShot == true )
	{
		bulletCoolTimer_--;
		// 射撃アニメーションの再生
		object_->PlayAnimation(FBXAnimetion::ShoT,false);
		if ( bulletSize_ < MaxBulletSize_ )
		{
			if ( bulletCoolTimer_ <= 0 )
			{
				bulletSize_++;
				std::unique_ptr<HomingBullet> newBullet = std::make_unique<HomingBullet>();
				newBullet->Initialize(
					object_->worldTransform.translation_,player_,Distance_,COLLISION_ATTR_ENEMYBULLETS);

				
				bulletCoolTimer_ = 30.0f;

				newBullet->SetPlayer(player_);

				BulletManager::GetInstance()->AddHomingBullet(std::move(newBullet));
			}
		}
		if ( bulletSize_ >= MaxBulletSize_ )
		{
			koutyokuTimer_--;
		}
		if ( koutyokuTimer_ < 0 )
		{
			bulletSize_ = 0;
			MaxBulletSize_ = 0;

			bulletCoolTimer_ = 0.0f;
			isNowShot_ = false;

			isShot = false; // こいつのリセット処理は一番最後

		}
	}


}

void EnemyHomingShot::Draw() {}

void EnemyHomingShot::Reset() {
	speed_ = 0.5f;
	scale_ = 0.01f;
	bulletSizeUpTimer_ = 10.0f;
	MaxBulletSize_ = 0;
	bulletSize_ = 0;

	bulletCoolTimer_ = 0.0f;
	isShot = false;
}

