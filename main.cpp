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

//�֐��̃v���g�^�C�v�錾
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

	//��I������
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);
	// 3D�I�u�W�F�N�g�ÓI������
	Object3d::StaticInitialize(dxCommon->GetDevice());
	//���C�g��I������
	Light::StaticInitalize(dxCommon->GetDevice());
	Particle::StaticInitialize(dxCommon->GetDevice());

	//FBX�̏�����
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());

	Sprite::LoadTexture(100,L"Resources/white1x1.png");

	postEffect = new PostEffect();
	postEffect->Initialize();

	//�Q�[���V�[���̏�����
	gameScene = new GameScene();
	gameScene->Initalize();
	//�Q�[�����[�v
	while (true)
	{

		fps->FpsControlBegin();
		if (winApp->ProcessMessage())
		{
			//�Q�[�����[�v�𔲂���
			break;
		}
		//��������DirectX���t���[������

		input->Update();

		gameScene->Update();

		postEffect->PreDrawScene(dxCommon->GetCommandList());
		//�Q�[���V�[���̕`��
		gameScene->Draw();
		postEffect->PostDrawScene(dxCommon->GetCommandList());

		//�`��O����
		dxCommon->preDraw();

		postEffect->Draw(dxCommon->GetCommandList());

		//�`��㏈��
		dxCommon->postDraw();


		fps->FpsControlEnd();
		//�����܂�DirectX���t���[������
	}

	/*if (SUCCEEDED(dxCommon->GetDevice()->QueryInterface(&debugInterface))) {
		debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
		debugInterface->Release();
	}*/

	//windowAPI�̏I������
	winApp->Finalize();
	////FBX�������J��
	//FbxLoader::GetInstance()->Finalize();

	//���͉��
	delete winApp;
	delete gameScene;

	return 0;
}

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
		//�E�B���h�E���j�����ꂽ
	case WM_DESTROY:
		//OS�ɑ΂���,�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}

	//�W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}