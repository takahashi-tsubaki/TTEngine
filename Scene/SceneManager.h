#pragma once
#include "Defineder.h"
#include "Pading.h"

ALICE_SUPPRESS_WARNINGS_BEGIN
#include <memory>
#include <stack>
ALICE_SUPPRESS_WARNINGS_END

#include "DirectXCommon.h"
#include "GameCamera.h"
#include "Input.h"
#include "GamePad.h"
#include "SceneObjects.h"
class IScene;
class Input;
/// <summary>
/// シーンマネージャー
/// </summary>
class SceneManager
{
public:
	DirectXCommon* dxCommon_;
	GameCamera* camera_;

	Input* input_ = nullptr;
	GamePad* gamePad_ = nullptr;
protected:
	std::stack<std::shared_ptr<IScene>> _scene;
	SceneObjects* sceneObjects_;

	bool isChange = false;
	int sceneNum = 0;

public:
	//コンストラクタとデストラクタ
	SceneManager(DirectXCommon* dxCommon,GameCamera* camera,SceneObjects* sceneObjects);
	~SceneManager();

	DirectXCommon* GetDxCommon() {
		return dxCommon_;
	}

	GameCamera* GetGameCamera() {
		return camera_;
	}


	Input* GetInput() {
		return input_;
	}

	GamePad* GetGamePad() {
		return gamePad_;
	}

	//オブジェクトのInitializeを呼び出す
	void ObjectInitialize();
	// 各シーンのInitializeを呼び出す
	void SceneInitialize();
	// 各シーンのUpdateを呼び出す
	void SceneUpdate(Input* input,GamePad* gamePad);
	// 各シーンのDrawを呼び出す
	void SceneDraw();

	//シーンを変更する番号をセット
	void ChangeSceneNum(int number);
	// シーンを変更する
	void ChangeScene();
	//シーンを挿入
	void PushScene(int number);
	//シーンを取り出す
	void PopScene();
	//シーンの番号を取得
	int GetSceneNum() {
		return sceneNum;
	}

};

