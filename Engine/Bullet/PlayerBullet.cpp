#include "PlayerBullet.h"
#include "Enemy.h"
#include "ImguiManager.h"
void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{

	bulletO_ = Object3d::Create();
	bulletO_->SetModel(model);

	bulletO_->worldTransform.translation_ = position;

	bulletO_->SetColor({ 1.0f,0.0f,0.0f,1.0f });

	velocity_ = velocity;


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
}

void PlayerBullet::Update()
{
	Shot();
	CheckCollision();
	bulletO_->Update();
	

	//for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
	//	spherePos[i] = bulletO_->GetPosition();
	//	sphere[i]->Update();
	//	/*if (sphere[i]->GetIsHit() == true && player_->GetIsAtkCollide() == true){


	//	 }*/

	//}
	/*if (enemy_->GetIsHit() == true)
	{
		
	}*/

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
	//	//各positionを取得
	//	bulletpos = bulletO_->worldTransform.translation_;
	//	enemypos = enemy_->GetPosition();

	//	//敵と弾の差分ベクトル
	//	distance = enemypos - bulletpos;
	//	distance.length();
	//	//正規化
	//	distance.nomalize();
	//	
	//	//速さの決定
	//	distance*= speed;

	//	bulletO_->worldTransform.translation_.x += distance.x;
	//	bulletO_->worldTransform.translation_.z += distance.z;

	//}
	//

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
			if (sphere[i]->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ENEMYS)
			{
				SetisDead(true);
				livingTimer = 540.0f;
				hitDeley = 4;
				break;
			}
		}



	}
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		spherePos[i] = bulletO_->GetPosition();
		sphere[i]->Update();
	}

	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		if (isDead_ == true)
		{
			CollisionManager::GetInstance()->RemoveCollider(sphere[i]);
			//こいつはいらない
			/*sphere[i]->GetCollisionInfo().collider->RemoveAttribute(COLLISION_ATTR_PLAYERBULLETS);*/
		}
	}
	

	/*ImGui::Begin("livingbullet");
	ImGui::SetWindowPos({ 800 , 400 });
	ImGui::SetWindowSize({ 500,100 });
	ImGui::InputFloat("livindBullet",&livingTimer);

	ImGui::End();*/

}
