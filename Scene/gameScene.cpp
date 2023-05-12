#include "gameScene.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"


GameScene::GameScene() {}

GameScene::~GameScene() {
	delete camera_ ;
	delete fbxObject;
	delete fbxModel;
	/*safe_delete(fbxObject);
	safe_delete(fbxModel);*/
}

void GameScene::Initalize()
{
	//インスタンスの取得
	input_ = Input::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();

	//input_ = Input::GetInstance();
	camera_ = new Camera(WinApp::window_width, WinApp::window_height);
	camera_->SetEye({0,0,-100});
	// カメラ注視点をセット
	camera_->SetTarget({ 0, 20, 0 });
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

	Sprite::LoadTexture(1, L"Resources/kuribo-.jpg");
	Sprite::LoadTexture(2, L"Resources/mario.jpg");

	fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("cube");

	

	fbxObject =  new FbxObject3d();
	fbxObject->Initialize();
	fbxObject->SetModel(fbxModel);

	fbxObject->SetPosition({0,-30,100});
}

void GameScene::Update()
{
	fbxObject->Update();
	camera_->Update();
	light_->Update();
	
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

	fbxObject->Draw(dxCommon_->GetCommandList());

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

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommandList());

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}


