#include "EnemyCharacter.h"
#include "Affin.h"


void EnemyCharacter::Initialize(TTEngine::DirectXCommon* dxCommon, Vector3 position, PlayerCharacter* player)
{
	dxCommon_ = dxCommon;
	player_ = player;
	// fbxの大きさ
	fbxScale_ = 0.01f;
	// デバイスのセット
	FbxObject3d::SetDevice(dxCommon_->GetDevice());
	// グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	fbxModel_.reset(FbxLoader::GetInstance()->LoadModelFromFile("enemy2"));

	//Fbxオブジェクトの初期化
	fbxObject_ = FbxObject3d::Create();
	fbxObject_->SetModel(fbxModel_.get());
	fbxObject_->SetScale({fbxScale_, fbxScale_, fbxScale_});
	fbxObject_->SetPosition(position);
	// fbxPlayerO_->SetIsBonesWorldMatCalc(true); // ボーンワールド行列計算あり
	fbxObject_->Update();

	SPHERE_COLISSION_NUM = 1;
	sphere.resize(SPHERE_COLISSION_NUM);
	spherePos.resize(SPHERE_COLISSION_NUM);

	//当たり判定の作成
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		spherePos[i] = fbxObject_->GetPosition();
		sphere[i]->SetBasisPos(&spherePos[i]);
		sphere[i]->SetRadius(1.0f);

		sphere[i]->SetAttribute(COLLISION_ATTR_ENEMYS);
		sphere[i]->Update();
	}



	// 行動マネージャー
	ActManager_ = std::make_unique<EnemyActionManager>();
	// pActManager_->ColliderInitialize(&sphere, SPHERE_COLISSION_NUM);
	ActManager_->ActionInitialize(fbxObject_.get(), player_);

	blowAwayCount = 0;
	Hp_ = 30;

		 particle_ = std::make_unique<ParticleManager>();
	particle_->SetDrawBlendMode(1);
	particle_->Initialize();
	particle_->LoadTexture("sprite/particle.png");
	particle_->Update();
}

void EnemyCharacter::Update()
{

	moveAngle();
	CheckHitCollision();
	if (GetisDead() == false) {
		fbxObject_->worldTransform.rotation_ = cameraAngle;
	}
	if (Hp_ <= 0) {
		isDead_ = true;
		IsDeadAnime();
	}
	else
	{

		transNormal = {0, 0.5f, 5};

		transNormal = MyMath::bVelocity(transNormal, fbxObject_->worldTransform.matWorld_);
		distance_ = distance;

		ActManager_->ActionUpdate();
		fbxObject_->Update();
		particle_->Update();
	}


}

void EnemyCharacter::Draw()
{

	fbxObject_->Draw(dxCommon_->GetCommandList());

}

void EnemyCharacter::Damage()
{
	damageSize_ += damage;
	Hp_ -= damage;
	decreaseHpCount--;
	if (decreaseHpCount <= 0) {
		if (damageSize_ > 0) {
			damageSize_ -= 1;
		}
	}
	if (damageSize_ <= 0) {
		damageSize_ = 0;
	}

	SetHp(Hp_);
}

void EnemyCharacter::Reset()
{
	Hp_ = 30;
	if (isDead_ == true) {
		ResetAttribute();
		isDead_ = false;
	}
	blowAwayCount = 0;
	isDamage = false;
	isHit_ = false;
}

void EnemyCharacter::ResetAttribute()
{
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		spherePos[i] = fbxObject_->GetPosition();
		sphere[i]->SetBasisPos(&spherePos[i]);
		sphere[i]->SetRadius(1.0f);
		sphere[i]->SetAttribute(COLLISION_ATTR_ENEMYS);
		sphere[i]->Update();
		////test
		// coliderPosTest_[i] = Object3d::Create();
		// coliderPosTest_[i]->SetModel(hpModel_.get());
		// coliderPosTest_[i]->SetPosition(sphere[i]->center);
		// coliderPosTest_[i]->SetScale({ sphere[i]->GetRadius(),sphere[i]->GetRadius()
		// ,sphere[i]->GetRadius() }); coliderPosTest_[i]->SetRotate({ 0,0,0 });
		// coliderPosTest_[i]->Update();
	}
}

void EnemyCharacter::CheckHitCollision()
{
#pragma region オブジェクト同士の押し出し処理
	class EnemyQueryCallBack : public QueryCallback
	{
	public:
		EnemyQueryCallBack(Sphere* sphere) : sphere(sphere){};

		bool OnQueryHit(const QueryHit& info) {
			rejectDir = info.reject;
			rejectDir.nomalize();

			// 上方向と排斥方向の角度差のコサイン値
			float cos = rejectDir.dot(up);

			// 地面判定しきい値角度
			const float threshold = cosf(XMConvertToRadians(30.0f));
			// 角度差によって天井または地面と判定される場合を除いて
			if (-threshold < cos && cos < threshold) {
				// 押し出す
				sphere->center += info.reject;
				move += info.reject;
			}
			return true;
		}
		// void SphereQuery();

		// ワールドの上方向
		const Vector3 up = {0, 1, 0};
		// 排斥方向
		Vector3 rejectDir;
		// クエリーに使用する球
		Sphere* sphere = nullptr;
		// 排斥による移動量
		Vector3 move = {};
	};

	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		EnemyQueryCallBack callback(sphere[i]);

		// 球と地形の交差を全探索する
		CollisionManager::GetInstance()->QuerySphere(*sphere[i], &callback);
		if (sphere[i]->GetIsHit() == true) {
			if (sphere[i]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_PLAYERS) {
				fbxObject_->worldTransform.translation_.x += callback.move.x;
				fbxObject_->worldTransform.translation_.y += callback.move.y;
				fbxObject_->worldTransform.translation_.z += callback.move.z;
			}
		}

		// sphere[i]->Update();
	}

#pragma endregion

	if (hitDeley <= 0) {
		isDamage = false;
	}

	if (hitDeley > 0) { // 毎フレームヒットを防止
		// enemyO_->SetColor({ 0,0,1,1 });
		hitDeley--;
	} else {

		// enemyO_->SetColor({ 1,1,1,1 });
	}

	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		if (hitDeley <= 0 && sphere[i]->GetIsHit() == true) {
			if (sphere[i]->GetCollisionInfo().collider_->GetAttribute() ==
			    COLLISION_ATTR_PLAYERBULLETS) {
				Damage();
				/*particle_->Charge(
				    60, sphere[i]->GetCollisionInfo().inter_, sphere[i]->GetCollisionInfo().inter_,1.0f);*/
				particle_->RandParticle(sphere[i]->GetCollisionInfo().inter_);
				hitDeley = 5;

				SetIsHit(true);

				break;
			}
		}
	}

	if (GetHp() <= 0) {
		for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
			CollisionManager::GetInstance()->RemoveCollider(sphere[i]);
			// こいつはいらない
			/*sphere[i]->GetCollisionInfo().collider->RemoveAttribute(COLLISION_ATTR_PLAYERBULLETS);*/
		}
	}
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {

		spherePos[i] = fbxObject_->GetPosition();
		sphere[i]->Update();
	}
}

void EnemyCharacter::IsDeadAnime() {
	blowAwayCount++;

	// 水平投射をしながら自機を下に落下させる
	Affin::HorizontalProjection(fbxObject_->worldTransform, transNormal, 1.0f, blowAwayCount);

	fbxObject_->SetPosition(fbxObject_->worldTransform.translation_);
}
