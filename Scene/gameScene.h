#pragma once
#include "ErrorException.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "GamePad.h"

#include "Sprite.h"
#include "PostEffect.h"

#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "Object3d.h"

#include "ParticleManager.h"

#include "Vector3.h"

#include "FbxObject3d.h"

#include "SceneManager.h"
#include "SceneObjects.h"
class GameScene
{

public:
	//コンストラクタとデストラクタ
	GameScene();
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxCommon"></param>
	/// <param name="input"></param>
	/// <param name="gamePad"></param>
	void Initalize(TTEngine::DirectXCommon* dxCommon, Input* input, GamePad* gamePad);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public:
	SceneObjects* sceneObjects = nullptr;
private:
	TTEngine::DirectXCommon* dxCommon_ = nullptr;

	Input* input_ = nullptr;
	GamePad* gamePad_ = nullptr;

	Sprite* sprite_ = nullptr;
	Sprite* sprite2_ = nullptr;

	Model* model = nullptr;
	Object3d* object = nullptr;

	PostEffect* postEffect = nullptr;

	Camera* camera_ = nullptr;

	GameCamera* gameCamera = nullptr;

	Light* light_ = nullptr;

	FbxModel* fbxModel = nullptr;
	FbxObject3d* fbxObject = nullptr;

	//天球
	Object3d* skydomeO_ = nullptr;
	Model* skydomeM_ = nullptr;

	CollisionManager* colMan = nullptr;

	IScene* scene_ = nullptr;
	SceneManager* sceneManager = nullptr;

};
