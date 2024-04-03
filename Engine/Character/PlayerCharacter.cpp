#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "SceneObjects.h"

#include "Shot.h"

void PlayerCharacter::Initialize(
    TTEngine::DirectXCommon* dxCommon, Vector3 position, EnemyCharacter* enemy,
    SceneObjects* sceneObj) {
	dxCommon_ = dxCommon;
	enemy_ = enemy;

	sceneObj_ = sceneObj;

	// fbxの大きさ
	fbxScale_ = 0.01f;
	// デバイスのセット
	FbxObject3d::SetDevice(dxCommon_->GetDevice());
	// グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	fbxModel_.reset(FbxLoader::GetInstance()->LoadModelFromFile("player2"));

	//Fbxオブジェクトの作成
	fbxObject_ = FbxObject3d::Create();
	fbxObject_->SetModel(fbxModel_.get());
	fbxObject_->SetScale({fbxScale_, fbxScale_, fbxScale_});
	fbxObject_->SetPosition(position);
	// fbxPlayerO_->SetIsBonesWorldMatCalc(true); // ボーンワールド行列計算あり
	fbxObject_->Update();

	SPHERE_COLISSION_NUM = 1;
	sphere.resize(SPHERE_COLISSION_NUM);
	spherePos.resize(SPHERE_COLISSION_NUM);


	// 当たり判定の作成
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		spherePos[i] = fbxObject_->GetPosition();
		sphere[i]->SetBasisPos(&spherePos[i]);
		sphere[i]->SetRadius(1.0f);

		sphere[i]->SetAttribute(COLLISION_ATTR_PLAYERS);
		sphere[i]->Update();
	}

	// 行動マネージャー
	ActManager_ = std::make_unique<PlayerActionManager>();
	// pActManager_->ColliderInitialize(&sphere, SPHERE_COLISSION_NUM);
	ActManager_->ActionInitialize(fbxObject_.get(), enemy_, sceneObj_);

	Hp_ = 10;

	 particle_ = std::make_unique<ParticleManager>();
	 particle_->SetDrawBlendMode(1);
	 particle_->Initialize();
	 particle_->LoadTexture("sprite/particle.png");
	 particle_->Update();

	//PlayerBullet::StaticInitialize();
}

void PlayerCharacter::Update(Input* input, GamePad* gamePad)
{

	//// デスフラグが立った球を削除
	//bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->GetIsDead(); });

	//bullets_ = ActManager_->GetBullets();
	moveAngle();
	if (GetisDead() == false) {
		fbxObject_->worldTransform.rotation_ = cameraAngle;
		particle_->Update();
	}
	if (Hp_ <= 0)
	{
		isDead_ = true;
	}
	else
	{
		distance_ = distance;

		// 更...新!!
		fbxObject_->Update();

		ActManager_->ActionUpdate(input, gamePad);
		particle_->Update();
	}

	CheckHitCollision();
}

void PlayerCharacter::Draw()
{
	fbxObject_->Draw(dxCommon_->GetCommandList());
	ActManager_->ActionDraw();
}

void PlayerCharacter::CheckHitCollision()
{
#pragma region オブジェクト同士の押し出し処理
	class PlayerQueryCallBack : public QueryCallback
	{
	public:
		PlayerQueryCallBack(Sphere* sphere) : sphere(sphere){};

		bool OnQueryHit(const QueryHit& info) {
			rejectDir = info.reject;
			rejectDir.nomalize();

			// 上方向と排斥方向の角度差のコサイン値
			float cos = rejectDir.dot(up);

			//
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
		PlayerQueryCallBack callback(sphere[i]);

		// 球と地形の交差を全探索する
		CollisionManager::GetInstance()->QuerySphere(*sphere[i], &callback);

		if (sphere[i]->GetIsHit() == true) {
			// 当たったものの属性が敵だった時
			if (sphere[i]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_ENEMYS) {
				fbxObject_->worldTransform.translation_.x += callback.move.x;
				fbxObject_->worldTransform.translation_.y += callback.move.y;
				fbxObject_->worldTransform.translation_.z += callback.move.z;
				break;
			}
		}
	}
#pragma endregion

	oldPos = fbxObject_->worldTransform.translation_;

	if (hitDeley > 0) { // 毎フレームヒットを防止
		// playerO_->SetColor({ 0,0,1,1 });
		hitDeley--;
	} else {
		// playerO_->SetColor({ 1,1,1,1 });
	}

	// 当たり判定の処理
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		if (hitDeley <= 0 && sphere[i]->GetIsHit() == true) {
			// 当たったものの属性が敵の弾だった時
			if (sphere[i]->GetCollisionInfo().collider_->GetAttribute() ==
			    COLLISION_ATTR_ENEMYBULLETS) {

				isDamage = false;
				hitCountTime = 60;
				if (isDamage == false) {
					Hp_ -= 1;
					hitDeley = 3;
					// particle_->SetColor({0, 0, 1, 1});
					 particle_->RandParticle(sphere[i]->GetCollisionInfo().inter_);
					//	被弾時のアニメーションの再生
					fbxObject_->SetCurrentTimer(0);
					//fbxObject_->PlayAnimation(FBXAnimetion::HIT, false);
					isDamage = true;
				}

				break;
			}
		}
	}
	if (GetisDead() == true) {
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
	if (isDamage == true) {
		hitCountTime--;
	}

	if (hitCountTime <= 0) {
		isDamage = false;
	}
}

void PlayerCharacter::Reset() {

	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{

		CollisionManager::GetInstance()->RemoveCollider(sphere[i]);
		// こいつはいらない
		/*sphere[i]->GetCollisionInfo().collider->RemoveAttribute(COLLISION_ATTR_PLAYERBULLETS);*/
	}
	ResetAttribute();

	Hp_ = 10;
	isDamage = false;

	hitCountTime = 60;
	hitDeley = 0;
}

void PlayerCharacter::ResetAttribute()
{
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		spherePos[i] = fbxObject_->GetPosition();
		sphere[i]->SetBasisPos(&spherePos[i]);
		sphere[i]->SetRadius(1.0f);
		sphere[i]->SetAttribute(COLLISION_ATTR_PLAYERS);
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

void PlayerCharacter::RemoveAttribute() {
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {

		CollisionManager::GetInstance()->RemoveCollider(sphere[i]);
		// こいつはいらない
		/*sphere[i]->GetCollisionInfo().collider->RemoveAttribute(COLLISION_ATTR_PLAYERBULLETS);*/
	}
}