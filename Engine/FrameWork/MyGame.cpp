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


	//情的初期化
	Sprite::StaticInitialize(dxCommon_->GetDevice(), WinApp::window_width, WinApp::window_height);
	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon_->GetDevice());
	//ライト情的初期化
	Light::StaticInitalize(dxCommon_->GetDevice());
	Particle::StaticInitialize(dxCommon_->GetDevice());

	//FBXの初期化
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());

	/*Sprite::LoadTexture(100,L"Resources/white1x1.png");*/

	postEffect = new PostEffect();
	postEffect->Initialize();

	//ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initalize();
}

void MyGame::Finalize()
{
	winApp->Finalize();
	////FBXメモリ開放
	//FbxLoader::GetInstance()->Finalize();

	//入力解放
	delete winApp;
	delete imgui;
	delete gameScene;

	//基底クラスの終了処理
	Framework::Finalize();
}

void MyGame::Update()
{

	//基底クラスの更新処理
	Framework::Update();


	//ゲームループ
	while (true)
	{

		fps->FpsControlBegin();
		if (winApp->ProcessMessage())
		{
			Framework::SetRequest(true);
			//ゲームループを抜ける
		}
		//ここからDirectX毎フレーム処理

		input->Update();

		

		gameScene->Update();
	
		Draw();

		fps->FpsControlEnd();
		//ここまでDirectX毎フレーム処理
	}

	ImGui::Begin("Pause");
	
	ImGui::End();
}

void MyGame::Draw()
{
	postEffect->PreDrawScene(dxCommon_->GetCommandList());
	//ゲームシーンの描画
	gameScene->Draw();
	postEffect->PostDrawScene(dxCommon_->GetCommandList());

	//描画前処理
	dxCommon_->preDraw();

	postEffect->Draw(dxCommon_->GetCommandList());

	//描画後処理
	dxCommon_->postDraw();
}
