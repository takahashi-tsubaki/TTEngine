#include "MyGame.h"


void DebugOutputFormatString(const char* format, ...) {
#ifdef _DEBUG
	va_list valist;
	va_start(valist, format);
	vprintf(format, valist);
	va_end(valist);
#endif
}

void MyGame::Initialize()
{

	Framework::Initialize();

	OutputDebugStringA("Hello DirectX!!\n");

	fps->SetFrameRate(60);

	winApp = new WinApp();
	winApp->Initialize();
	input = Input::GetInstance();
	input->Initialize(winApp);

	dxCommon_ = DirectXCommon::GetInstance();
	dxCommon_->Initialize(winApp);

	imgui = ImguiManager::GetInstance();
	imgui->Initialize(winApp,dxCommon_);


	//��I������
	Sprite::StaticInitialize(dxCommon_->GetDevice(), WinApp::window_width, WinApp::window_height);
	// 3D�I�u�W�F�N�g�ÓI������
	Object3d::StaticInitialize(dxCommon_->GetDevice());
	//���C�g��I������
	Light::StaticInitalize(dxCommon_->GetDevice());
	Particle::StaticInitialize(dxCommon_->GetDevice());

	//FBX�̏�����
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());

	/*Sprite::LoadTexture(100,L"Resources/white1x1.png");*/

	postEffect = new PostEffect();
	postEffect->Initialize();

	//�Q�[���V�[���̏�����
	gameScene = new GameScene();
	gameScene->Initalize();
}

void MyGame::Finalize()
{
	winApp->Finalize();
	////FBX�������J��
	//FbxLoader::GetInstance()->Finalize();

	//���͉��
	delete winApp;
	delete imgui;
	delete gameScene;

	//���N���X�̏I������
	Framework::Finalize();
}

void MyGame::Update()
{

	//���N���X�̍X�V����
	Framework::Update();


	//�Q�[�����[�v
	while (true)
	{

		fps->FpsControlBegin();
		if (winApp->ProcessMessage())
		{
			Framework::SetRequest(true);
			//�Q�[�����[�v�𔲂���
		}
		//��������DirectX���t���[������

		input->Update();

		

		gameScene->Update();
	
		Draw();

		fps->FpsControlEnd();
		//�����܂�DirectX���t���[������
	}

	ImGui::Begin("Pause");
	
	ImGui::End();
}

void MyGame::Draw()
{
	postEffect->PreDrawScene(dxCommon_->GetCommandList());
	//�Q�[���V�[���̕`��
	gameScene->Draw();
	postEffect->PostDrawScene(dxCommon_->GetCommandList());

	//�`��O����
	dxCommon_->preDraw();

	postEffect->Draw(dxCommon_->GetCommandList());

	//�`��㏈��
	dxCommon_->postDraw();
}
