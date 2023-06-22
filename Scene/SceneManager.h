#pragma once
#include <memory>
#include <stack>


#include "DirectXCommon.h"

class IScene;
class Input;

class SceneManager
{
protected:
	std::stack<std::shared_ptr<IScene>> _scene;
	DirectXCommon* dxCommon_;

public:

	SceneManager(DirectXCommon* dxCommon);
	~SceneManager();

	//オブジェクトのInitializeを呼び出す
	void ObjectInitialize();
	// 各シーンのInitializeを呼び出す
	void SceneInitialize();
	// 各シーンのUpdateを呼び出す
	void SceneUpdate(Input* input);
	// 各シーンのDrawを呼び出す
	void SceneDraw();

	// シーンを変更する
	void ChangeScene(IScene*);

	void PushScene(IScene*);

	void PopScene();

};

