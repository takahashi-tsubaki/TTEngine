#include "PlayScene.h"
#include "ImguiManager.h"
PlayScene::PlayScene(SceneManager* controller, SceneObjects* sceneObj)
{
	controller_ = controller;
	sceneObj_ = sceneObj;
	//sceneObj_->Initialize(controller_);
}

PlayScene::~PlayScene()
{
	sceneObj_->Reset();
	//sceneObj_->Delete();
	//delete player_;
	//delete enemy_;
}

void PlayScene::Initialize()
{
	sceneObj_->player_->Initialize(controller_->dxCommon_,sceneObj_->enemy_);
	sceneObj_->enemy_->Initialize(controller_->dxCommon_, sceneObj_->player_);

	Sprite::LoadTexture(1, L"Resources/kuribo-.jpg");
	Sprite::LoadTexture(2, L"Resources/mario.jpg");
	//HpSprite
	Sprite::LoadTexture(3, L"Resources/sprite/hp.png");

	sprite_ = Sprite::Create(1, { WinApp::window_width,WinApp::window_height });
	enemyHpSprite_ = Sprite::Create(3, { 200,10 });
	enemyHpSprite_->Initialize();

	playerHpSprite_ = Sprite::Create(3,{100,600});
	playerHpSprite_->Initialize();

	player_ = sceneObj_->player_;
	enemy_ = sceneObj_->enemy_;

	controller_->camera_->SetFollowerPos(player_->GetObject3d()->GetWorldTransformPtr());

	controller_->camera_->SetTargetPos(enemy_->GetObject3d()->GetWorldTransformPtr());
}

void PlayScene::Update(Input* input, GamePad* gamePad)
{
	gamePad->Update();
	//�V�[���`�F���W
	if (input->TriggerKey(DIK_RETURN) || gamePad->ButtonTrigger(X))
	{
		player_->Reset();
		enemy_->Reset();
		controller_->ChangeSceneNum(S_TITLE);
	}

	//�X�v���C�g�̑傫����̗͂ɐݒ�
	enemyHpSprite_->SetSize({ enemy_->GetHp() * 32.0f, 32.0f });
	playerHpSprite_->SetSize({ player_->GetHp() * 32.0f, 32.0f});


	Vector3 nowEye = controller_->camera_->GetEye();


	//fbxObject->Update();
	assert(input);



	controller_->camera_->Update();

	/*ImGui::Begin("cameraPos");
	ImGui::SetWindowPos({ 200 , 200 });
	ImGui::SetWindowSize({ 500,100 });
	ImGui::InputFloat3("eye", &camera_->eye_.x);
	ImGui::InputFloat3("target", &camera_->target_.x);
	ImGui::End();*/

	sceneObj_->skydomeO_->Update();

	player_->Update(input,gamePad);
	enemy_->Update();

	//���Z�b�g����
	if (input->TriggerKey(DIK_R))
	{
		player_->Reset();
		enemy_->Reset();
		//player_->Initialize(controller_->dxCommon_,enemy_);
		//enemy_->Initialize(controller_->dxCommon_,player_);
	}


	ImGui::Begin("Camera");

	ImGui::SliderFloat("eye:x", &nowEye.x, -400.0f, 400.0f);
	ImGui::SliderFloat("eye:xz", &nowEye.z, -400.0f, 400.0f);

	ImGui::End();
}

void PlayScene::Draw()
{
#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(controller_->dxCommon_->GetCommandList());
	// �w�i�X�v���C�g�`��


	/// <summary>
	/// �����ɔw�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion

#pragma region 3D�I�u�W�F�N�g�`��
	//// 3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw(controller_->dxCommon_->GetCommandList());

	//// 3D�I�u�W�F�N�g�̕`��

	/*fbxObject->Draw(dxCommon_->GetCommandList());*/

	sceneObj_->skydomeO_->Draw();

	player_->Draw();
	enemy_->Draw();

	///// <summary>
	///// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	///// </summary>

	//// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
#pragma endregion

#pragma region �p�[�e�B�N���`��

	//// �p�[�e�B�N���`��O����
	Particle::PreDraw(controller_->dxCommon_->GetCommandList());

	//// 3D�I�u�W�F�N�g�̕`��

	///// <summary>
	///// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	///// </summary>

	//// �p�[�e�B�N���`��㏈��
	Particle::PostDraw();

#pragma endregion

	//#pragma region �؂�|���S���`��
	//	postEffect->PreDrawScene(dxCommon_->GetCommandList());
	//
	//	//// �؂�|���S���̕`��
	//	postEffect->Draw(dxCommon_->GetCommandList());
	//	///// <summary>
	//	///// �����ɂ؂�|���S���̕`�揈����ǉ��ł���
	//	///// </summary>
	//
	//	
	//
	//	postEffect->PostDrawScene(dxCommon_->GetCommandList());
	//
	//
	//
	//#pragma endregion
#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(controller_->dxCommon_->GetCommandList());

	//sprite_->Draw();
	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>
	playerHpSprite_->Draw();
	enemyHpSprite_->Draw();
	//
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

#pragma endregion
}
