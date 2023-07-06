#include "gameScene.h"
#pragma warning(push)
#include "FbxLoader.h"
#include "FbxObject3d.h"

#include "ImguiManager.h"
#pragma warning(pop)

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete camera_ ;
	delete fbxObject;
	delete fbxModel;
	/*safe_delete(fbxObject);
	safe_delete(fbxModel);*/
}

void GameScene::Initalize(DirectXCommon* dxCommon, Input* input, GamePad* gamePad)
{
	dxCommon_ = dxCommon;
	input_ = input;
	//インスタンスの取得
	gamePad_ = gamePad;

	//input_ = Input::GetInstance();
	camera_ = new Camera(WinApp::window_width, WinApp::window_height);
	camera_->SetEye({0,0,-100});
	// カメラ注視点をセット
	camera_->SetTarget({ 0, 0, 0 });
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera_);
	//ライト生成
	light_ = Light::Create();
	//ライト色を設定
	light_->SetLightColor({ 1,1,1 });
	//3Dオブジェクトにライトをセット
	Object3d::SetLight(light_);

	//デバイスをセット
	FbxObject3d::SetDevice(dxCommon_->GetDevice());
	//カメラをセット
	FbxObject3d::SetCamera(camera_);
	//グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	object = Object3d::Create();
	model = Model::CreateFromOBJ("cube");
	object->SetModel(model);

	Sprite::LoadTexture(1, L"Resources/kuribo-.jpg");
	Sprite::LoadTexture(2, L"Resources/mario.jpg");

	sprite_ = Sprite::Create(1,{WinApp::window_width,WinApp::window_height});


	fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("boss_prot4");

	

	enemy_ = new Enemy();
	enemy_->Initialize(dxCommon_);
	
	player_ = new Player();
	player_->Initialize(dxCommon_, input_ , gamePad_, enemy_);

	fbxObject =  new FbxObject3d();
	fbxObject->Initialize();
	fbxObject->SetModel(fbxModel);
	//fbxObject->SetScale({0.1f,0.1f,0.1f});
	//fbxObject->SetPosition({ 0,-50,0 });

	fbxObject->SetPosition({0,-10,10});

	colMan = CollisionManager::GetInstance();
	
}

void GameScene::Update()
{

	camera_->eye_.y = 20.0f;

	Vector3 move;
	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= 0.5f;
	}
	if (input_->PushKey(DIK_RIGHT))
	{
		move.x += 0.5f;
	}
	if (input_->PushKey(DIK_UP))
	{
		move.z += 0.5f;
	}

	if (input_->PushKey(DIK_DOWN))
	{
		move.z -= 0.5f;
	}

	


	light_->Update();
	gamePad_->Update();

	fbxObject->Update();

	ImGui::Begin("cameraPos");
	ImGui::SetWindowPos({ 200 , 200 });
	ImGui::SetWindowSize({ 500,100 });
	ImGui::InputFloat3("isPause", &camera_->eye_.x);
	ImGui::InputFloat3("isPause", &camera_->target_.x);
	ImGui::End();

	object->Update();

	player_->Update();
	enemy_->Update();

	/*camera_->disEyeTarget(player_->GetPosition(),enemy_->GetPosition());*/
	camera_->MoveTarget(input_);
	camera_->Update();
	
	colMan->CheckAllCollisions();
}
void GameScene::Draw()
{
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommandList());
	// 背景スプライト描画
	
	
	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion

#pragma region 3Dオブジェクト描画
	//// 3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon_->GetCommandList());

	//// 3Dオブジェクトの描画

	/*fbxObject->Draw(dxCommon_->GetCommandList());
	object->Draw();*/
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
	Particle::PreDraw(dxCommon_->GetCommandList());

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
	Sprite::PreDraw(dxCommon_->GetCommandList());

	//sprite_->Draw();
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}


