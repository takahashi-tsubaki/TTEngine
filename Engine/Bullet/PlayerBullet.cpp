#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{

	bulletO_ = Object3d::Create();
	bulletO_->SetModel(model);

	bulletO_->worldTransform.translation_ = position;

	bulletO_->SetColor({ 1.0f,0.0f,0.0f,1.0f });

	velocity_ = velocity;
}

void PlayerBullet::Update()
{
	Shot();
	bulletO_->Update();

	bulletO_->worldTransform.UpdateMatWorld();
}

void PlayerBullet::Draw()
{
	bulletO_->Draw();
}

void PlayerBullet::Shot()
{
	//Vector3 bulletpos;
	//Vector3 enemypos;
	//Vector3 distance;

	//const float speed = 0.5f;

	livingTimer--;
	if (livingTimer <= 0)
	{
		SetisDead(true);
		livingTimer = 540.0f;
		
	}
	//else
	//{
	//	//Šeposition‚ðŽæ“¾
	//	bulletpos = bulletO_->worldTransform.translation_;
	//	enemypos = enemy_->GetPosition();

	//	//“G‚Æ’e‚Ì·•ªƒxƒNƒgƒ‹
	//	distance = enemypos - bulletpos;
	//	distance.length();
	//	//³‹K‰»
	//	distance.nomalize();
	//	
	//	//‘¬‚³‚ÌŒˆ’è
	//	distance*= speed;

	//	bulletO_->worldTransform.translation_.x += distance.x;
	//	bulletO_->worldTransform.translation_.z += distance.z;

	//}
	//

	bulletO_->worldTransform.translation_ += velocity_;
}
