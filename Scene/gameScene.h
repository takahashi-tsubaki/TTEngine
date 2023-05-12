#pragma once
#include "DirectXCommon.h"
#include "Input.h"

#include "Sprite.h"

#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "Object3d.h"

#include "ParticleManager.h"

#include "Vector3.h"

#include "FbxObject3d.h"

class GameScene
{

public:

	GameScene();
	~GameScene();

	void Initalize();

	void Update();

	void Draw();

	

private:
	DirectXCommon* dxCommon_ = nullptr;

	Input* input_ = nullptr;


	Sprite* sprite_ = nullptr;
	Sprite* sprite2_ = nullptr;


	Camera* camera_ = nullptr;
	Light* light_ = nullptr;

	FbxModel* fbxModel = nullptr;
	FbxObject3d* fbxObject = nullptr;
};