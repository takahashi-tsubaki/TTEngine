#include "TitleScene.h"
#include "levelLoader.h"
#include <cassert>
#include <sstream>
#include <iomanip>
TitleScene::TitleScene(SceneManager* controller, SceneObjects* sceneObj)
{
	controller_ = controller;
	sceneObj_ = sceneObj;

	targetPos.translation_ = { 0,0,0 };
	followPos.translation_ = { 0,0,-50 };
}

TitleScene::~TitleScene()
{


	sceneObj_->Reset();
	isTransition = false;
	//delete sceneObj_->skydomeO_;
	/*sceneObj_->Delete();*/
}

void TitleScene::Initialize()
{

	/*sceneObj_->transitionO_->worldTransform.scale_ = scale;*/

	controller_->camera_->SetFollowerPos(&followPos);

	controller_->camera_->SetTargetPos(&targetPos);
	Sprite::LoadTexture(6, L"Resources/sprite/title.png");
	sprite_ = Sprite::Create(6, { 100,100 });
}

void TitleScene::Update(Input* input, GamePad* gamePad)
{
	sceneObj_->skydomeO_->Update();
	controller_->camera_->Update();
	
	gamePad->Update();

	//�{�^������������V�[���J�ڂ��s��
	if (input->TriggerKey(DIK_SPACE) || gamePad->ButtonTrigger(X))
	{
		isTransition = true;
	}

	//�V�[���J�ڂ̃I�u�W�F�N�g�̃X�P�[�������ȏ�ɓ��B�����Ƃ�
	if (sceneObj_->transitionO_->worldTransform.scale_.x>=60 && sceneObj_->transitionO_->worldTransform.scale_.z>=60)
	{
		controller_->ChangeSceneNum(S_PLAY);
	}
	if (isTransition == true)
	{
		SceneTransition();
	}
}

void TitleScene::Draw()
{
#pragma region �w�i3D�I�u�W�F�N�g�`��
	//// 3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw(controller_->dxCommon_->GetCommandList());

	//// 3D�I�u�W�F�N�g�̕`��

	/*fbxObject->Draw(dxCommon_->GetCommandList());*/

	sceneObj_->skydomeO_->Draw();

	//skydomeO_->Draw();

	///// <summary>
	///// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	///// </summary>

	//// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(controller_->dxCommon_->GetCommandList());
	// �w�i�X�v���C�g�`��

	sprite_->Draw();
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

	//skydomeO_->Draw();

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

	//
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

#pragma endregion
}

void TitleScene::SceneTransition()
{
	sceneObj_->transitionO_->worldTransform.scale_ += scale;
	sceneObj_->transitionO_->Update();
}
