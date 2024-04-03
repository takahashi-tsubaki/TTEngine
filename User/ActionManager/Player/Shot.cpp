#include "Shot.h"
#include "Character.h"
#include "SceneObjects.h"
Shot::Shot(PlayerActionManager* ActionManager) {

	ActionManager_ = ActionManager;
	isNowShot_ = true;
	//このやり方だと重くなる
	//StaticInitialize();
	//bullets_ = bullet;
}

Shot::~Shot()
{
	Reset();
	
}

void Shot::StaticInitialize() { ; }

void Shot::Initialize(FbxObject3d* object, EnemyCharacter* enemy, SceneObjects* sceneObj)
{
	object_ = object;
	enemy_ = enemy;

	sceneObj_ = sceneObj;
	bulletSizeUpTimer_ = 10.0f;
	sceneObj_->effectO_->SetPosition(
	    {object_->GetWorldTransform().translation_.x,
	     object_->GetWorldTransform().translation_.y,
	     object_->GetWorldTransform().translation_.z});
	sceneObj_->effectO_->SetScale({4, 4, 4});
}

void Shot::Update(Input* input, GamePad* gamePad)
{

	speed_ = 1.0f;
	if ( shotType_ == ShotType::RAPID )
	{
		speed_ = 1.5f;
	}
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
			isEffect = true;
		}
		if ( bulletSizeUpTimer_ <= 0.0f )
		{
			if ( MaxBulletSize_ < 20 )
			{
				MaxBulletSize_++;
			}
			//弾の撃つ数を上げるために必要なフレーム数
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
			//状態 = 射撃;
		}
	
		isSubColor = true;


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
				newBullet->Initialize(object_->worldTransform.translation_, Distance_, COLLISION_ATTR_PLAYERBULLETS);

				if (shotType_ == ShotType::ONE) {
					bulletCoolTimer_ = 15.0f;
				} else if (shotType_ == ShotType::RAPID) {
					bulletCoolTimer_ = 5.0f;
				}

				newBullet->SetEnemy(enemy_);

				BulletManager::GetInstance()->AddBullet(std::move(newBullet));

				pSourceVoice[0] = sceneObj_->audio_->PlayWave("bullet.wav");
				pSourceVoice[0]->SetVolume(0.2f);

			}

			
		}
		if (bulletSize_ >= MaxBulletSize_) {
			koutyokuTimer_--;
		}
		if (koutyokuTimer_ < 0) {
			bulletSize_ = 0;
			MaxBulletSize_ = 0;

			shotType_ = ShotType::Nasi;
			bulletCoolTimer_ = 0.0f;
			koutyokuTimer_ = 120.0f;
			isNowShot_ = false;
			isNowStandBy_ = true;
			isShot = false; // こいつのリセット処理は一番最後
		}
	}
	if ( isEffect == true )
	{
		sceneObj_->effectO_->rotation_.x += 3.0f;
		sceneObj_->effectO_->rotation_.y += 3.0f;
		//sceneObj_->effectO_->rotation_.z += 3.0f;
		sceneObj_->effectO_->SetRotation(sceneObj_->effectO_->rotation_);

	}
	if (isSubColor == true)
	{
		sceneObj_->effectO_->color_.w -= 0.01f;
		sceneObj_->effectO_->scale_.x += 0.1f;
		sceneObj_->effectO_->scale_.y += 0.1f;
	}


	if ( sceneObj_->effectO_->color_.w < 0 )
	{
		sceneObj_->effectO_->scale_.x = 4.0f;
		sceneObj_->effectO_->scale_.y = 4.0f;
		isEffect = false;
		isSubColor = false;

		sceneObj_->effectO_->color_.w = 1.0f;

	}
	sceneObj_->effectO_->SetColor(sceneObj_->effectO_->color_);

	sceneObj_->effectO_->SetScale(sceneObj_->effectO_->scale_);
	sceneObj_->effectO_->Update();

}

void Shot::Draw()
{
	if ( isEffect == true )
	{
		if (sceneObj_->effectO_->color_.w > 0) {
			sceneObj_->effectO_->Draw();
		}
	}
	
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
