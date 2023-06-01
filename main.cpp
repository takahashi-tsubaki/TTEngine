#include "DirectXCommon.h"
#include "WinApp.h"
#include "FPS.h"
#include "Input.h"
#include "gameScene.h"
#include "Object3d.h"
#include "Light.h"
#include "ParticleManager.h"
#include "PostEffect.h"

#include "FbxLoader.h"

void DebugOutputFormatString(const char* format, ...) {
#ifdef _DEBUG
	va_list valist;
	va_start(valist, format);
	vprintf(format, valist);
	va_end(valist);
#endif
}

//関数のプロトタイプ宣言
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{

	ID3D12DebugDevice* debugInterface;
	OutputDebugStringA("Hello DirectX!!\n");
	//FPS
	FPS* fps = new FPS;
	fps->SetFrameRate(60);

	WinApp* winApp = nullptr;
	Input* input = nullptr;
	DirectXCommon* dxCommon = nullptr;

	PostEffect* postEffect = nullptr;

	GameScene* gameScene = nullptr;

	

	winApp = new WinApp();
	winApp->Initialize();
	input = Input::GetInstance();
	input->Initialize(winApp);

	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(winApp);

	//情的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);
	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon->GetDevice());
	//ライト情的初期化
	Light::StaticInitalize(dxCommon->GetDevice());
	Particle::StaticInitialize(dxCommon->GetDevice());

	//FBXの初期化
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());

	Sprite::LoadTexture(100,L"Resources/white1x1.png");

	postEffect = new PostEffect();
	postEffect->Initialize();

	//ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initalize();
	//ゲームループ
	while (true)
	{

		fps->FpsControlBegin();
		if (winApp->ProcessMessage())
		{
			//ゲームループを抜ける
			break;
		}
		//ここからDirectX毎フレーム処理

		input->Update();

		gameScene->Update();

		postEffect->PreDrawScene(dxCommon->GetCommandList());
		//ゲームシーンの描画
		gameScene->Draw();
		postEffect->PostDrawScene(dxCommon->GetCommandList());

		//描画前処理
		dxCommon->preDraw();

		postEffect->Draw(dxCommon->GetCommandList());

		//描画後処理
		dxCommon->postDraw();


		fps->FpsControlEnd();
		//ここまでDirectX毎フレーム処理
	}

	/*if (SUCCEEDED(dxCommon->GetDevice()->QueryInterface(&debugInterface))) {
		debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
		debugInterface->Release();
	}*/

	//windowAPIの終了処理
	winApp->Finalize();
	////FBXメモリ開放
	//FbxLoader::GetInstance()->Finalize();

	//入力解放
	delete winApp;
	delete gameScene;

	return 0;
}

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対して,アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}