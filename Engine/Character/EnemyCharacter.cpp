#include "EnemyCharacter.h"



void EnemyCharacter::Initialize(
    TTEngine::DirectXCommon* dxCommon, Vector3 position, PlayerCharacter* player) {
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
}

void EnemyCharacter::Update() {

	moveAngle();
	if (GetisDead() == false) {
		fbxObject_->worldTransform.rotation_ = cameraAngle;
	}

	distance_ = distance;
	ActManager_->ActionUpdate();
	fbxObject_->Update();
}

void EnemyCharacter::Draw()
{
	fbxObject_->Draw(dxCommon_->GetCommandList());
}
