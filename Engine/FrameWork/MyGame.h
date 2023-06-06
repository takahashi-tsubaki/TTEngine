#pragma once
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

#include "Framework.h"

class MyGame : public Framework
{
public:

	//������
	void Initialize() override;

	//�I��
	void Finalize() override;

	//���t���[���X�V
	void Update() override;

	//�`��
	void Draw() override;

private:
	ID3D12DebugDevice* debugInterface;
	//FPS
	FPS* fps = new FPS;
	WinApp* winApp = nullptr;
	Input* input = nullptr;
	DirectXCommon* dxCommon_ = nullptr;

	PostEffect* postEffect = nullptr;

	GameScene* gameScene = nullptr;

};