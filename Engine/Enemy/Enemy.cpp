#include "Enemy.h"

void Enemy::Initialize(DirectXCommon* dxCommon)
{

	dxCommon_ = dxCommon;

	//デバイスのセット
	FbxObject3d::SetDevice(dxCommon_->GetDevice());
	//グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	////敵のFbx読み込み
	//enemyFbxM_.reset(FbxLoader::GetInstance()->LoadModelFromFile("boss_prot4"));
	//enemyFbxO_ = std::make_unique<FbxObject3d>();
	//enemyFbxO_->Initialize();
	//enemyFbxO_->SetModel(enemyFbxM_.get());
	//enemyFbxO_->PlayAnimetion(5);

	//enemyFbxO_->SetPosition(enemy_.translation_);

	enemyO_ = Object3d::Create();

	enemyM_ = Model::CreateFromOBJ("cube");

	enemyO_->SetModel(enemyM_);

	enemy_.translation_ = { 0,0,0 };
	enemyO_->SetPosition(enemy_.translation_);

}

void Enemy::Update()
{
	enemyO_->Update();
}

void Enemy::Draw()
{
	enemyO_->Draw();
}
