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
	camera_->SetEye({ 0,40.0f,-200 });

	gameCamera = new GameCamera(WinApp::window_width, WinApp::window_height,input);
	assert(gameCamera);
	gameCamera->SetEye({ 0,40.0f,-2000 });
	/*gameCamera->SetTarget({ 0 , 0 , 0 });*/
	
	
	//// カメラ注視点をセット
	//camera_->SetTarget({ 0, 0, 0 });
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(gameCamera);
	//ライト生成
	light_ = Light::Create();
	//ライト色を設定
	light_->SetLightColor({ 1,1,1 });
	//3Dオブジェクトにライトをセット
	Object3d::SetLight(light_);

	//デバイスをセット
	FbxObject3d::SetDevice(dxCommon_->GetDevice());
	//カメラをセット
	FbxObject3d::SetCamera(gameCamera);
	//グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	skydomeO_ = Object3d::Create();
	skydomeM_ = Model::CreateFromOBJ("skydome");
	skydomeO_->SetModel(skydomeM_);

	skydomeO_->SetScale({2,2,2});


	Sprite::LoadTexture(1, L"Resources/kuribo-.jpg");
	Sprite::LoadTexture(2, L"Resources/mario.jpg");

	sprite_ = Sprite::Create(1,{WinApp::window_width,WinApp::window_height});


	fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("boss_prot4");

	
	player_ = new Player();
	enemy_ = new Enemy();
	enemy_->Initialize(dxCommon_,player_);
	
	
	player_->Initialize(dxCommon_, input_ , gamePad_, enemy_);

	fbxObject =  new FbxObject3d();
	fbxObject->Initialize();
	fbxObject->SetModel(fbxModel);
	//fbxObject->SetScale({0.1f,0.1f,0.1f});
	//fbxObject->SetPosition({ 0,-50,0 });

	fbxObject->SetPosition({0,-10,10});

	colMan = CollisionManager::GetInstance();
	
	

	gameCamera->SetFollowerPos(player_->GetObject3d()->GetWorldTransformPtr());

	gameCamera->SetTargetPos(enemy_->GetObject3d()->GetWorldTransformPtr());
}

void GameScene::Update()
{
	Vector3 nowEye = gameCamera->GetEye();
	gameCamera->SetTarget(player_->wtf.translation_);
	//Vector3 move;
	//if (input_->PushKey(DIK_LEFT))
	//{
	//	move.x -= 0.5f;
	//}
	//if (input_->PushKey(DIK_RIGHT))
	//{
	//	move.x += 0.5f;
	//}
	//if (input_->PushKey(DIK_UP))
	//{
	//	move.z += 0.5f;
	//}

	//if (input_->PushKey(DIK_DOWN))
	//{
	//	move.z -= 0.5f;
	//}

	/*gameCamera->SetTarget(player_->GetObject3d()->GetPosition());*/
	

	light_->Update();
	gamePad_->Update();

	fbxObject->Update();


	ImGui::Begin("Camera");

	ImGui::SliderFloat("eye:x", &nowEye.x, -400.0f, 400.0f);
	ImGui::SliderFloat("eye:xz", &nowEye.z, -400.0f, 400.0f);

	ImGui::End();

	gameCamera->Update();

	/*ImGui::Begin("cameraPos");
	ImGui::SetWindowPos({ 200 , 200 });
	ImGui::SetWindowSize({ 500,100 });
	ImGui::InputFloat3("eye", &camera_->eye_.x);
	ImGui::InputFloat3("target", &camera_->target_.x);
	ImGui::End();*/

	skydomeO_->Update();

	player_->Update();
	enemy_->Update();

	gameCamera->Update();

	/*camera_->disEyeTarget(player_->GetPosition(),enemy_->GetPosition());*/

	/*camera_->SetEye({ player_->wtf.translation_.x, player_->wtf.translation_.y+20, player_->wtf.translation_.z-50});*/
	/*camera_->SetTarget((enemy_->wtf.translation_ - player_->wtf.translation_)/2);*/

	//camera_->MoveTarget(input_);
	/*camera_->Update();*/
	
	//当たり判定
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


