#include "gameScene.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete camera_ ;
}

void GameScene::Initalize()
{
	//インスタンスの取得
	input_ = Input::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();

	//input_ = Input::GetInstance();
	camera_ = new Camera(WinApp::window_width, WinApp::window_height);
	// カメラ注視点をセット
	camera_->SetTarget({ 0, 1, 0 });
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera_);
	//ライト生成
	light_ = Light::Create();
	//ライト色を設定
	light_->SetLightColor({ 1,1,1 });
	//3Dオブジェクトにライトをセット
	Object3d::SetLight(light_);

	Particle::SetCamera(camera_);

	Sprite::LoadTexture(1, L"Resources/kuribo-.jpg");
	Sprite::LoadTexture(2, L"Resources/mario.jpg");

	//クリボー
	sprite_ = Sprite::Create(1, { 0.0f,0.0f });
	sprite_->SetSize({128.0f,128.0f });

	//マリオ
	sprite2_ = Sprite::Create(2, { 300.0f,0.0f });
	sprite2_->SetSize({207.0f/2,205.0f/2});

	modelSkydome_ = Model::CreateFromOBJ("skydome");
	objSkydome_ = Object3d::Create();
	
	objSkydome_->SetModel(modelSkydome_);
	objSkydome_->SetColor({ 1,1,1,0.7f });

	modelPlayer_ = Model::CreateFromOBJ("player");
	objPlayer_ = Object3d::Create();

	objPlayer_->SetModel(modelPlayer_);

	modelParticle_ = Model::CreateFromOBJ("enemy");
	particle_ = Particle::Create();

	particle_->SetModel(modelParticle_);
	particle_->SetPosition({ objPlayer_->position.x+1 ,objPlayer_->position .y,objPlayer_->position.z});
	particle_->SetColor({ 1,1,1,0.7f });


	modelParticle2_ = Model::CreateFromOBJ("enemy2");
	particle2_ = Particle::Create();

	particle2_->SetModel(modelParticle2_);
	particle2_->SetPosition({ objPlayer_->position.x - 1 ,objPlayer_->position.y,objPlayer_->position.z });
	particle2_->SetColor({ 1,1,1,0.7f });

}

void GameScene::Update()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		if (isParticle == false)
		{
			isParticle = true;
		}
		else
		{
			isParticle = false;
		}
	}

	if (input_->PushKey(DIK_W))
	{
		camera_->eye.z += 0.2f;
		camera_->target.z += 0.2f;
		camera_->SetEye(camera_->eye);
	}
	if (input_->PushKey(DIK_S))
	{
		camera_->eye.z -= 0.2f;
		camera_->target.z -= 0.2f;
		camera_->SetEye(camera_->eye);
	}
	camera_->Update();


	objSkydome_->Update();
	objPlayer_->Update();
	particle_->Update();
	particle2_->Update();
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
	objSkydome_->Draw();


	objPlayer_->Draw();
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
	/*objEnemy_->Draw();*/
	if (isParticle == true)
	{
		particle_->Draw();
		particle2_->Draw();
	}
	
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
	sprite_->Draw();
	sprite2_->Draw();
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}


