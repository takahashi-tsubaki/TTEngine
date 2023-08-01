#include "SceneManager.h"
#include "Input.h"
#include "SceneInc.h"

#include <cassert>

SceneManager::SceneManager(DirectXCommon* dxCommon,GameCamera* camera, Input* input, GamePad* gamePad)
{
	dxCommon_ = dxCommon;
	input_ = input;
	gamePad_ = gamePad;
	_scene.emplace(new TitleScene(&*this));
	camera_ = camera;

}

SceneManager::~SceneManager()
{
}

void SceneManager::SceneInitialize()
{
	_scene.top().get()->Initialize();
}

void SceneManager::SceneUpdate(Input* input)
{
	_scene.top().get()->Update(input);
}

void SceneManager::SceneDraw()
{
	_scene.top().get()->Draw();
}

void SceneManager::ChangeSceneNum(int number)
{
	sceneNum = number;
	if (isChange == false)
	{
		isChange = true;
	}
}

void SceneManager::ChangeScene()
{
	if (isChange == true)
	{
		
		switch (sceneNum)
		{
		case S_TITLE:
			_scene.pop();
			_scene.emplace(new TitleScene(&*this));
			SceneInitialize();
			break;
		case S_PLAY:
			_scene.pop();
			_scene.emplace(new PlayScene(&*this));
			SceneInitialize();
			break;
		case S_RESULT:
			_scene.pop();
			_scene.emplace(new ResultScene(&*this));
			SceneInitialize();
			break;
		case 4:
			_scene.pop();
			_scene.emplace(new TitleScene(&*this));
			SceneInitialize();
			break;
		default:
			_scene.emplace(new TitleScene(&*this));
			SceneInitialize();
			break;
		}
		isChange = false;

	
	}
	else
	{
		isChange = false;
	}
	
}

void SceneManager::PushScene(IScene*)
{
}

void SceneManager::PopScene()
{
}
