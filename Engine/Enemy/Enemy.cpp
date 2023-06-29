#include "Enemy.h"

void Enemy::Initialize(DirectXCommon* dxCommon)
{

	dxCommon_ = dxCommon;

	//�f�o�C�X�̃Z�b�g
	FbxObject3d::SetDevice(dxCommon_->GetDevice());
	//�O���t�B�b�N�X�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();

	//�G��Fbx�ǂݍ���
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
