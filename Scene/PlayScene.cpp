#include "PlayScene.h"
#include "ImguiManager.h"
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

	player_ = sceneObj_->player_;
	enemy_ = sceneObj_->enemy_;

	controller_->camera_->SetFollowerPos(player_->GetObject3d()->GetWorldTransformPtr());

	controller_->camera_->SetTargetPos(enemy_->GetObject3d()->GetWorldTransformPtr());


}

void PlayScene::Update(Input* input, GamePad* gamePad)
{

	assert(input);
	gamePad->Update();
	if (isTransition == false)
	{
		//シーンチェンジ
		if (input->TriggerKey(DIK_RETURN) || gamePad->ButtonTrigger(X))
		{
			player_->Reset();
			enemy_->Reset();
			controller_->ChangeSceneNum(S_TITLE);
		}

		if (input->TriggerKey(DIK_TAB) || gamePad->ButtonTrigger(START))
		{
			sceneObj_->player_ =player_;
			sceneObj_->enemy_ = enemy_;
			controller_->PushScene(S_PAUSE);
		}

		if (input->TriggerKey(DIK_LSHIFT) || gamePad->ButtonTrigger(BACK))
		{
			if (enemy_->GetDebugMode() == false)
			{
				enemy_->SetDebugMode(true);
			}
			else
			{
				enemy_->SetDebugMode(false);
			}
		}
		
		player_->Update(input, gamePad);
		enemy_->Update();
	}
	player_ = sceneObj_->player_;
	

	if (isTransition == true)
	{
		SceneTransition();
	}
	

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

	if (sceneObj_->transitionO_->worldTransform.scale_.x <= 0 || sceneObj_->transitionO_->worldTransform.scale_.z <= 0)
	{
		isTransition = false;
	}
	if (isTransition == false)
	{
		sceneObj_->transitionO_->worldTransform.scale_ = { 1,1,1 };
	}

	//リセット処理
	if (input->TriggerKey(DIK_R))
	{
		player_->Reset();
		enemy_->Reset();

	}

	if (player_->GetHp() <= 0)
	{
		controller_->ChangeSceneNum(S_OVER);
		player_->Reset();
		enemy_->Reset();
	}

	else if (enemy_->GetHp() <= 0)
	{
		controller_->ChangeSceneNum(S_CLEAR);
		player_->Reset();
		enemy_->Reset();
	}


	controller_->camera_->Update();
}

void PlayScene::Draw()
{
#pragma region 3Dオブジェクト描画
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
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(controller_->dxCommon_->GetCommandList());
	// 背景スプライト描画


	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion

#pragma region 3Dオブジェクト描画
	//// 3Dオブジェクト描画前処理
	Object3d::PreDraw(controller_->dxCommon_->GetCommandList());

	//// 3Dオブジェクトの描画

	/*fbxObject->Draw(dxCommon_->GetCommandList());*/

	if (isTransition == true)
	{
		sceneObj_->transitionO_->Draw();
	}

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
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(controller_->dxCommon_->GetCommandList());

	//sprite_->Draw();
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	playerHpSprite_->Draw();
	enemyHpSprite_->Draw();
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

