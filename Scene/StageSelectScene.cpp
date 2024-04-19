#include "StageSelectScene.h"
#include "Ease.h"
StageSelectScene::StageSelectScene(SceneManager* controller, SceneObjects* sceneObj)
{
	controller_ = controller;
	sceneObj_ = sceneObj;

	targetPos.translation_ = { 0,0,0 };
	followPos.translation_ = { 0,0,-50 };
}

StageSelectScene::~StageSelectScene()
{

	//sceneObj_->Reset();
	isTransition = false;
	delete audio;
}

void StageSelectScene::Initialize()
{
	tutorialSP_ = Sprite::Create(SpriteNumber::TUTORIALTEXT,{ 400, 425 });
	tutorialSP_->Initialize();

	battleSP_ = Sprite::Create(SpriteNumber::BATTLETEXT,{ 375, 425 });
	battleSP_->Initialize();


	player_ = sceneObj_->player_;
	enemy_ = sceneObj_->enemy_;

	controller_->camera_->SetFollowerPos(&followPos);

	controller_->camera_->SetTargetPos(&targetPos);

	sceneObj_->transitionO_->SetScale({ 400,400,60 });
	sceneObj_->transitionO_->SetPosition({ transPos });

	currentSceneNum_ = CurrentSelectScene::Tutorial;

	// 音声データの初期化と読み取り
	audio = new TTEngine::Audio();
	audio->Initialize();

	audio->LoadWave("select.wav");
}

void StageSelectScene::Update(Input* input, GamePad* gamePad)
{
	gamePad->Update();
	//天球を回転させる処理
	sceneObj_->selectSkydomeO_->Update();
	sceneObj_->transitionO_->Update();


	player_->GetObject3d()->Update();
	//enemy_->GetObject3d()->Update();
	//ボタンを押したらシーン遷移を行う
	if (input->TriggerKey(DIK_SPACE) || gamePad->ButtonTrigger(A))
	{

		isTransition = true;
		/*controller_->ChangeSceneNum(S_PLAY);*/

	}
	if ( input->TriggerKey(DIK_TAB) || gamePad->ButtonTrigger(B) )
	{
		controller_->ChangeSceneNum(S_TITLE);
	}
	if ( easeTimer >= 120 )
	{
		
		if ( currentSceneNum_ == CurrentSelectScene::Tutorial )
		{
			ParamReset();
			pSourceVoice[ 0 ] = audio->PlayWave("select.wav");
	/*		pSourceVoice[ 0 ]->SetVolume(0.05f);*/
			controller_->ChangeSceneNum(S_TUTORIAL);
		}
		if ( currentSceneNum_ == CurrentSelectScene::Battle )
		{
			ParamReset();
			pSourceVoice[0] = audio->PlayWave("select.wav");
			//pSourceVoice[ 0 ]->SetVolume(0.05f);
			controller_->ChangeSceneNum(S_PLAY);
		}

	}
	ChangeNextScene(input,gamePad);
	

	if ( isTransition == true )
	{
		SceneTransition();
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

	sceneObj_->selectSkydomeO_->Draw();

	//enemy_->GetObject3d()->Draw();
	player_->GetObject3d()->Draw();

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

	if ( isTransition == false )
	{
		sceneObj_->spaceButton_->Draw();
		sceneObj_->selectSp_->Draw();


		if ( currentSceneNum_ == CurrentSelectScene::Tutorial )
		{
			tutorialSP_->Draw();
		}
		if ( currentSceneNum_ == CurrentSelectScene::Battle )
		{
			battleSP_->Draw();
		}
	}



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


	//sprite_->Draw();
	if ( isTransition == true )
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
	playerScale.x -= reduction;
	playerScale.y += expansion;
	player_->GetObject3d()->SetScale(playerScale);


	if ( player_->GetObject3d()->GetScale().x <= 0 )
	{
		player_->GetObject3d()->SetScale({ 0,0,0 });
		enemyScale.x -= reduction;
		enemyScale.y += expansion;
		cameraDescent = true;
		//enemy_->GetObject3d()->SetScale(enemyScale);
	}

	//if ( enemy_->GetObject3d()->GetScale().x <= 0 )
	//{
	//	//enemy_->GetObject3d()->SetScale({ 0,0,0 });

	//}

	if ( cameraDescent == true )
	{
		easeTimer++;

		descentSpeed = 3.0f * ( float ) Ease::InOutQuint(change,0,120,easeTimer);

		controller_->camera_->eye_.y -= descentSpeed;
		controller_->camera_->target_.y -= descentSpeed;
		controller_->camera_->SetEye(controller_->camera_->eye_);
		controller_->camera_->SetTarget(controller_->camera_->target_);
		//controller_->camera_->Update();
	}
	else
	{
		easeTimer = 0;
		descentSpeed = 0;
	}
}

void StageSelectScene::ParamReset()
{
	//カメラの座標と注視点をセット
	//controller_->camera_->SetFollowerPos(player_->GetObject3d()->GetWorldTransformPtr());

	//controller_->camera_->SetTargetPos(enemy_->GetObject3d()->GetWorldTransformPtr());

	//controller_->camera_->MoveCamera();
	//enemy_->GetObject3d()->SetScale({ 1,1,1 });
	player_->GetObject3d()->SetScale({ 1,1,1 });

	sceneObj_->player_->GetObject3d()->SetScale({1,1,1});
	//sceneObj_->enemy_->GetObject3d()->SetScale({ 1,1,1 });

	player_->GetObject3d()->SetPosition({ 0,0,-50 });

	sceneObj_->transitionO_->SetScale({400, 400, 60});
	sceneObj_->transitionO_->SetPosition({transPos});
	sceneObj_->transitionO_->Update();


	transScale_ = { 1,1,1 };
}

void StageSelectScene::ChangeNextScene(Input* input,GamePad* gamePad)
{
	if ( currentSceneNum_ == CurrentSelectScene::Tutorial )
	{
		if ( input->TriggerKey(DIK_D) || input->TriggerKey(DIK_RIGHT) || gamePad->StickTrigger(L_RIGHT) )
		{
			currentSceneNum_ = CurrentSelectScene::Battle;
		}
	}
	if ( currentSceneNum_ == CurrentSelectScene::Battle )
	{
		if ( input->TriggerKey(DIK_A) || input->TriggerKey(DIK_LEFT) || gamePad->StickTrigger(L_LEFT) )
		{
			currentSceneNum_ = CurrentSelectScene::Tutorial;
		}
	}
}
