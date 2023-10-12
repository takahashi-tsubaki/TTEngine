#include "StageSelectScene.h"

StageSelectScene::StageSelectScene(SceneManager* controller, SceneObjects* sceneObj)
{
	controller_ = controller;
	sceneObj_ = sceneObj;

	targetPos.translation_ = { 0,0,0 };
	followPos.translation_ = { 0,0,-50 };
}

StageSelectScene::~StageSelectScene()
{

	sceneObj_->Reset();
	isTransition = false;
	//delete sceneObj_->skydomeO_;
	/*sceneObj_->Delete();*/
}

void StageSelectScene::Initialize()
{

	player_ = sceneObj_->player_;
	enemy_ = sceneObj_->enemy_;

	controller_->camera_->SetFollowerPos(&followPos);

	controller_->camera_->SetTargetPos(&targetPos);
}

void StageSelectScene::Update(Input* input, GamePad* gamePad)
{
	gamePad->Update();
	//天球を回転させる処理

	sceneObj_->skydomeO_->Update();
	sceneObj_->transitionO_->Update();


	player_->GetObject3d()->Update();
	enemy_->GetObject3d()->Update();
	//ボタンを押したらシーン遷移を行う
	if (input->TriggerKey(DIK_SPACE) || gamePad->ButtonTrigger(X))
	{
		if (sceneObj_->transitionO_->worldTransform.scale_.x <= 1 || sceneObj_->transitionO_->worldTransform.scale_.z <= 1)
		{
			addScale = true;
			isTransition = true;
		}

	}
	if ( input->TriggerKey(DIK_TAB) || gamePad->ButtonTrigger(B) )
	{
		controller_->ChangeSceneNum(S_TITLE);
	}
	//シーン遷移のオブジェクトのスケールが一定以上に到達したとき
	if (isTransition == true)
	{
		if (sceneObj_->transitionO_->worldTransform.scale_.x >= 60 && sceneObj_->transitionO_->worldTransform.scale_.z >= 60)
		{
			
			player_->GetObject3d()->SetScale({ 1,1,1 });
			player_->GetObject3d()->SetPosition({ 0,0,-50 });
			sceneObj_->player_ = player_;
			sceneObj_->enemy_ = enemy_;
			controller_->camera_->SetFollowerPos(player_->GetObject3d()->GetWorldTransformPtr());
			controller_->camera_->SetTargetPos(enemy_->GetObject3d()->GetWorldTransformPtr());
			controller_->ChangeSceneNum(S_PLAY);
		}
	}

	SceneTransition();
	if (sceneObj_->transitionO_->worldTransform.scale_.x <= 0 || sceneObj_->transitionO_->worldTransform.scale_.z <= 0)
	{
		sceneObj_->transitionO_->worldTransform.scale_ = { 1,1,1 };
		subScale = false;
	}
	if (subScale == false)
	{

	}
	controller_->camera_->Update();
}

void StageSelectScene::Draw()
{
#pragma region
	//// 3Dオブジェクト描画前処理
	Object3d::PreDraw(controller_->dxCommon_->GetCommandList());

	//// 3Dオブジェクトの描画

	/*fbxObject->Draw(dxCommon_->GetCommandList());*/

	sceneObj_->skydomeO_->Draw();

	enemy_->Draw();
	player_->Draw();

	//skydomeO_->Draw();

	///// <summary>
	///// ここに3Dオブジェクトの描画処理を追加できる
	///// </summary>

	//// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma region
	// 背景スプライト描画前処理
	Sprite::PreDraw(controller_->dxCommon_->GetCommandList());
	// 背景スプライト描画

	//sprite_->Draw();
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


	if (subScale == true || addScale == true)
	{
		sceneObj_->transitionO_->Draw();
	}

	//skydomeO_->Draw();

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

	//sprite_->Draw();
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void StageSelectScene::SceneTransition()
{
	if (subScale == true)
	{
		sceneObj_->transitionO_->worldTransform.scale_ -= scale;
	}
	else if(addScale == true)
	{
		sceneObj_->transitionO_->worldTransform.scale_ += scale;
	}
	sceneObj_->transitionO_->Update();
}
