#include "HomingBullet.h"
#include "PlayerCharacter.h"
#include "Ease.h"
#include <random>


Model* HomingBullet::model_ = nullptr;

HomingBullet::HomingBullet() {}

HomingBullet::~HomingBullet() {}

void HomingBullet::Initialize(
    const Vector3& position,  PlayerCharacter* player, const Vector3& velocity,
    const unsigned short attribute)
{
	isDead_ = false;
	bulletO_ = Object3d::Create();
	bulletO_->SetModel(model_);

	bulletO_->worldTransform.translation_ = position;

	if (attribute == COLLISION_ATTR_PLAYERBULLETS) {
		bulletO_->SetColor({1.0f, 1.0f, 0.0f, 1.0f});
	}

	if (attribute == COLLISION_ATTR_ENEMYBULLETS) {
		bulletO_->SetColor({1.0f, 0.0f, 1.0f, 1.0f});
	}
	player_ = player;
	velocity_ = velocity;

	angle_ = (atan2(velocity.x, velocity.z) + MyMath::PI / 2);
	bulletO_->worldTransform.rotation_.y = (angle_ + MyMath::PI / 2);

	SPHERE_COLISSION_NUM = 1;
	sphere.resize(SPHERE_COLISSION_NUM);
	spherePos.resize(SPHERE_COLISSION_NUM);
	// 当たり判定の初期化
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		spherePos[i] = bulletO_->GetPosition();
		sphere[i]->SetBasisPos(&spherePos[i]);
		sphere[i]->SetRadius(1.0f);
		sphere[i]->SetAttribute(attribute);
		sphere[i]->Update();
		////test
		// coliderPosTest_[i] = Object3d::Create();
		// coliderPosTest_[i]->SetModel(hpModel_.get());
		// coliderPosTest_[i]->SetPosition(sphere[i]->center);
		// coliderPosTest_[i]->SetScale({ sphere[i]->GetRadius(),sphere[i]->GetRadius()
		// ,sphere[i]->GetRadius() }); coliderPosTest_[i]->SetRotate({ 0,0,0 });
		// coliderPosTest_[i]->Update();
	}

	// particle_ = std::make_unique<ParticleManager>();
	// particle_->SetDrawBlendMode(1);
	// particle_->Initialize();
	// particle_->LoadTexture("sprite/particle.png");
	// particle_->Update();
	
//	#pragma region//乱数生成の下準備
//	//乱数シード生成器
//	std::random_device seed_gen;
//	//メルセンヌ・ツイスターの乱数エンジン
//	std::mt19937_64 engine(seed_gen());
//	//乱数範囲の設定
//	std::uniform_real_distribution<float> accuaryDist(0.0 , 2.0);
//	std::uniform_real_distribution<float> wayDist(0 , 4.0);
//	std::uniform_real_distribution<float> timeDist(0 , 30.0);
//
//#pragma endregion
//	//精度の決定
//	homingAccuary_ = accuaryDist(engine);
//
//	//回転行列を用意
//	Matrix4 matRot = matRot.identity();
//	Matrix4 matRot2 = matRot.identity();
//
//	//回転ベクトルを用意
//	Vector3 rotation = {
//		MyMath::Dig2Rad((wayDist(engine) - 2) * 20) ,
//		MyMath::Dig2Rad((wayDist(engine) - 2) * 20) ,
//		0};
//
//	//回転ベクトルを用意
//
//	Vector3 rotation2 = {0 ,
//
//		MyMath::Dig2Rad((wayDist(engine) - 2) * 20) ,
//		0};
//	//回転行列に回転ベクトルを反映
//	matRot = MyMath::Rotate(rotation , 6);
//	matRot2 = MyMath::Rotate(rotation2 , 2);
//	//アングルと回転行列の席を求める
//	angle = MyMath::bVelocity(angle , matRot2);

	kFireTimer = 0;

	/*kStartHomingTime = timeDist(engine);*/
	livingTimer = BulletTimer::LivingTime;
	homingTimer = BulletTimer::HomingTime;
	upTimer = BulletTimer::UpTime;
	playerPos_ = player_->GetFbxObject3d()->GetPosition();
}

void HomingBullet::Update() {
	Shot();
	CheckCollision();
	bulletO_->Update();
	// particle_->Update();
	if (isDead_ == true) {
		for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {

			CollisionManager::GetInstance()->RemoveCollider(sphere[i]);
			// こいつはいらない
			/*sphere[i]->GetCollisionInfo().collider->RemoveAttribute(COLLISION_ATTR_PLAYERBULLETS);*/
		}
	}

	bulletO_->worldTransform.UpdateMatWorld();
}

void HomingBullet::Draw() { bulletO_->Draw(); }

void HomingBullet::Shot() {

	

	//ホーミング強すぎ
	livingTimer--;
	
	upTimer--;
	if ( upTimer > 0 )
	{
		bulletO_->worldTransform.translation_.y += 0.5f;
		currentPlayerPos_ = player_->GetFbxObject3d()->GetPosition();
	}
	if ( upTimer <= 0 )
	{
		upTimer = BulletTimer::UpTime;
		homingTimer--;

	}
	if (livingTimer <= 0) {
		isDead_ = true;
		livingTimer = BulletTimer::LivingTime;
	}
	if (homingTimer > 0)
	{
		playerPos_ = player_->GetFbxObject3d()->GetPosition();

		moveVelocity = currentPlayerPos_ - bulletO_->worldTransform.translation_;

		moveVelocity.nomalize();
		moveVelocity *= speed;
		bulletO_->worldTransform.translation_ += moveVelocity;
	}
	else
	{
		//moveVelocity = playerPos_ - bulletO_->worldTransform.translation_;
		//moveVelocity.nomalize();
		//moveVelocity *= speed;
		//bulletO_->worldTransform.translation_ += moveVelocity;
	}
}

void HomingBullet::CheckCollision() {
	if (hitDeley > 0) { // 毎フレームヒットを防止
		hitDeley--;
	}

	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {

		if (hitDeley <= 0 && sphere[i]->GetIsHit() == true) {
			// 弾の属性が自機の弾で当たった対象が敵だった時
			if (sphere[i]->GetAttribute() == COLLISION_ATTR_PLAYERBULLETS &&
			    sphere[i]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_ENEMYS) {
				isDead_ = true;
				livingTimer = BulletTimer::LivingTime;
				// particle_->RandParticle(sphere[i]->GetCollisionInfo().inter_);
				hitDeley = 4;
				break;
			}
			// 弾の属性が自機の弾で当たった対象が敵の弾だった時
			if (sphere[i]->GetAttribute() == COLLISION_ATTR_PLAYERBULLETS &&
			    sphere[i]->GetCollisionInfo().collider_->GetAttribute() ==
			        COLLISION_ATTR_ENEMYBULLETS) {
				isDead_ = true;
				livingTimer = BulletTimer::LivingTime;
				// particle_->RandParticle(sphere[i]->GetCollisionInfo().inter_);
				hitDeley = 4;
				break;
			}
		}
	}
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		if (hitDeley <= 0 && sphere[i]->GetIsHit() == true) {
			// 弾の属性が敵の弾で当たった対象が自機だった時
			if (sphere[i]->GetAttribute() == COLLISION_ATTR_ENEMYBULLETS &&
			    sphere[i]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_PLAYERS) {
				isDead_ = true;
				livingTimer = BulletTimer::LivingTime;
				// particle_->RandParticle(sphere[i]->GetCollisionInfo().inter_);
				hitDeley = 4;
				break;
			}
			// 弾の属性が敵の弾で当たった対象が自機の弾だった時
			if (sphere[i]->GetAttribute() == COLLISION_ATTR_ENEMYBULLETS &&
			    sphere[i]->GetCollisionInfo().collider_->GetAttribute() ==
			        COLLISION_ATTR_PLAYERBULLETS) {
				isDead_ = true;
				livingTimer = BulletTimer::LivingTime;
				// particle_->RandParticle(sphere[i]->GetCollisionInfo().inter_);
				hitDeley = 4;
				break;
			}
		}
	}
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		spherePos[i] = bulletO_->GetPosition();
		sphere[i]->Update();
	}

    /*ImGui::Begin("livingbullet");
	ImGui::SetWindowPos({ 800 , 400 });
	ImGui::SetWindowSize({ 500,100 });
	ImGui::InputFloat("livindBullet",&livingTimer);

	ImGui::End();*/}

void HomingBullet::Reset()
{
	livingTimer = BulletTimer::LivingTime;
	homingTimer = BulletTimer::HomingTime;
	isDead_ = true;
	if (isDead_ == true) {
		for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
			CollisionManager::GetInstance()->RemoveCollider(sphere[i]);
			// こいつはいらない
			/*sphere[i]->GetCollisionInfo().collider->RemoveAttribute(COLLISION_ATTR_PLAYERBULLETS);*/
		}
	}
}

void HomingBullet::Fire()
{
	;
}

void HomingBullet::Homing()
{
	;
}

//void BossBullet::Fire2()
//{
//	kFireTimer++;
//
//	if (kStartHomingTime <= kFireTimer)
//	{
//		phase = ShotPhase::Homing;
//	}
//
//
//	velocity_ = {
//
//		angle.x / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
//		1 * kBulletSpeed ,
//		angle.z / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
//	};
//
//}
//
//void BossBullet::Homing2()
//{
//	//タイマーの減少
//
//	kHomingTimer_++;
//
//
//	/*kBulletSpeed = Ease::OutQuad(change, 0, 75, kHomingTimer_);*/
//
//	kBulletSpeed = 2.0f*Ease::InQuad(change, 0,60, kHomingTimer_);
//	//タイマーが0以下の時
//	if (kHomingTimer_ < 60) {
//
//
//
//		targetPos_ = player_->GetObject3d()->GetPosition();
//
//
//
//
//		//弾のアングルと弾から敵までのベクトルの外積を出す
//		Vector3 cross = angle.cross(targetPos_.sub(bulletObj_->worldTransform.translation_));
//
//		//回転行列を用意
//		Matrix4 matRot = matRot.identity();
//
//		//回転ベクトルを用意
//		Vector3 rotation = {0 , 0 , 0};
//
//		//求まった外積の正負によって回転ベクトルを増減させる
//		if (cross.x < 0)
//		{
//			rotation.x -= MathFunc::Dig2Rad(homingRotateValue_);
//		}
//		else if (0 < cross.x)
//		{
//			rotation.x += MathFunc::Dig2Rad(homingRotateValue_);
//		}
//
//		if (cross.y < 0)
//		{
//			rotation.y -= MathFunc::Dig2Rad(homingRotateValue_);
//		}
//		else if (0 < cross.y)
//		{
//			rotation.y += MathFunc::Dig2Rad(homingRotateValue_);
//		}
//
//		if (cross.z < 0)
//		{
//			rotation.z -= MathFunc::Dig2Rad(homingRotateValue_);
//		}
//		else if (0 < cross.z)
//		{
//			rotation.z += MathFunc::Dig2Rad(homingRotateValue_);
//		}
//
//		//回転行列に回転ベクトルを反映
//		matRot = MathFunc::Rotation(rotation , 6);
//
//		//アングルと回転行列の席を求める
//		angle = MathFunc::bVelocity(angle , matRot);
//
//		////ホーミング精度に合わせてタイマーをリセット
//		//kHomingTimer_ = homingAccuary_ * 5;
//
//	}
//
//
//
//	//アングルから弾が進むベクトルを算出
//	velocity_ = {
//		angle.x / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
//		angle.y / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
//		angle.z / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
//	};
//

//}
