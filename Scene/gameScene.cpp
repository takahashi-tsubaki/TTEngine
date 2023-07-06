#include "gameScene.h"
#pragma warning(push)
#include "FbxLoader.h"
#include "FbxObject3d.h"

#include "ImguiManager.h"
#pragma warning(pop)

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete camera_ ;
	delete fbxObject;
	delete fbxModel;
	/*safe_delete(fbxObject);
	safe_delete(fbxModel);*/
}

void GameScene::Initalize(DirectXCommon* dxCommon, Input* input, GamePad* gamePad)
{
	dxCommon_ = dxCommon;
	input_ = input;
	//�C���X�^���X�̎擾
	gamePad_ = gamePad;

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

	object = Object3d::Create();
	model = Model::CreateFromOBJ("cube");
	object->SetModel(model);

	Sprite::LoadTexture(1, L"Resources/kuribo-.jpg");
	Sprite::LoadTexture(2, L"Resources/mario.jpg");

	sprite_ = Sprite::Create(1,{WinApp::window_width,WinApp::window_height});


	fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("boss_prot4");

	

	enemy_ = new Enemy();
	enemy_->Initialize(dxCommon_);
	
	player_ = new Player();
	player_->Initialize(dxCommon_, input_ , gamePad_, enemy_);

	fbxObject =  new FbxObject3d();
	fbxObject->Initialize();
	fbxObject->SetModel(fbxModel);
	//fbxObject->SetScale({0.1f,0.1f,0.1f});
	//fbxObject->SetPosition({ 0,-50,0 });

	fbxObject->SetPosition({0,-10,10});

	colMan = CollisionManager::GetInstance();
	
}

void GameScene::Update()
{

	camera_->eye_.y = 20.0f;

	Vector3 move;
	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= 0.5f;
	}
	if (input_->PushKey(DIK_RIGHT))
	{
		move.x += 0.5f;
	}
	if (input_->PushKey(DIK_UP))
	{
		move.z += 0.5f;
	}

	if (input_->PushKey(DIK_DOWN))
	{
		move.z -= 0.5f;
	}

	


	light_->Update();
	gamePad_->Update();

	fbxObject->Update();

	ImGui::Begin("cameraPos");
	ImGui::SetWindowPos({ 200 , 200 });
	ImGui::SetWindowSize({ 500,100 });
	ImGui::InputFloat3("isPause", &camera_->eye_.x);
	ImGui::InputFloat3("isPause", &camera_->target_.x);
	ImGui::End();

	object->Update();

	player_->Update();
	enemy_->Update();

	/*camera_->disEyeTarget(player_->GetPosition(),enemy_->GetPosition());*/
	camera_->MoveTarget(input_);
	camera_->Update();
	
	colMan->CheckAllCollisions();
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

	/*fbxObject->Draw(dxCommon_->GetCommandList());
	object->Draw();*/
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
	Particle::PreDraw(dxCommon_->GetCommandList());

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
	Sprite::PreDraw(dxCommon_->GetCommandList());

	//sprite_->Draw();
	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	//
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

#pragma endregion
}


