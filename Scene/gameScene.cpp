#include "gameScene.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete camera_ ;
}

void GameScene::Initalize()
{
	//�C���X�^���X�̎擾
	input_ = Input::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();

	//input_ = Input::GetInstance();
	camera_ = new Camera(WinApp::window_width, WinApp::window_height);
	// �J���������_���Z�b�g
	camera_->SetTarget({ 0, 1, 0 });
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera_);
	//���C�g����
	light_ = Light::Create();
	//���C�g�F��ݒ�
	light_->SetLightColor({ 1,1,1 });
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLight(light_);

	Particle::SetCamera(camera_);

	Sprite::LoadTexture(1, L"Resources/kuribo-.jpg");
	Sprite::LoadTexture(2, L"Resources/mario.jpg");

	//�N���{�[
	sprite_ = Sprite::Create(1, { 0.0f,0.0f });
	sprite_->SetSize({128.0f,128.0f });

	//�}���I
	sprite2_ = Sprite::Create(2, { 300.0f,0.0f });
	sprite2_->SetSize({207.0f/2,205.0f/2});

	modelSkydome_ = Model::CreateFromOBJ("skydome");
	objSkydome_ = Object3d::Create();
	
	objSkydome_->SetModel(modelSkydome_);
	objSkydome_->SetColor({ 1,1,1,0.7f });

	modelPlayer_ = Model::CreateFromOBJ("player");
	objPlayer_ = Object3d::Create();

	objPlayer_->SetModel(modelPlayer_);

	modelParticle_ = Model::CreateFromOBJ("enemy");
	particle_ = Particle::Create();

	particle_->SetModel(modelParticle_);
	particle_->SetPosition({ objPlayer_->position.x+1 ,objPlayer_->position .y,objPlayer_->position.z});
	particle_->SetColor({ 1,1,1,0.7f });


	modelParticle2_ = Model::CreateFromOBJ("enemy2");
	particle2_ = Particle::Create();

	particle2_->SetModel(modelParticle2_);
	particle2_->SetPosition({ objPlayer_->position.x - 1 ,objPlayer_->position.y,objPlayer_->position.z });
	particle2_->SetColor({ 1,1,1,0.7f });

}

void GameScene::Update()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		if (isParticle == false)
		{
			isParticle = true;
		}
		else
		{
			isParticle = false;
		}
	}

	if (input_->PushKey(DIK_W))
	{
		camera_->eye.z += 0.2f;
		camera_->target.z += 0.2f;
		camera_->SetEye(camera_->eye);
	}
	if (input_->PushKey(DIK_S))
	{
		camera_->eye.z -= 0.2f;
		camera_->target.z -= 0.2f;
		camera_->SetEye(camera_->eye);
	}
	camera_->Update();


	objSkydome_->Update();
	objPlayer_->Update();
	particle_->Update();
	particle2_->Update();
	light_->Update();
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
	objSkydome_->Draw();


	objPlayer_->Draw();
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
	/*objEnemy_->Draw();*/
	if (isParticle == true)
	{
		particle_->Draw();
		particle2_->Draw();
	}
	
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
	sprite_->Draw();
	sprite2_->Draw();
	//
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

#pragma endregion
}


