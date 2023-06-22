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

	//�I�u�W�F�N�g��Initialize���Ăяo��
	void ObjectInitialize();
	// �e�V�[����Initialize���Ăяo��
	void SceneInitialize();
	// �e�V�[����Update���Ăяo��
	void SceneUpdate(Input* input);
	// �e�V�[����Draw���Ăяo��
	void SceneDraw();

	// �V�[����ύX����
	void ChangeScene(IScene*);

	void PushScene(IScene*);

	void PopScene();

};

