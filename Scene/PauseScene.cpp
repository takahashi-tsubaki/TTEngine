#include "PauseScene.h"
#include "PlayScene.h"
PauseScene::PauseScene(SceneManager* controller, SceneObjects* sceneObj)
{
	controller_ = controller;
	sceneObj_ = sceneObj;

	player_ = sceneObj_->player_;
	enemy_ = sceneObj_->enemy_;

	operationSP_ = Sprite::Create(SpriteNumber::OPERATION, {400, 100});
	operationSP_->Initialize();

	fCheckSP_ = Sprite::Create(SpriteNumber::FINALCHECK, {760, 300});
	fCheckSP_->Initialize();
}


PauseScene::~PauseScene()
{
	//sceneObj_->Reset();
}

void PauseScene::Initialize()
{
	finalCheck = false;
	checkNum = 0;
}

void PauseScene::Update(Input* input, GamePad* gamePad)
{
	gamePad->Update();
	if (checkNum == 0)
	{
		if (input->TriggerKey(DIK_ESCAPE) || gamePad->ButtonTrigger(X)) {
			checkNum = 1;
		}
		if (input->TriggerKey(DIK_TAB) || gamePad->ButtonTrigger(START)) {
			sceneObj_->player_ = player_;
			sceneObj_->enemy_ = enemy_;
			checkNum = 0;
			controller_->PopScene();
		}
	}
	

	if (checkNum == 1)
	{
		if (input->TriggerKey(DIK_ESCAPE) || gamePad->ButtonTrigger(X)) {
			player_->Reset();
			enemy_->Reset();
			checkNum = 2;
			controller_->ChangeSceneNum(S_TITLE);
		}
		if (input->TriggerKey(DIK_TAB) || gamePad->ButtonTrigger(START)) {
			sceneObj_->player_ = player_;
			sceneObj_->enemy_ = enemy_;
			checkNum = 0;
			controller_->PopScene();
		}
	}

	//player_->Update(input, gamePad);
	//enemy_->Update();
}

void PauseScene::Draw()
{
#pragma region
	// 背景スプライト描画前処理
	Sprite::PreDraw(controller_->dxCommon_->GetCommandList());
	// 背景スプライト描画


	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion

#pragma region
	//// 3Dオブジェクト描画前処理
	Object3d::PreDraw(controller_->dxCommon_->GetCommandList());

	//// 3Dオブジェクトの描画

	/*fbxObject->Draw(dxCommon_->GetCommandList());*/

	sceneObj_->skydomeO_->Draw();
	enemy_->Draw();
	player_->Draw();
	//player_->Draw();
	//enemy_->Draw();

	///// <summary>
	///// ここに3Dオブジェクトの描画処理を追加できる
	///// </summary>

	//// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion
#pragma region
	//// 3Dオブジェクト描画前処理
	Object3d::PreDraw(controller_->dxCommon_->GetCommandList());

	//// 3Dオブジェクトの描画

	/*fbxObject->Draw(dxCommon_->GetCommandList());*/

	player_->GetParticle()->Draw(controller_->dxCommon_->GetCommandList());
	enemy_->GetParticle()->Draw(controller_->dxCommon_->GetCommandList());

	///// <summary>
	///// ここに3Dオブジェクトの描画処理を追加できる
	///// </summary>

	//// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

	//#pragma region ぺらポリゴン描画
	//	postEffect->PreDrawScene(dxCommon_->GetCommandList());
	//
	//	//// ぺらポリゴンの描画
	//	postEffect->Draw(dxCommon_->GetCommandList());
	//	///// <summary>
	//	///// ここにぺらポリゴンの描画処理を追加できる
	//	///// </summary>
	//
	//	
	//
	//	postEffect->PostDrawScene(dxCommon_->GetCommandList());
	//
	//
	//
	//#pragma endregion
#pragma region
	// 前景スプライト描画前処理
	Sprite::PreDraw(controller_->dxCommon_->GetCommandList());


	if (checkNum == 1)
	{
		fCheckSP_->Draw();
	}
	else if (checkNum == 0)
	{
		operationSP_->Draw();
	}
	//sprite_->Draw();
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	//playerHpSprite_->Draw();
	//enemyHpSprite_->Draw();
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
