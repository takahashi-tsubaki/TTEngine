#include "GameOverScene.h"
#include "ImguiManager.h"
GameOverScene::GameOverScene(SceneManager* controller, SceneObjects* sceneObj)
{
	controller_ = controller;
	sceneObj_ = sceneObj;
}

GameOverScene::~GameOverScene()
{
	sceneObj_->Reset();
}

void GameOverScene::Initialize()
{
}

void GameOverScene::Update(Input* input, GamePad* gamePad)
{
	gamePad->Update();
	if (input->TriggerKey(DIK_RETURN) || gamePad->ButtonTrigger(X))
	{
		controller_->ChangeSceneNum(S_TITLE);
	}
	assert(input);
	controller_->camera_->Update();
}

void GameOverScene::Draw()
{
}
