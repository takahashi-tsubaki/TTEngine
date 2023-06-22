#include "SceneManager.h"

#include "Input.h"

#include "SceneInc.h"

SceneManager::SceneManager(DirectXCommon* dxCommon)
{
	/*dxCommon_ = dxCommon;
	_scene.top().reset(new TitleScene(&* this));*/
}

SceneManager::~SceneManager()
{
}

void SceneManager::SceneInitialize()
{
	_scene.top().get()->Initialize(dxCommon_);
}

void SceneManager::SceneUpdate(Input* input)
{
	_scene.top().get()->Update(input);
}

void SceneManager::SceneDraw()
{
	_scene.top().get()->Draw(dxCommon_);
}

void SceneManager::ChangeScene(IScene* scene)
{
	_scene.top().reset(scene);
}

void SceneManager::PushScene(IScene*)
{
}

void SceneManager::PopScene()
{
}
