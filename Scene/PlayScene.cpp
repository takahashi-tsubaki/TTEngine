#include "PlayScene.h"
#include "ImguiManager.h"
#include "Ease.h"
PlayScene::PlayScene(SceneManager* controller, SceneObjects* sceneObj)
{
	controller_ = controller;
	sceneObj_ = sceneObj;
	//sceneObj_->player_->Initialize(controller_->dxCommon_, sceneObj_->enemy_);
	//sceneObj_->enemy_->Initialize(controller_->dxCommon_, sceneObj_->player_);
	//sceneObj_->Initialize(controller_);
}

PlayScene::~PlayScene()
{
	sceneObj_->Reset();
	//sceneObj_->Delete();
	//delete player_;
	//delete enemy_;
}

void PlayScene::Initialize()
{
	

	//Sprite::LoadTexture(1, L"Resources/kuribo-.jpg");
	//Sprite::LoadTexture(2, L"Resources/mario.jpg");
	////HpSprite
	

	sprite_ = Sprite::Create(1, { WinApp::window_width,WinApp::window_height });
	enemyHpSprite_ = Sprite::Create(3, { 200,10 });
	enemyHpSprite_->Initialize();

	playerHpSprite_ = Sprite::Create(3,{100,600});
	playerHpSprite_->Initialize();

	alart = Sprite::Create(7, { 400,200 });
	alart->Initialize();

	player_ = sceneObj_->player_;
	enemy_ = sceneObj_->enemy_;
	player_->GetObject3d()->SetScale({1,1,1});
	enemy_->GetObject3d()->SetScale({ 1,1,1 });

	//controller_->camera_->SetFollowerPos(player_->GetObject3d()->GetWorldTransformPtr());

	//controller_->camera_->SetTargetPos(enemy_->GetObject3d()->GetWorldTransformPtr());

	//controller_->camera_->SetEye(StartPos);
	//controller_->camera_->Update();
}

void PlayScene::Update(Input* input, GamePad* gamePad)
{

	assert(input);
	gamePad->Update();


	//controller_->camera_->eye_.y = 50.0f;
	//controller_->camera_->SetEye(controller_->camera_->eye_);

	if ( isFight == false )
	{
		controller_->GetGameCamera()->Update();
		StartSign(input);

	}
	if ( isFight == true )
	{
			//シーンチェンジ
		if ( input->TriggerKey(DIK_RETURN) || gamePad->ButtonTrigger(X) )
		{

			player_->Reset();
			enemy_->Reset();
			controller_->ChangeSceneNum(S_TITLE);
		}

		//ポーズシーンへ
		if ( input->TriggerKey(DIK_TAB) || gamePad->ButtonTrigger(START) )
		{
			sceneObj_->player_ = player_;
			sceneObj_->enemy_ = enemy_;
			controller_->PushScene(S_PAUSE);
		}

		if ( input->TriggerKey(DIK_LSHIFT) || gamePad->ButtonTrigger(BACK) )
		{
			if ( enemy_->GetDebugMode() == false )
			{
				enemy_->SetDebugMode(true);
			}
			else
			{
				enemy_->SetDebugMode(false);
			}
		}

		player_->Update(input,gamePad);
		enemy_->Update();

		controller_->GetGameCamera()->SetFollowerPos(player_->GetObject3d()->GetWorldTransformPtr());

		controller_->GetGameCamera()->SetTargetPos(enemy_->GetObject3d()->GetWorldTransformPtr());
		controller_->GetGameCamera()->MoveCamera();
	}

	player_->GetObject3d()->Update();
	enemy_->GetObject3d()->Update();
	/*player_ = sceneObj_->player_;*/
	

	//if (isTransition == true)
	//{
	//	SceneTransition();
	//}
	

	//スプライトの大きさを体力に設定
	enemyHpSprite_->SetSize({ enemy_->GetHp() * 32.0f, 32.0f });
	playerHpSprite_->SetSize({ player_->GetHp() * 32.0f, 32.0f});


	//fbxObject->Update();

	/*ImGui::Begin("cameraPos");
	//ImGui::SetWindowPos({ 200 , 200 });
	ImGui::SetWindowSize({ 500,100 });
	ImGui::InputFloat3("eye", &camera_->eye_.x);
	ImGui::InputFloat3("target", &camera_->target_.x);
	ImGui::End();*/


	sceneObj_->skydomeO_->SetPosition({ sceneObj_->skydomeO_->GetPosition().x + player_->GetPosition().x,sceneObj_->skydomeO_->GetPosition().y,sceneObj_->skydomeO_->GetPosition().z });
	sceneObj_->skydomeO_->Update();
	sceneObj_->transitionO_->Update();
	//if (sceneObj_->transitionO_->worldTransform.scale_.x <= 0 || sceneObj_->transitionO_->worldTransform.scale_.z <= 0)
	//{
	//	isTransition = false;
	//}
	//if (isTransition == false)
	//{
	//	sceneObj_->transitionO_->worldTransform.scale_ = { 1,1,1 };
	//}

	//リセット処理
	if (input->TriggerKey(DIK_R))
	{
		player_->Reset();
		enemy_->Reset();

	}

	if (player_->GetHp() <= 0)
	{
		ResetParam();
		controller_->ChangeSceneNum(S_OVER);
	}

	else if (enemy_->GetHp() <= 0)
	{
		ResetParam();
		controller_->ChangeSceneNum(S_CLEAR);
	}



}

void PlayScene::Draw()
{
#pragma region
	//// 3Dオブジェクト描画前処理
	Object3d::PreDraw(controller_->dxCommon_->GetCommandList());

	//// 3Dオブジェクトの描画

	/*fbxObject->Draw(dxCommon_->GetCommandList());*/

	sceneObj_->skydomeO_->Draw();


	enemy_->Draw();
	player_->Draw();

	///// <summary>
	///// ここに3Dオブジェクトの描画処理を追加できる
	///// </summary>

	//// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion
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

	//sprite_->Draw();
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	playerHpSprite_->Draw();
	enemyHpSprite_->Draw();

	if (player_->GetVanishTimer() > 0)
	{
		alart->Draw();
	}
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void PlayScene::SceneTransition()
{
	sceneObj_->transitionO_->worldTransform.scale_ -= scale;
	sceneObj_->transitionO_->Update();
}

void PlayScene::StartSign(Input* input)
{
	Vector3 StartPos = { 2,50,50 };
	GoalPos = { player_->GetObject3d()->GetPosition().x,player_->GetObject3d()->GetPosition().y + 9 ,player_->GetObject3d()->GetPosition().z - 7 };
	cameraDis = StartPos  - GoalPos ;
	addSpeed =  2.0f * (float)Ease::OutCubic(change,0,120,startSignCount);

	//addSpeed *= -1;

	//cameraDis.nomalize();
	cameraDis *= addSpeed;

	cameraDis.y *= -1;
	cameraDis.z *= -1;

	cameraDis.y += 50.0f;//イージング開始の初期値をずらす


	controller_->GetGameCamera()->eye_ = cameraDis;
	controller_->GetGameCamera()->SetEye(controller_->camera_->eye_);
	controller_->GetGameCamera()->SetTarget(enemy_->GetObject3d()->GetPosition());

	startSignCount++;
	if ( startSignCount>=120 )
	{
		startSignCount = 120;
		if ( isReady == false )
		{
			if (input->TriggerKey(DIK_SPACE) )
			{
				isReady = true;
			}
		}
	
	}
	if ( isReady == true )
	{
		readyCount++;
		if ( readyCount >= 60 )
		{
			isFight = true;
			isReady = false;
			readyCount = 0;
		}
	}

	if ( isFight == true )
	{
		isStartSign = false;
		startSignCount = 0;
	}

}

void PlayScene::SetCamera()
{
	if ( isStartSign == true )
	{
		//controller_->camera_->Update();
	}
	else
	{

		controller_->GetGameCamera()->SetFollowerPos(player_->GetObject3d()->GetWorldTransformPtr());

		controller_->GetGameCamera()->SetTargetPos(enemy_->GetObject3d()->GetWorldTransformPtr());
		controller_->GetGameCamera()->MoveCamera();
	}
}

void PlayScene::ResetParam()
{
	player_->Reset();
	enemy_->Reset();
	sceneObj_->player_ = player_;
	sceneObj_->enemy_ = enemy_;
	controller_->GetGameCamera()->SetFollowerPos(player_->GetObject3d()->GetWorldTransformPtr());
	controller_->GetGameCamera()->SetTargetPos(enemy_->GetObject3d()->GetWorldTransformPtr());
}

