#include "GameClearScene.h"
#include "ImguiManager.h"
#include "Ease.h"

#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

GameClearScene::GameClearScene(SceneManager* controller, SceneObjects* sceneObj)
{
	controller_ = controller;
	sceneObj_ = sceneObj;
}

GameClearScene::~GameClearScene()
{
	sceneObj_->Reset();
	ResetParam();
}

void GameClearScene::Initialize() {
	//                      num    pos         color       anchorpoint
	winSP_ = Sprite::Create(SpriteNumber::WIN, {645, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});
	winSP_->Initialize();

	// シーン遷移時のスプライト
	transSP_ = Sprite::Create(SpriteNumber::SCENETRANS, {WinApp::window_width / 2, WinApp::window_height / 2}, {1, 1, 1, 1},{0.5f, 0.5f});
	transSP_->Initialize();
	transSP_->SetSize({10.0f * 275.0f, 10.0f * 183.0f});

	player_ = sceneObj_->player;
	player_->GetFbxObject3d()->SetRotate({0, 0, 0});
	sceneObj_->spaceButton_->SetPosition({545, 575});
	playerPos = {0, 50, 0};
	sprite_ = Sprite::Create(SpriteNumber::SPACE, {310, 200});

	spSize = 5.0f;
}

void GameClearScene::Update(Input* input, GamePad* gamePad)
{
	gamePad->Update();
	//sceneObj_->transitionO_->Update();
	player_->GetFbxObject3d()->Update();
	//player_->GetObject3d()->Update();
	GameClearAnime();
	if (input->TriggerKey(DIK_SPACE) || gamePad->ButtonTrigger(X))
	{

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
	//sceneObj_->transitionO_->Draw();
	Object3d::PostDraw();

#pragma region
	// 前景スプライト描画前処理
	Sprite::PreDraw(controller_->dxCommon_->GetCommandList());

	//sprite_->Draw();
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	
	if(gameClearAnimeCount > 50)
	{
		winSP_->Draw();
	}
	if (gameClearAnimeCount > 150)
	{
		sceneObj_->spaceButton_->Draw();
	}
	transSP_->Draw();

	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameClearScene::GameClearAnime()
{

	gameClearAnimeCount++;

	addPos = (float)Ease::OutCubic(0.5f, 2, 60, gameClearAnimeCount);

	playerPos.y -= addPos;

	if (playerPos.y <= 0) {
		playerPos.y = 0;
	}

	player_->GetFbxObject3d()->SetPosition(playerPos);
	player_->GetFbxObject3d()->Update();


	winSP_->SetSize({spSize * 320.0f, spSize * 128.0f});
	if ( gameClearAnimeCount > 50 )
	{
		spSize -= 0.5f;
	}
	if ( spSize <= 1.0f )
	{
		spSize = 1.0f;
	}

	transSP_->SetColor({1, 1, 1, color});
	color -= 0.02f;
	controller_->GetGameCamera()->SetEye({0, 0, -20});
	controller_->GetGameCamera()->SetTarget({0, 0, 0});
	controller_->GetGameCamera()->Update();



	if (gameClearAnimeCount > 120) {
		winSpSize += 0.025f;
		winSP_->SetSize({winSpSize * 320.0f, winSpSize * 128.0f});
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
	sceneObj_->enemy->Reset();
	color = 1.0f;
	spSize = 5.0f;
}
