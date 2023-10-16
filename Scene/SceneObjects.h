#pragma once
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
#include "Player.h"
#include "Enemy.h"

class SceneManager;
/// <summary>
/// シーンオブジェクト
/// </summary>
class SceneObjects
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="controller"></param>
	void Initialize(SceneManager* controller);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 削除
	/// </summary>
	void Delete();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

public:
	SceneObjects* sceneObj = nullptr;

	FbxModel* fbxModel = nullptr;
	FbxObject3d* fbxObject = nullptr;
	Sprite* sprite_ = nullptr;


	Sprite* spaceButton_ = nullptr;

	//天球
	Object3d* skydomeO_ = nullptr;
	Model* skydomeM_ = nullptr;

	//天球
	Object3d* selectSkydomeO_ = nullptr;
	Model* selectSkydomeM_ = nullptr;

	Model* model = nullptr;
	Object3d* object = nullptr;

	Model* transitionM_ = nullptr;
	Object3d* transitionO_ = nullptr;


	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;

	SceneManager* controller_;
};

