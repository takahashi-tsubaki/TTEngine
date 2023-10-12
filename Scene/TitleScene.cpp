#include "TitleScene.h"
#include "levelLoader.h"
#include "Ease.h"
#include <cassert>
#include <sstream>
#include <iomanip>
TitleScene::TitleScene(SceneManager* controller, SceneObjects* sceneObj)
{
	controller_ = controller;
	sceneObj_ = sceneObj;

	targetPos.translation_ = { 0,0,0 };
	followPos.translation_ = { 0,0,-50 };

}

TitleScene::~TitleScene()
{
	delete player_;
	delete enemy_;

	sceneObj_->Reset();
	isTransition = false;
	//delete sceneObj_->skydomeO_;
	/*sceneObj_->Delete();*/
}

void TitleScene::Initialize()
{

	controller_->camera_->SetFollowerPos(&followPos);

	controller_->camera_->SetTargetPos(&targetPos);
	Sprite::LoadTexture(6, L"Resources/sprite/title.png");
	sprite_ = Sprite::Create(6, { 100,100 });

	//sceneObj_->transitionO_->SetRotation({ 0,0,0 });
	//sprite2_ = Sprite::Create(8,{200,200});

	player_ = new Player();
	enemy_ = new Enemy();

	//自機と敵の初期化
	player_->Initialize(controller_->dxCommon_,enemy_);
	enemy_->Initialize(controller_->dxCommon_,player_);

	//カメラの座標と注視点をセット
	controller_->camera_->SetEye({ player_->GetObject3d()->GetPosition().x,player_->GetObject3d()->GetPosition().y,player_->GetObject3d()->GetPosition().z -50 });
	controller_->camera_->SetTarget(enemy_->GetObject3d()->GetPosition());

	sceneObj_->transitionO_->SetScale({ 400,400,60 });
	sceneObj_->transitionO_->SetPosition({ transPos });

}

void TitleScene::Update(Input* input, GamePad* gamePad)
{
	gamePad->Update();
	//天球を回転させる処理
	skydomeRotate.y += rotateSpeed;
	sceneObj_->skydomeO_->SetRotation(skydomeRotate);

	sceneObj_->skydomeO_->Update();
	sceneObj_->transitionO_->Update();
	

	//player_->TitleAnime();
	player_->GetObject3d()->Update();
	enemy_->GetObject3d()->Update();

	/*player_->Update(input,gamePad);
	enemy_->Update();*/
	


	//ボタンを押したらシーン遷移を行う
	if (input->TriggerKey(DIK_SPACE) || gamePad->ButtonTrigger(X))
	{
		isTransition = true;
	}
	if ( easeTimer >= 120 )
	{
		ParamReset();
		controller_->ChangeSceneNum(S_SELECT);
	}
	if (isTransition == true)
	{
		SceneTransition();
	}
	controller_->camera_->Update();
}

void TitleScene::Draw()
{
#pragma region
	//// 3Dオブジェクト描画前処理
	Object3d::PreDraw(controller_->dxCommon_->GetCommandList());

	//// 3Dオブジェクトの描画

	/*fbxObject->Draw(dxCommon_->GetCommandList());*/

	sceneObj_->skydomeO_->Draw();

	if ( transScale_.x >= 0 )
	{
		enemy_->Draw();
		player_->Draw();
	}
	

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

	sprite_->Draw();
	//sprite2_->Draw();
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


	if (isTransition == true)
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

void TitleScene::ParamReset()
{
	controller_->camera_->SetFollowerPos(sceneObj_->player_->GetObject3d()->GetWorldTransformPtr());
	controller_->camera_->SetTargetPos(sceneObj_->enemy_->GetObject3d()->GetWorldTransformPtr());
	controller_->camera_->MoveCamera();
	player_->GetObject3d()->SetScale({ 1,1,1 });
	player_->GetObject3d()->SetPosition({ 0,0,-50 });
	sceneObj_->transitionO_->SetScale({1,1,1});
	sceneObj_->transitionO_->SetPosition({ 0,0,0 });

	transScale_ = { 1,1,1 };
	cameraRise = false;
}

void TitleScene::SceneTransition()
{
	//sceneObj_->transitionO_->worldTransform.scale_ += scale;
	//sceneObj_->transitionO_->Update();



	playerScale.x -= reduction;
	playerScale.y += expansion;
	player_->GetObject3d()->SetScale(playerScale);
	

	if ( player_->GetObject3d()->GetScale().x <= 0 )
	{
		player_->GetObject3d()->SetScale({ 0,0,0 });
		enemyScale.x -= reduction;
		enemyScale.y += expansion;
		enemy_->GetObject3d()->SetScale(enemyScale);
	}

	if ( enemy_->GetObject3d()->GetScale().x <= 0 )
	{
		enemy_->GetObject3d()->SetScale({ 0,0,0 });
		cameraRise = true;
	}

	if ( cameraRise == true )
	{
		easeTimer++;

		riseSpeed = 3.0f * ( float ) Ease::InOutQuint(change,0,120,easeTimer);

		controller_->camera_->eye_.y += riseSpeed;
		controller_->camera_->target_.y += riseSpeed;
		controller_->camera_->SetEye(controller_->camera_->eye_);
		controller_->camera_->SetTarget(controller_->camera_->target_);
		//controller_->camera_->Update();
	}
	else
	{
		easeTimer = 0;
		riseSpeed = 0;
	}

}
