#include "GameOverScene.h"
#include "ImguiManager.h"
#include "Ease.h"
GameOverScene::GameOverScene(SceneManager* controller, SceneObjects* sceneObj)
{
	controller_ = controller;
	sceneObj_ = sceneObj;
}

GameOverScene::~GameOverScene()
{
	sceneObj_->Reset();
}

void GameOverScene::Initialize() {
	sprite_ = Sprite::Create(SpriteNumber::SPACE, {400, 200});

	loseSP_ = Sprite::Create(SpriteNumber::LOSE, {645, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});
	loseSP_->Initialize();

	enemy_ = sceneObj_->enemy_;
	sceneObj_->spaceButton_->SetPosition({545,575});
	enemyPos = {0, 50, 0};
}

void GameOverScene::Update(Input* input, GamePad* gamePad)
{
	gamePad->Update();
	sceneObj_->transitionO_->Update();
	GameOverAnime();
	enemy_->GetObject3d()->Update();
	enemy_->GetFbxObject3d()->Update();
	if (input->TriggerKey(DIK_SPACE) || gamePad->ButtonTrigger(X))
	{
		ResetParam();
		controller_->ChangeSceneNum(S_TITLE);
	}
	assert(input);
	controller_->GetGameCamera()->Update();
}

void GameOverScene::Draw()
{
#pragma region

	//// 3Dオブジェクト描画前処理
	Object3d::PreDraw(controller_->dxCommon_->GetCommandList());

	sceneObj_->skydomeO_->Draw();
	enemy_->Draw();
	sceneObj_->transitionO_->Draw();
	Object3d::PostDraw();
	// 前景スプライト描画前処理
	Sprite::PreDraw(controller_->dxCommon_->GetCommandList());

	//sprite_->Draw();
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	if (gameOverAnimeCount > 60)
	{
		loseSP_->Draw();
	}
	if (gameOverAnimeCount > 150)
	{
		sceneObj_->spaceButton_->Draw();
	}
	//sprite_->Draw();
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameOverScene::GameOverAnime() {


	sceneObj_->transitionO_->SetColor({1, 1, 1, color});
	color -= 0.02f;

	controller_->GetGameCamera()->SetEye({0, 0, -20});
	controller_->GetGameCamera()->SetTarget({0,0,0});
	controller_->GetGameCamera()->Update();

	gameOverAnimeCount++;

	addPos = (float) Ease::OutCubic(0.5f, 2, 60, gameOverAnimeCount);

	enemyPos.y -= addPos;

	if (enemyPos.y <= 0)
	{
		enemyPos.y = 0;
	}

	enemy_->GetObject3d()->SetPosition(enemyPos);

	enemy_->GetObject3d()->UpdateMatrix();
	enemy_->GetFbxObject3d()->SetPosition(enemyPos);
	enemy_->GetFbxObject3d()->SetRotate({0, 0, 0});
	enemy_->GetFbxObject3d()->PlayAnimation(0,false);
	enemy_->GetFbxObject3d()->Update();

	if (gameOverAnimeCount > 90)
	{
		loseSpSize += 0.025f;
		loseSP_->SetSize({loseSpSize*320.0f,128.0f});
		loseSPAlpha -= 0.025f;
		loseSP_->SetColor({1, 1, 1, loseSPAlpha});
	}

}

void GameOverScene::ResetParam()
{
	gameOverAnimeCount = 0;
	loseSpSize = 1.0f;
	loseSPAlpha = 1.0f;
	addPos = 0.5f;
	enemy_->Reset();
	sceneObj_->player_->Reset();
}
