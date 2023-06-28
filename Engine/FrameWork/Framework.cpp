#include "Framework.h"

void Framework::Initialize()
{
	fps = new FPS();
	fps->SetFrameRate(60);

	winApp = new WinApp();
	winApp->Initialize();

	input = Input::GetInstance();
	input->Initialize(winApp);

	dxCommon_ = DirectXCommon::GetInstance();
	dxCommon_->Initialize(winApp);

	imgui = ImguiManager::GetInstance();
	imgui->Initialize(winApp, dxCommon_);
	endRequest_ = false;



	/*Sprite::LoadTexture(100,L"Resources/white1x1.png");*/

}

void Framework::Finalize()
{
	imgui->Finalize();
	winApp->Finalize();
	////FBX�������J��
	//FbxLoader::GetInstance()->Finalize();

	//���͉��
	delete imgui;
	delete winApp;
	delete input;
	delete dxCommon_;
	delete fps;
}

void Framework::Update()
{
	if (winApp->ProcessMessage())
	{
		SetRequest(true);
		//�Q�[�����[�v�𔲂���
	}

	fps->FpsControlBegin();
	input->Update();
	imgui->Begin();
}

void Framework::Run()
{
	//������
	Initialize();

	while (true)
	{
		//�X�V
		Update();
		if (IsEndRequest())
		{
			break;
			//�Q�[�����[�v�𔲂���
		}
		//�`��
		Draw();
	}
	//�������
	Finalize();
}
