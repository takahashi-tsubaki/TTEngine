#include "PlayScene.h"
#include "ImguiManager.h"
PlayScene::PlayScene(SceneManager* controller)
{
	controller_ = controller;
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	skydomeO_ = Object3d::Create();
	skydomeM_ = Model::CreateFromOBJ("skydome");
	skydomeO_->SetModel(skydomeM_);

	skydomeO_->SetScale({ 2,2,2 });


	Sprite::LoadTexture(1, L"Resources/kuribo-.jpg");
	Sprite::LoadTexture(2, L"Resources/mario.jpg");

	sprite_ = Sprite::Create(1, { WinApp::window_width,WinApp::window_height });


	fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("boss_prot4");


	player_ = new Player();
	enemy_ = new Enemy();
	enemy_->Initialize(controller_->dxCommon_, player_);


	player_->Initialize(controller_->dxCommon_, controller_->input_, controller_->gamePad_, enemy_);

	fbxObject = new FbxObject3d();
	fbxObject->Initialize();
	fbxObject->SetModel(fbxModel);
	//fbxObject->SetScale({0.1f,0.1f,0.1f});
	//fbxObject->SetPosition({ 0,-50,0 });

	fbxObject->SetPosition({ 0,-10,10 });

	controller_->camera_->SetFollowerPos(player_->GetObject3d()->GetWorldTransformPtr());

	controller_->camera_->SetTargetPos(enemy_->GetObject3d()->GetWorldTransformPtr());
}

void PlayScene::Update(Input* input)
{
	Vector3 nowEye = controller_->camera_->GetEye();

	controller_->gamePad_->Update();

	fbxObject->Update();


	ImGui::Begin("Camera");

	ImGui::SliderFloat("eye:x", &nowEye.x, -400.0f, 400.0f);
	ImGui::SliderFloat("eye:xz", &nowEye.z, -400.0f, 400.0f);

	ImGui::End();

	controller_->camera_->Update();

	/*ImGui::Begin("cameraPos");
	ImGui::SetWindowPos({ 200 , 200 });
	ImGui::SetWindowSize({ 500,100 });
	ImGui::InputFloat3("eye", &camera_->eye_.x);
	ImGui::InputFloat3("target", &camera_->target_.x);
	ImGui::End();*/

	skydomeO_->Update();

	player_->Update();
	enemy_->Update();
	if (input->TriggerKey(DIK_RETURN))
	{
		controller_->ChangeSceneNum(S_TITLE);
	}
}

void PlayScene::Draw()
{
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

	skydomeO_->Draw();

	player_->Draw();
	enemy_->Draw();

	///// <summary>
	///// ここに3Dオブジェクトの描画処理を追加できる
	///// </summary>

	//// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

#pragma region パーティクル描画

	//// パーティクル描画前処理
	Particle::PreDraw(controller_->dxCommon_->GetCommandList());

	//// 3Dオブジェクトの描画

	///// <summary>
	///// ここに3Dオブジェクトの描画処理を追加できる
	///// </summary>

	//// パーティクル描画後処理
	Particle::PostDraw();

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

	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
