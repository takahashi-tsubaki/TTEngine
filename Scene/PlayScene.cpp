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
	//sceneObj_->Reset();
	//sceneObj_->Delete();
	//delete player_;
	//delete enemy_;

	delete alart;
	delete startSp_;
	delete isFightSP_;
	delete damageSP_;

	ResetParam();

}

void PlayScene::Initialize()
{
	
#pragma region スプライト関連の初期化

	enemyHpSprite_ = Sprite::Create(SpriteNumber::HPBAR,{ 200,10 },{ 1,1,1,1 },{0.0f,0.5f});
	enemyHpSprite_->Initialize();

	playerHpSprite_ = Sprite::Create(SpriteNumber::HPBAR, {100, 600}, {1, 1, 1, 1}, {0.0f, 0.5f});
	playerHpSprite_->Initialize();

	alart = Sprite::Create(SpriteNumber::ALART, {400, 200});
	alart->Initialize();

	//Fightの文字のスプライト
	isFightSP_ = Sprite::Create(SpriteNumber::FIGHT, {WinApp::window_width / 2, WinApp::window_height / 2}, {1, 1, 1, 1},
	    {0.5f, 0.5f});
	isFightSP_->Initialize();

	//敵を倒せ　のスプライト
	startSp_ = Sprite::Create(SpriteNumber::STARTSIGN, {WinApp::window_width / 2, WinApp::window_height / 2}, {1, 1, 1, 1},
	    {0.5f, 0.5f});
	startSp_->Initialize();

	//シーン遷移時のスプライト
	transSP_ = Sprite::Create(SpriteNumber::SCENETRANS, {WinApp::window_width / 2, WinApp::window_height / 2}, {1, 1, 1, 1},
	    {0.5f, 0.5f});
	transSP_->Initialize();

	//Hp減少バーのスプライト
	damageSP_ = Sprite::Create(SpriteNumber::DAMAGEBAR, {1160, 10});
	damageSP_->SetIsFlipX(true);

	finishSP_ = Sprite::Create(SpriteNumber::FINISH, finishSpPos);
	finishSP_->Initialize();

	pauseSP_ = Sprite::Create(SpriteNumber::PAUSE, {30, 30});
	pauseSP_->Initialize();

	sousaSP_ = Sprite::Create(SpriteNumber::SOUSA,{960,300});
	sousaSP_->Initialize();
	sceneObj_->fieldRock_->LoadFile("field");

#pragma endregion

	player_ = sceneObj_->player_;
	enemy_ = sceneObj_->enemy_;


	player = sceneObj_->player;
	enemy = sceneObj_->enemy;
	//Player_ = sceneObj_->Player_;
	//Enemy_ = sceneObj_->Enemy_;

	player_->GetObject3d()->SetScale({1,1,1});
	enemy_->GetObject3d()->SetScale({ 1,1,1 });

	RockO_ = Object3d::Create();
	RockM_ = Model::CreateFromOBJ("bume");
	RockO_->SetModel(RockM_);
	RockO_->SetPosition({-50,0,-50});

	addRotation = {45, 0, 0};

	addfinishSpeed = 30.0f;
	//controller_->camera_->SetFollowerPos(player_->GetObject3d()->GetWorldTransformPtr());

	//controller_->camera_->SetTargetPos(enemy_->GetObject3d()->GetWorldTransformPtr());

	//controller_->camera_->SetEye(StartPos);
	//controller_->camera_->Update();

	sceneObj_->transitionO_->SetScale({100,100,1});




	// 音声データの初期化と読み取り
	audio = new TTEngine::Audio();
	audio->Initialize();

	audio->LoadWave("oto.wav");
}

void PlayScene::Update(Input* input, GamePad* gamePad)
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




	//プレイヤー(エネミーも)座標でのカメラの計算
	if ( isFinish == false )
	{
		finishPlayerCamera();
		finishEnemyCamera();
	}

	//controller_->camera_->eye_.y = 50.0f;
	//controller_->camera_->SetEye(controller_->camera_->eye_);

	if ( isFight == false )
	{
		controller_->GetGameCamera()->Update();
		StartSign(input,gamePad);

	}
#pragma region 戦闘中
	if ( isFight == true )
	{
			//シーンチェンジ
		if ( input->TriggerKey(DIK_RETURN) || gamePad->ButtonTrigger(X) )
		{

			player_->Reset();
			enemy_->Reset();
			ResetParam();
			controller_->ChangeSceneNum(S_SELECT);
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


		if ( player_->GetHp() >= 0 || enemy_->GetHp() >= 0 )
		{

			player_->Update(input, gamePad);
			enemy_->Update();
			if (player_->GetVanish() == true) {
				controller_->GetGameCamera()->SetEye(player_->GetOldPos());
				//カメラ補間
				controller_->GetGameCamera()->eye_.lerp(controller_->GetGameCamera()->eye_, player_->GetFbxObject3d()->GetPosition(),30.0f);
			}
			if (enemy_->GetVanish() == true) {
				controller_->GetGameCamera()->SetEye(enemy_->GetOldPos());
				// カメラ補間
				controller_->GetGameCamera()->eye_.lerp(controller_->GetGameCamera()->eye_, enemy_->GetPosition(), 10.0f);
			}
			// カメラ補間
			controller_->GetGameCamera()->eye_.lerp(
			    controller_->GetGameCamera()->eye_, player_->GetFbxObject3d()->GetPosition(),
			    30.0f);

		}


		if ( isFinish == false )
		{
			player->Update(input, gamePad);

			controller_->GetGameCamera()->SetFollowerPos(
			player->GetFbxObject3d()->GetWorldTransformPtr()); // カメラの座標の設定

			//controller_->GetGameCamera()->SetFollowerPos(
			//player_->GetObject3d()->GetWorldTransformPtr()); // カメラの座標の設定

			controller_->GetGameCamera()->SetTargetPos(
			    enemy->GetFbxObject3d()->GetWorldTransformPtr());//カメラの注視点の設定

			controller_->GetGameCamera()->MoveCamera();
		}

		isStartSign = false;
		fightSpCount++;
		startSignCount = 0;

		if (fightSpCount > Number::Twenty)
		{
			isFightSP_->SetColor({ 1,1,1,SpAlpha });
			SpAlpha -= decreaseAlpha;
			isFightSP_->SetSize({ SpSize * 320.0f,128.0f });

			if ( SpSize < 2.0f )
			{
				SpSize += addSize;
			}
		}

	}

	player->GetFbxObject3d()->Update();
	enemy->Update();

	BulletManager::GetInstance()->Update();
	//Player_->Update();
	//Enemy_->Update();
#pragma endregion

	player_->GetObject3d()->Update();
	player_->GetFbxObject3d()->Update();
	enemy_->GetObject3d()->Update();
	enemy_->GetFbxObject3d()->Update();
	RockO_->Update();
	//パーティクル

	//スプライトの大きさを設定
	enemyHpSprite_->SetSize({ enemy_->GetHp() * 32.0f, hpSpSize *  32.0f });
	playerHpSprite_->SetSize({ player_->GetHp() * 32.0f, hpSpSize * 32.0f});
	damageSP_->SetSize({enemy_->GetDamageSize() * 32.0f, hpSpSize * 32.0f});
	startSp_->SetSize({ 1280.0f,startSpSize * 256.0f });
	transSP_->SetSize({(float)Size::TenTimes * 275.0f,(float) Size::TenTimes* 183.0f});
	//fbxObject->Update();

//#ifdef _DEBUG
//	ImGui::Begin("Player");
//	ImGui::SetWindowPos({ 200 , 200 });
//	ImGui::SetWindowSize({ 500,100 });
//	ImGui::InputFloat3("Pos", &player_->GetFbxObject3d()->worldTransform.translation_.x);
//	ImGui::InputFloat3("Rotate", &player_->GetFbxObject3d()->worldTransform.rotation_.x);
//	ImGui::End();
//#endif

	sceneObj_->skydomeO_->SetPosition({ sceneObj_->skydomeO_->GetPosition().x + player_->GetPosition().x,sceneObj_->skydomeO_->GetPosition().y,sceneObj_->skydomeO_->GetPosition().z });
	sceneObj_->skydomeO_->Update();
	sceneObj_->transitionO_->Update();

	
	//リセット処理
	if (input->TriggerKey(DIK_R))
	{
		player_->Reset();
		enemy_->Reset();

	}

	if (player_->GetHp() <= Number::Zero || enemy_->GetHp() <= Number::Zero)
	{
		isFinish = true;
	}

	if (isFinish == true && player_->GetHp() <= Number::Zero)
	{
		gameOverAnimetion();
	}

	if (isFinish == true && enemy_->GetHp() <= Number::Zero)
	{
		gameClearAnimetion();
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


	/*enemy_->Draw();
	player_->Draw();*/

	player->Draw();
	
	enemy->Draw();

	BulletManager::GetInstance()->Draw();
	//Player_->Draw();

	//Enemy_->Draw();

	//RockO_->Draw();
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

	if ( player_->GetIsMoveLimit() == true )
	{
		/*player_->GetBarriarParticle()->Draw(controller_->dxCommon_->GetCommandList());*/
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

	if ( isFinish == false )
	{
		playerHpSprite_->Draw();
		enemyHpSprite_->Draw();

		damageSP_->Draw();

	}


	if (player_->GetHp() > 0 && enemy_->GetVanishTimer()>0)
	{
		alart->Draw();
	}

	if ( isFight == true )
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


	if ( isFinishSpCount > Number::Ninety )
	{
		transSP_->Draw();
	}



	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
#pragma region
	//// 3Dオブジェクト描画前処理
	Object3d::PreDraw(controller_->dxCommon_->GetCommandList());

	//if (isFinishSpCount > Number::Sixty)
	//{
	//	sceneObj_->transitionO_->Draw();
	//}

	//// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion
}

void PlayScene::SceneTransition()
{
	sceneObj_->transitionO_->worldTransform.scale_ -= scale;
	sceneObj_->transitionO_->Update();
}

void PlayScene::StartSign(Input* input,GamePad*gamepad)
{
	Vector3 StartPos = { 2,50,50 };

	const float shiftPos = 50.0f;

	//GoalPos = { player_->GetObject3d()->GetPosition().x,player_->GetObject3d()->GetPosition().y + 9 ,player_->GetObject3d()->GetPosition().z - 7 };
	GoalPos = {
	    player_->GetFbxObject3d()->GetPosition().x, player_->GetFbxObject3d()->GetPosition().y + 9,
	    player_->GetFbxObject3d()->GetPosition().z - 7};
	cameraDis = StartPos  - GoalPos ;
	addSpeed =  2.0f * (float)Ease::OutCubic(change,0,120,startSignCount);

	//addSpeed *= -1;

	//cameraDis.nomalize();
	cameraDis *= addSpeed;

	cameraDis.y *= -1;
	cameraDis.z *= -1;

	cameraDis.y += shiftPos; // イージング開始の初期値をずらす

	controller_->GetGameCamera()->GetEye() = cameraDis;
	controller_->GetGameCamera()->SetEye(controller_->camera_->eye_);
	controller_->GetGameCamera()->SetTarget(enemy_->GetFbxObject3d()->GetPosition());

	startSignCount++;
	if (startSignCount >= Number::HundredTwenty)
	{
		startSignCount = Number::HundredTwenty;

		if ( isReady == false )
		{
			if ( startSpSize <= 1.0f )
			{
				startSpSize += addSize * 2.0f;
			}
			if (input->TriggerKey(DIK_SPACE) || gamepad->ButtonInput(A))
			{
				
				isReady = true;
			}
		}
	
	}
	if ( isReady == true )
	{
		if ( startSpSize >= 0.0f )
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

void PlayScene::SetCamera()
{
	if ( isStartSign == true )
	{
		//controller_->camera_->Update();
	}
	else
	{
		controller_->GetGameCamera()->SetFollowerPos(
		    player_->GetFbxObject3d()->GetWorldTransformPtr());
		//controller_->GetGameCamera()->SetFollowerPos(player_->GetObject3d()->GetWorldTransformPtr());
		controller_->GetGameCamera()->SetTargetPos(enemy_->GetFbxObject3d()->GetWorldTransformPtr());
		controller_->GetGameCamera()->MoveCamera();
	}
}

void PlayScene::ResetParam()
{
	player_->Reset();
	enemy_->Reset();
	sceneObj_->player_ = player_;
	sceneObj_->enemy_ = enemy_;
	controller_->GetGameCamera()->SetFollowerPos(player_->GetFbxObject3d()->GetWorldTransformPtr());
	controller_->GetGameCamera()->SetTargetPos(enemy_->GetFbxObject3d()->GetWorldTransformPtr());

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


	finishSpPos = {WinApp::window_width, WinApp::window_height / 2};
	isFinishSpCount = 0; // FIHISHSP_が画面に描画される時間

	addfinishSpeed = 0.1f;
	addRotation = {0, 0, 0};

	finishCameraPlayerPos = {0, 0, 0};
	finishCameraPlayerTarget = {0, 0, 0};

	finishCameraEnemyPos = {0, 0, 0};
	finishCameraEnemyTarget = {0, 0, 0};
	
	transObjAlpha = 0.0f;
	addAlpha = 0.018f;
	isFinish = false;
}


void PlayScene::gameOverAnimetion() {

	isFinishSpCount++;
	if (isFinishSpCount > Number::Ninety)
	{
		transObjAlpha += addAlpha;
	}

	float addRota = 0.0075f;

	addRotation.x += addRota;
	//addRotation.y += addRota;
	addRotation.z+= addRota;
	player_->GetFbxObject3d()->SetRotate(addRotation);

	if ( transObjAlpha >= (float)Size::OneTimes )
	{
		transObjAlpha =(float) Size::OneTimes;
	}

	//transSP_->SetColor({1, 1, 1, transObjAlpha});

	if (isFinishSpCount >28 && isFinishSpCount < Number::Ninety) {
		finishSP_->SetPosition(finishSpPos);
	} else {
		finishSpPos.x -= addfinishSpeed;
		finishSP_->SetPosition({finishSpPos.x, finishSpPos.y});
	}

	if (isFinishSpCount > Number::HundredTwenty) {
		if (player_->GetHp() <= Number::Zero) {
			
			ResetParam();
			audio->StopWave(pSourceVoice[0]);
			controller_->ChangeSceneNum(S_OVER);

		
		} else if (enemy_->GetHp() <= Number::Zero && player_->GetHp() <= Number::Zero)
		{
			// 後で追加
		}
	}



	controller_->GetGameCamera()->SetEye(finishCameraPlayerPos);

	controller_->GetGameCamera()->SetTarget(finishCameraPlayerTarget);

	controller_->GetGameCamera()->Update();
}

void PlayScene::gameClearAnimetion()
{
	//const float shiftRotate = 2.0f;
	transSP_->SetColor({1, 1, 1, transObjAlpha});

	isFinishSpCount++;
	if (isFinishSpCount > Number::Ninety) {
		transObjAlpha += addAlpha;
	}
	float addRota = 0.075f;

	addRotation.y += addRota;
	//addRotation.z = shiftRotate;
	enemy_->GetFbxObject3d()->SetRotate(addRotation);

	if (transObjAlpha >= (float)Size::OneTimes) {
		transObjAlpha = (float)Size::OneTimes;
	}
	//sceneObj_->transitionO_->SetColor({1, 1, 1, transObjAlpha});
	//sceneObj_->transitionO_->Update();
	//transSP_->SetColor({1, 1, 1, transObjAlpha});

	if (isFinishSpCount > 28 && isFinishSpCount < Number::Ninety) {
		finishSP_->SetPosition(finishSpPos);
	} else {
		finishSpPos.x -= addfinishSpeed;
		finishSP_->SetPosition({finishSpPos.x, finishSpPos.y});
	}

	if (isFinishSpCount > Number::HundredTwenty)
	{
		if (enemy_->GetHp() <= Number::Zero)
		{
			ResetParam();
			audio->StopWave(pSourceVoice[0]);
			controller_->ChangeSceneNum(S_CLEAR);
			
		}
		else if (enemy_->GetHp() <= Number::Zero && player_->GetHp() <= Number::Zero)
		{
			// 後で追加
		}
	}

		

	controller_->GetGameCamera()->SetEye(finishCameraEnemyPos);

	controller_->GetGameCamera()->SetTarget(finishCameraEnemyTarget);

	controller_->GetGameCamera()->Update();
}

void PlayScene::finishPlayerCamera() {
	Vector3 finishCameraPlayerVec;

	finishCameraPlayerVec = {-5, 0, 20}; // オフセット座標の設定

	// finishCameraVec = {0, 0, 20};
	//座標の計算
	finishCameraPlayerVec = MyMath::bVelocity(
	    finishCameraPlayerVec, player_->GetFbxObject3d()->GetWorldTransform().matWorld_);

	finishCameraPlayerPos = player_->GetFbxObject3d()->GetPosition() + finishCameraPlayerVec;

		// finishCameraVec = {0, 0, 20};
	//finishCameraPlayerVec = MyMath::bVelocity(
	//    finishCameraPlayerVec, player_->GetObject3d()->GetWorldTransform().matWorld_);

	//finishCameraPlayerPos = player_->GetObject3d()->GetPosition() + finishCameraPlayerVec;

	finishCameraPlayerTarget = player_->GetFbxObject3d()->GetPosition();
}

void PlayScene::finishEnemyCamera() {
	Vector3 finishCameraEnemyVec;

	finishCameraEnemyVec = {-5, 0, -20}; // オフセット座標の設定

	// finishCameraVec = {0, 0, 20};

	//平行移動だけを取り除いた回転行列の計算
	finishCameraEnemyVec = MyMath::bVelocity(
	    finishCameraEnemyVec, enemy_->GetFbxObject3d()->GetWorldTransform().matWorld_);

	finishCameraEnemyPos = enemy_->GetFbxObject3d()->GetPosition() + finishCameraEnemyVec;

	finishCameraEnemyTarget = enemy_->GetFbxObject3d()->GetPosition();
}

