#include "Enemy.h"

void Enemy::Initialize(DirectXCommon* dxCommon)
{

	dxCommon_ = dxCommon;

	//デバイスのセット
	FbxObject3d::SetDevice(dxCommon_->GetDevice());
	//グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	//敵のFbx読み込み
	enemyFbxM_.reset(FbxLoader::GetInstance()->LoadModelFromFile("boss_prot4"));
	enemyFbxO_ = std::make_unique<FbxObject3d>();
	enemyFbxO_->Initialize();
	enemyFbxO_->SetModel(enemyFbxM_.get());
}

void Enemy::Update()
{
	enemyFbxO_->Update();
}

void Enemy::Draw()
{
	enemyFbxO_->Draw(dxCommon_->GetCommandList());
}
