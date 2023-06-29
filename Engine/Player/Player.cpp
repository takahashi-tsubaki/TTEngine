#include "Player.h"

void Player::Initialize(DirectXCommon* dxCommon, Input* input)
{
	dxCommon_ = dxCommon;
	input_ = input;

	//�f�o�C�X�̃Z�b�g
	FbxObject3d::SetDevice(dxCommon_->GetDevice());
	//�O���t�B�b�N�X�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();

	//�G��Fbx�ǂݍ���
	playerFbxM_.reset(FbxLoader::GetInstance()->LoadModelFromFile("boss_prot4"));
	playerFbxO_ = std::make_unique<FbxObject3d>();
	playerFbxO_->Initialize();
	playerFbxO_->SetModel(playerFbxM_.get());
}

void Player::Update()
{
	Move();
	playerFbxO_->Update();
}

void Player::Draw()
{
	playerFbxO_->Draw(dxCommon_->GetCommandList());
}

void Player::Move()
{
	if (input_->PushKey(DIK_A))
	{
		playerFbxO_->worldTransform.translation_.x += 0.1f;
	}
	if (input_->PushKey(DIK_D))
	{
		playerFbxO_->worldTransform.translation_.x -= 0.1f;
	}
	if (input_->PushKey(DIK_W))
	{
		playerFbxO_->worldTransform.translation_.y += 0.1f;
	}
	if (input_->PushKey(DIK_S))
	{
		playerFbxO_->worldTransform.translation_.y -= 0.1f;
	}
}
