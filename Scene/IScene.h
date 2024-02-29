#pragma once
#include "DirectXCommon.h"
#include "SceneManager.h"
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
#include "Character.h"
class IScene
{

protected:

	SceneManager* controller;

public:
	//コンストラクタ
	IScene();
	//デストラクタ
	virtual ~IScene();
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input"></param>
	/// <param name="gamePad"></param>
	virtual void Update(Input* input,GamePad* gamePad) = 0;
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

};

