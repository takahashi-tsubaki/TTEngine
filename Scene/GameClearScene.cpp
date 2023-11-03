#include "GameClearScene.h"
#include "ImguiManager.h"
#include "Ease.h"
GameClearScene::GameClearScene(SceneManager* controller, SceneObjects* sceneObj)
{
	controller_ = controller;
	sceneObj_ = sceneObj;
}

GameClearScene::~GameClearScene()
{
	sceneObj_->Reset();
}

void GameClearScene::Initialize()
{
	//                      num    pos         color       anchorpoint
	winSP_ = Sprite::Create(14, {645, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});
	winSP_->Initialize();

	player_ = sceneObj_->player_;
	sceneObj_->spaceButton_->SetPosition({545, 575});
	playerPos = {0, 50, 0};
	sprite_ = Sprite::Create(4, { 310,200 });
}

void GameClearScene::Update(Input* input, GamePad* gamePad)
{
	gamePad->Update();
	sceneObj_->transitionO_->Update();
	player_->GetObject3d()->Update();
	GameClearAnime();
	if (input->TriggerKey(DIK_SPACE) || gamePad->ButtonTrigger(X))
	{
		ResetParam();
		controller_->ChangeSceneNum(S_TITLE);
	}

	assert(input);
	controller_->GetGameCamera()->Update();
}

void GameClearScene::Draw()
{
	//// 3Dオブジェクト描画前処理
	Object3d::PreDraw(controller_->dxCommon_->GetCommandList());

	sceneObj_->skydomeO_->Draw();
	player_->Draw();
	sceneObj_->transitionO_->Draw();
	Object3d::PostDraw();

#pragma region
	// 前景スプライト描画前処理
	Sprite::PreDraw(controller_->dxCommon_->GetCommandList());

	//sprite_->Draw();
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	
	if(gameClearAnimeCount > 60)
	{
		winSP_->Draw();
	}
	if (gameClearAnimeCount > 150)
	{
		sceneObj_->spaceButton_->Draw();
	}
	

	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameClearScene::GameClearAnime()
{
	sceneObj_->transitionO_->SetColor({1, 1, 1, color});
	color -= 0.02f;
	controller_->GetGameCamera()->SetEye({0, 0, -20});
	controller_->GetGameCamera()->SetTarget({0, 0, 0});
	controller_->GetGameCamera()->Update();

	gameClearAnimeCount++;

	addPos = (float)Ease::OutCubic(0.5f, 2, 60, gameClearAnimeCount);

	playerPos.y -= addPos;

	if (playerPos.y <= 0) {
		playerPos.y = 0;
	}

	player_->GetObject3d()->SetPosition(playerPos);
	player_->GetObject3d()->UpdateMatrix();
	if (gameClearAnimeCount > 90) {
		winSpSize += 0.025f;
		winSP_->SetSize({winSpSize * 320.0f, 128.0f});
		winSPAlpha -= 0.025f;
		winSP_->SetColor({1, 1, 1, winSPAlpha});
	};

}

void GameClearScene::ResetParam()
{
	gameClearAnimeCount = 0;
	winSpSize = 1.0f;
	winSPAlpha = 1.0f;
	addPos = 0.5f;
	player_->Reset();
	sceneObj_->enemy_->Reset();
}
