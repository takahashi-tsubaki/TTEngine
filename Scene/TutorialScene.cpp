#include "TutorialScene.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "Ease.h"

TutorialScene::TutorialScene(SceneManager* controller, SceneObjects* sceneObj)
{
	controller_ = controller;
	sceneObj_ = sceneObj;
}

TutorialScene::~TutorialScene()
{
	ResetParam();
}

void TutorialScene::Initialize()
{
	enemyHpSprite_ = Sprite::Create(SpriteNumber::HPBAR, { 200,10 }, { 1,1,1,1 }, { 0.0f,0.5f });
	enemyHpSprite_->Initialize();

	playerHpSprite_ = Sprite::Create(SpriteNumber::HPBAR, { 100, 600 }, { 1, 1, 1, 1 }, { 0.0f, 0.5f });
	playerHpSprite_->Initialize();
	//Fightの文字のスプライト
	isFightSP_ = Sprite::Create(SpriteNumber::FIGHT, { WinApp::window_width / 2, WinApp::window_height / 2 }, { 1, 1, 1, 1 },
		{ 0.5f, 0.5f });
	isFightSP_->Initialize();

	//敵を倒せ　のスプライト
	startSp_ = Sprite::Create(SpriteNumber::STARTTUTORIAL, { WinApp::window_width / 2, WinApp::window_height / 2 }, { 1, 1, 1, 1 },
		{ 0.5f, 0.5f });
	startSp_->Initialize();

	//シーン遷移時のスプライト
	transSP_ = Sprite::Create(SpriteNumber::SCENETRANS, { WinApp::window_width / 2, WinApp::window_height / 2 }, { 1, 1, 1, 1 },
		{ 0.5f, 0.5f });
	transSP_->Initialize();

	//Hp減少バーのスプライト
	damageSP_ = Sprite::Create(SpriteNumber::DAMAGEBAR, { 1160, 10 });
	damageSP_->SetIsFlipX(true);

	finishSP_ = Sprite::Create(SpriteNumber::FINISH, finishSpPos);
	finishSP_->Initialize();


	pauseSP_ = Sprite::Create(SpriteNumber::PAUSE, { 30, 30 });
	pauseSP_->Initialize();

	sousaSP_ = Sprite::Create(SpriteNumber::SOUSA, { 960,300 });
	sousaSP_->Initialize();

	player = new PlayerCharacter();
	enemy = new EnemyCharacter();

	player->Initialize(controller_->dxCommon_, { 0, 0, -50 }, enemy, sceneObj_);
	enemy->Initialize(controller_->dxCommon_, { 0, 0, 0 }, player,sceneObj_);
	player->SetAttribute();
	enemy->SetAttribute();

	// 音声データの初期化と読み取り
	audio = new TTEngine::Audio();
	audio->Initialize();

	audio->LoadWave("oto.wav");


	fieldModel_ = Model::CreateFromOBJ("cube6");
	fieldObj_ = Object3d::Create();
	fieldObj_->SetModel(fieldModel_);

}

void TutorialScene::Update(Input* input, GamePad* gamePad)
{
	assert(input);
	gamePad->Update();

	// 音声再生
	if (soundCheckFlag == 0) {
		// 音声再生

		pSourceVoice[0] = audio->PlayWave("oto.wav");
		pSourceVoice[0]->SetVolume(0.05f);
		soundCheckFlag = 1;
	}

	if (isFight == false)
	{
		controller_->GetGameCamera()->Update();
		StartSign(input, gamePad);

	}
#pragma region 戦闘中
	if (isFight == true)
	{

		//ポーズシーンへ
		if (input->TriggerKey(DIK_TAB) || gamePad->ButtonTrigger(START))
		{
			sceneObj_->player = player;
			sceneObj_->enemy = enemy;
			audio->StopWave(pSourceVoice[0]);
			soundCheckFlag = 0;
			controller_->PushScene(S_PAUSE);
		}

		if (player->GetHp() <= 1 )
		{
			player->SetHp(10);
		}
		if (enemy->GetHp() <= 1)
		{
			enemy->SetHp(30);
			enemy->SetDamageSize(0);
			enemy->SetAttribute();
		}



		player->Update(input, gamePad);
		enemy->Update();


		//継承したほうのやつ

		//enemy->Update();
		controller_->GetGameCamera()->SetFollowerPos(
			player->GetFbxObject3d()->GetWorldTransformPtr()); // カメラの座標の設定

		controller_->GetGameCamera()->SetTargetPos(
			enemy->GetFbxObject3d()->GetWorldTransformPtr());//カメラの注視点の設定

		controller_->GetGameCamera()->MoveCamera();
		
		isStartSign = false;
		fightSpCount++;
		startSignCount = 0;

		if (fightSpCount > Number::Twenty)
		{
			isFightSP_->SetColor({ 1,1,1,SpAlpha });
			SpAlpha -= decreaseAlpha;
			isFightSP_->SetSize({ SpSize * 320.0f,128.0f });

			if (SpSize < 2.0f)
			{
				SpSize += addSize;
			}
		}

	}


	player->GetFbxObject3d()->Update(); //この2つはCharacterクラスを継承したほう
	enemy->GetFbxObject3d()->Update();

	BulletManager::GetInstance()->Update();

	//Player_->Update();
	//Enemy_->Update();

		//スプライトの大きさを設定
	enemyHpSprite_->SetSize({ enemy->GetHp() * 32.0f, hpSpSize * 32.0f });
	playerHpSprite_->SetSize({ player->GetHp() * 32.0f, hpSpSize * 32.0f });
	damageSP_->SetSize({ enemy->GetDamageSize() * 32.0f, hpSpSize * 32.0f });
	startSp_->SetSize({ 1280.0f,startSpSize * 256.0f });
	transSP_->SetSize({ (float)Size::TenTimes * 275.0f,(float)Size::TenTimes * 183.0f });

#pragma endregion
	sceneObj_->skydomeO_->SetPosition({ sceneObj_->skydomeO_->GetPosition().x,sceneObj_->skydomeO_->GetPosition().y,sceneObj_->skydomeO_->GetPosition().z });
	sceneObj_->skydomeO_->Update();
	sceneObj_->transitionO_->Update();
	fieldObj_->Update();
}

void TutorialScene::Draw()
{
#pragma region

	//// 3Dオブジェクト描画前処理
	Object3d::PreDraw(controller_->dxCommon_->GetCommandList());

	//// 3Dオブジェクトの描画


	sceneObj_->skydomeO_->Draw();
	fieldObj_->Draw();

	player->Draw();

	enemy->Draw();

	BulletManager::GetInstance()->Draw();


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


	//BulletManager::GetInstance()->ParticleDraw(controller_->dxCommon_->GetCommandList());
	if (isFinish == false)
	{
		player->ParticleDraw(controller_->dxCommon_->GetCommandList());
		enemy->GetParticle()->Draw(controller_->dxCommon_->GetCommandList());
	}

	///// <summary>
	///// ここに3Dオブジェクトの描画処理を追加できる
	///// </summary>

	//// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

#pragma region
	// 前景スプライト描画前処理
	Sprite::PreDraw(controller_->dxCommon_->GetCommandList());

	//sprite_->Draw();
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	///

	if (isFinish == false)
	{
		playerHpSprite_->Draw();
		enemyHpSprite_->Draw();

		damageSP_->Draw();

	}

	if (isFight == true)
	{
		isFightSP_->Draw();
		pauseSP_->Draw();
		sousaSP_->Draw();
	}
	if (startSignCount >= Number::HundredTwenty && startSpSize > 0.0f)
	{
		startSp_->Draw();
	}
	finishSP_->Draw();

	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
#pragma region
	//// 3Dオブジェクト描画前処理
	Object3d::PreDraw(controller_->dxCommon_->GetCommandList());


	//// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion
}

void TutorialScene::StartSign(Input* input, GamePad* gamepad)
{
	Vector3 StartPos = { 2,50,50 };

	const float shiftPos = 50.0f;
	GoalPos = {
		player->GetFbxObject3d()->GetPosition().x, player->GetFbxObject3d()->GetPosition().y + 9,
		player->GetFbxObject3d()->GetPosition().z - 7 };
	cameraDis = StartPos - GoalPos;
	addSpeed = 2.0f * (float)Ease::OutCubic(change, 0, 120, startSignCount);

	//addSpeed *= -1;

	//cameraDis.nomalize();
	cameraDis *= addSpeed;

	cameraDis.y *= -1;
	cameraDis.z *= -1;

	cameraDis.y += shiftPos; // イージング開始の初期値をずらす

	controller_->GetGameCamera()->GetEye() = cameraDis;
	controller_->GetGameCamera()->SetEye(controller_->camera_->eye_);
	controller_->GetGameCamera()->SetTarget(enemy->GetFbxObject3d()->GetPosition());

	startSignCount++;
	if (startSignCount >= Number::HundredTwenty)
	{
		startSignCount = Number::HundredTwenty;

		if (isReady == false)
		{
			if (startSpSize <= 1.0f)
			{
				startSpSize += addSize * 2.0f;
			}
			if (input->TriggerKey(DIK_SPACE) || gamepad->ButtonInput(A))
			{

				isReady = true;
			}
		}

	}
	if (isReady == true)
	{
		if (startSpSize >= 0.0f)
		{
			startSpSize -= decSize;
		}

		readyCount++;

		if (readyCount >= Number::Thirty && hpSpSize <= 1.0f)
		{
			hpSpSize += addHpSize;
		}

		if (readyCount >= Number::Sixty)
		{
			isFight = true;
			isReady = false;
			readyCount = 0;

		}
	}
}

void TutorialScene::ResetParam()
{
	player->Reset();
	enemy->Reset();
	controller_->GetGameCamera()->SetFollowerPos(player->GetFbxObject3d()->GetWorldTransformPtr());
	controller_->GetGameCamera()->SetTargetPos(enemy->GetFbxObject3d()->GetWorldTransformPtr());

	isStartSign = true;
	isReady = false;
	isFight = false;

	startSignCount = 0;
	readyCount = 0;
	fightSpCount = 0;//スプライト用のカウント

	SpAlpha = 1.0f;
	decreaseAlpha = 0.1f;

	addSpeed = 0.1f;
	SpSize = 1.0f;
	addSize = 0.05f;
	decSize = 0.1f;

	startSpSize = 0.0f;
	hpSpSize = 0.0f;
	addHpSize = 0.1f;


	finishSpPos = { WinApp::window_width, WinApp::window_height / 2 };
	isFinishSpCount = 0; // FIHISHSP_が画面に描画される時間

	addfinishSpeed = 0.1f;
	addRotation = { 0, 0, 0 };

	finishCameraPlayerPos = { 0, 0, 0 };
	finishCameraPlayerTarget = { 0, 0, 0 };

	finishCameraEnemyPos = { 0, 0, 0 };
	finishCameraEnemyTarget = { 0, 0, 0 };

	transObjAlpha = 0.0f;
	addAlpha = 0.018f;
	isFinish = false;
}
