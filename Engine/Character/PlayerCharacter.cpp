#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
void PlayerCharacter::Initialize(
    TTEngine::DirectXCommon* dxCommon, Vector3 position,EnemyCharacter* enemy) {
	dxCommon_ = dxCommon;
	enemy_ = enemy;
	

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
	ActManager_->ActionInitialize(fbxObject_.get(),enemy_);
}

void PlayerCharacter::Update(Input* input, GamePad* gamePad) {


	moveAngle();
	if (GetisDead() == false) {
		fbxObject_->worldTransform.rotation_ = cameraAngle;
	}

	distance_ = distance;
	// 更...新!!
	ActManager_->ActionUpdate(input, gamePad);
	fbxObject_->Update();
}

void PlayerCharacter::Draw()
{
	fbxObject_->Draw(dxCommon_->GetCommandList());
}

