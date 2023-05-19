#include "gameScene.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete camera_ ;
	delete fbxObject;
	delete fbxModel;
	/*safe_delete(fbxObject);
	safe_delete(fbxModel);*/
}

void GameScene::Initalize()
{
	//�C���X�^���X�̎擾
	input_ = Input::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();

	//input_ = Input::GetInstance();
	camera_ = new Camera(WinApp::window_width, WinApp::window_height);
	camera_->SetEye({0,0,-100});
	// �J���������_���Z�b�g
	camera_->SetTarget({ 0, 0, 0 });
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera_);
	//���C�g����
	light_ = Light::Create();
	//���C�g�F��ݒ�
	light_->SetLightColor({ 1,1,1 });
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLight(light_);

	//�f�o�C�X���Z�b�g
	FbxObject3d::SetDevice(dxCommon_->GetDevice());
	//�J�������Z�b�g
	FbxObject3d::SetCamera(camera_);
	//�O���t�B�b�N�X�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();

	Sprite::LoadTexture(1, L"Resources/kuribo-.jpg");
	Sprite::LoadTexture(2, L"Resources/mario.jpg");

	fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	

	fbxObject =  new FbxObject3d();
	fbxObject->Initialize();
	fbxObject->SetModel(fbxModel);
	fbxObject->SetScale({0.01f,0.01f,0.01f});

	//fbxObject->SetPosition({0,0,100});
	//fbxObject->PlayAnimetion(3);
}

void GameScene::Update()
{

	/*if (input_->TriggerKey(DIK_SPACE))
	{
		fbxObject->PlayAnimetion(2);
	}*/

	camera_->Update();
	light_->Update();

	fbxObject->Update();

	
}

void GameScene::Draw()
{
#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon_->GetCommandList());
	// �w�i�X�v���C�g�`��

	
	/// <summary>
	/// �����ɔw�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion

#pragma region 3D�I�u�W�F�N�g�`��
	//// 3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw(dxCommon_->GetCommandList());

	//// 3D�I�u�W�F�N�g�̕`��

	fbxObject->Draw(dxCommon_->GetCommandList());

	///// <summary>
	///// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	///// </summary>

	//// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
#pragma endregion

#pragma region �p�[�e�B�N���`��

	//// �p�[�e�B�N���`��O����
	Particle::PreDraw(dxCommon_->GetCommandList());

	//// 3D�I�u�W�F�N�g�̕`��
	
	///// <summary>
	///// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	///// </summary>

	//// �p�[�e�B�N���`��㏈��
	Particle::PostDraw();

#pragma endregion

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon_->GetCommandList());

	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	//
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

#pragma endregion
}


