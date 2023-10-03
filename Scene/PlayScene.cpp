#include "PlayScene.h"
#include "ImguiManager.h"
PlayScene::PlayScene(SceneManager* controller, SceneObjects* sceneObj)
{
	controller_ = controller;
	sceneObj_ = sceneObj;
	//sceneObj_->player_->Initialize(controller_->dxCommon_, sceneObj_->enemy_);
	//sceneObj_->enemy_->Initialize(controller_->dxCommon_, sceneObj_->player_);
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


	//Sprite::LoadTexture(1, L"Resources/kuribo-.jpg");
	//Sprite::LoadTexture(2, L"Resources/mario.jpg");
	////HpSprite


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

	assert(input);
	gamePad->Update();
	if (isTransition == false)
	{
		//�V�[���`�F���W
		if (input->TriggerKey(DIK_RETURN) || gamePad->ButtonTrigger(X))
		{
			player_->Reset();
			enemy_->Reset();
			controller_->ChangeSceneNum(S_TITLE);
		}

		if (input->TriggerKey(DIK_TAB) || gamePad->ButtonTrigger(START))
		{
			sceneObj_->player_ =player_;
			sceneObj_->enemy_ = enemy_;
			controller_->PushScene(S_PAUSE);
		}

		if (input->TriggerKey(DIK_LSHIFT) || gamePad->ButtonTrigger(BACK))
		{
			if (enemy_->GetDebugMode() == false)
			{
				enemy_->SetDebugMode(true);
			}
			else
			{
				enemy_->SetDebugMode(false);
			}
		}
		
		player_->Update(input, gamePad);
		enemy_->Update();
	}
	player_ = sceneObj_->player_;
	

	if (isTransition == true)
	{
		SceneTransition();
	}
	

	//�X�v���C�g�̑傫����̗͂ɐݒ�
	enemyHpSprite_->SetSize({ enemy_->GetHp() * 32.0f, 32.0f });
	playerHpSprite_->SetSize({ player_->GetHp() * 32.0f, 32.0f});


	//fbxObject->Update();

	/*ImGui::Begin("cameraPos");
	//ImGui::SetWindowPos({ 200 , 200 });
	ImGui::SetWindowSize({ 500,100 });
	ImGui::InputFloat3("eye", &camera_->eye_.x);
	ImGui::InputFloat3("target", &camera_->target_.x);
	ImGui::End();*/


	sceneObj_->skydomeO_->SetPosition({ sceneObj_->skydomeO_->GetPosition().x + player_->GetPosition().x,sceneObj_->skydomeO_->GetPosition().y,sceneObj_->skydomeO_->GetPosition().z });
	sceneObj_->skydomeO_->Update();

	if (sceneObj_->transitionO_->worldTransform.scale_.x <= 0 || sceneObj_->transitionO_->worldTransform.scale_.z <= 0)
	{
		isTransition = false;
	}
	if (isTransition == false)
	{
		sceneObj_->transitionO_->worldTransform.scale_ = { 1,1,1 };
	}

	//���Z�b�g����
	if (input->TriggerKey(DIK_R))
	{
		player_->Reset();
		enemy_->Reset();

	}

	if (player_->GetHp() <= 0)
	{
		controller_->ChangeSceneNum(S_OVER);
		player_->Reset();
		enemy_->Reset();
	}

	else if (enemy_->GetHp() <= 0)
	{
		controller_->ChangeSceneNum(S_CLEAR);
		player_->Reset();
		enemy_->Reset();
	}


	controller_->camera_->Update();
}

void PlayScene::Draw()
{
#pragma region 3D�I�u�W�F�N�g�`��
	//// 3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw(controller_->dxCommon_->GetCommandList());

	//// 3D�I�u�W�F�N�g�̕`��

	/*fbxObject->Draw(dxCommon_->GetCommandList());*/

	sceneObj_->skydomeO_->Draw();


	enemy_->Draw();
	player_->Draw();

	///// <summary>
	///// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	///// </summary>

	//// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
#pragma endregion
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

	if (isTransition == true)
	{
		sceneObj_->transitionO_->Draw();
	}

	player_->GetParticle()->Draw(controller_->dxCommon_->GetCommandList());
	enemy_->GetParticle()->Draw(controller_->dxCommon_->GetCommandList());

	///// <summary>
	///// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	///// </summary>

	//// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
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

void PlayScene::SceneTransition()
{
	sceneObj_->transitionO_->worldTransform.scale_ -= scale;
	sceneObj_->transitionO_->Update();
}

