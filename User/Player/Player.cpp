#include "Player.h"
#include "Enemy.h"
#include "ImguiManager.h"
#include "Affin.h"
#include "Ease.h"
#include "MyMath.h"
void Player::Initialize(TTEngine::DirectXCommon* dxCommon, Enemy* enemy) {
	dxCommon_ = dxCommon;

	fbxScale_ = 0.01f;
	
	//デバイスのセット
	FbxObject3d::SetDevice(dxCommon_->GetDevice());
	//グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	////敵のFbx読み込み

	fbxPlayerM_.reset(FbxLoader::GetInstance()->LoadModelFromFile("player2"));

	//fbxPlayerO_ = std::make_unique<FbxObject3d>();
	fbxPlayerO_ = FbxObject3d::Create();
	fbxPlayerO_->SetModel(fbxPlayerM_.get());
	fbxPlayerO_->SetScale({fbxScale_, fbxScale_, fbxScale_});
	fbxPlayerO_->SetPosition({0,0,-50});
	//fbxPlayerO_->SetIsBonesWorldMatCalc(true); // ボーンワールド行列計算あり
	fbxPlayerO_->Update();

	playerO_ = Object3d::Create();

	playerM_ = Model::CreateFromOBJ("human");

	playerO_->SetModel(playerM_);

	wtf.translation_ = { 0,0,-50 };

	playerO_->SetPosition(wtf.translation_);

	bulletM_ = Model::CreateFromOBJ("bullet");

	enemy_ = enemy;


	//パーティクル
	particle_ = std::make_unique<ParticleManager>();
	particle_->Initialize();
	particle_->LoadTexture("sprite/particle.png");
	particle_->GetWorldTransform().scale_ = {30, 30, 30};
	particle_->Update();

		// パーティクル
	stageBarriarParticle_ = std::make_unique<ParticleManager>();
	stageBarriarParticle_->Initialize();
	stageBarriarParticle_->LoadTexture("sprite/barrier.png");
	stageBarriarParticle_->GetWorldTransform().scale_ = {30, 30, 30};
	stageBarriarParticle_->Update();


	//SPHERE_COLISSION_NUM = fbxPlayerO_->GetBonesMatPtr()->size();
	//sphere.resize(SPHERE_COLISSION_NUM);
	//spherePos.resize(SPHERE_COLISSION_NUM);
	//fbxPlayerO_.get()->isBonesWorldMatCalc = true; // ボーンの行列を取得するか

	////FBX当たり判定初期化
	//for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	//{
	//	sphere[i] = new SphereCollider;
	//	CollisionManager::GetInstance()->AddCollider(sphere[i]);
	//	spherePos[i] = fbxPlayerO_->bonesMat[i].GetWorldPos();
	//	sphere[i]->SetBasisPos(&spherePos[i]);
	//	sphere[i]->SetRadius(1.0f);
	//	sphere[i]->SetAttribute(COLLISION_ATTR_PLAYERS);
	//	sphere[i]->Update();
	//	////test
	//	//coliderPosTest_[i] = Object3d::Create();
	//	//coliderPosTest_[i]->SetModel(hpModel_.get());
	//	//coliderPosTest_[i]->SetPosition(sphere[i]->center);
	//	//coliderPosTest_[i]->SetScale({ sphere[i]->GetRadius(),sphere[i]->GetRadius() ,sphere[i]->GetRadius() });
	//	//coliderPosTest_[i]->SetRotate({ 0,0,0 });
	//	//coliderPosTest_[i]->Update();

	//}

	SPHERE_COLISSION_NUM = 1;
	sphere.resize(SPHERE_COLISSION_NUM);
	spherePos.resize(SPHERE_COLISSION_NUM);

	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		spherePos[i] = fbxPlayerO_->GetPosition();
		sphere[i]->SetBasisPos(&spherePos[i]);
		sphere[i]->SetRadius(1.0f);
		sphere[i]->SetAttribute(COLLISION_ATTR_PLAYERS);
		sphere[i]->Update();
		

	}
	startCount = clock() / 1000;

	blowAwayPos = fbxPlayerO_->GetPosition();
	//playerO_->SetRotation({180, 0, 0});

	vanishBeforeCount = 30.0f;

	Distance_.z = fbxPlayerO_->GetPosition().z - 50;

	//音声データの初期化と読み取り
	audio = new TTEngine::Audio();
	audio->Initialize();

	audio->LoadWave("bullet.wav");
}

void Player::Update(Input* input, GamePad* gamePad)
{
	//カメラの角度の更新
	moveAngle();

	Vector3 d = {
	    enemy_->GetFbxObject3d()->GetPosition().x - fbxPlayerO_->GetPosition().x,
	    enemy_->GetFbxObject3d()->GetPosition().y - fbxPlayerO_->GetPosition().y,
	    enemy_->GetFbxObject3d()->GetPosition().z - fbxPlayerO_->GetPosition().z};

	posDistance.z = sqrtf(pow(d.x, 2.0f) + pow(d.z, 2.0f));



	if ( isStep == false && isShot == false)
	{
		isStandBy = true;
	}

	if ( isStandBy == true )
	{
		//	スタンバイアニメーションの再生
		fbxPlayerO_->PlayAnimation(FbxAnimetion::IDEL, true);
	}

	/*fbxPlayerO_->PlayAnimation(0);*/

	if (Hp_ <= 0)
	{
		SetisDead(true);

		
		GameOverAnime();
	}
	else
	{
		transNormal = {0, 0.5f, -5};

		transNormal = MyMath::bVelocity(transNormal, playerO_->worldTransform.matWorld_);
		//transNormal = MyMath::bVelocity(transNormal, fbxPlayerO_->worldTransform.matWorld_);
	}
	
	oldPos = wtf.translation_;


	//デスフラグが立った球を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->GetIsDead(); });


	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	if (GetisDead() == true)
	{
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
		{
			bullet->SetisDead(true);
		}
	}

	if (bulletType != PlayerBulletType::RapidShot )
	{
		if (isStep == false && GetVanish() == false)
		{
			Move(input, gamePad);//連射攻撃、ステップ移動、回避をしていない時に移動できる
		}
		if ( bulletType != PlayerBulletType::OneShot )
		{
			Step(input, gamePad);//弾を打っていない時
		}
		
	}

	Shot(input, gamePad);


	Vanish();
	////行列の更新など
	//playerO_->UpdateMatrix();

	particle_->Update();
	stageBarriarParticle_->Update();
	
	fbxPlayerO_->Update();

	
	
	//ImGui::Begin("BulletSize");
	//ImGui::SetWindowPos({ 800 , 100 });
	//ImGui::SetWindowSize({ 500,300 });
	//ImGui::InputInt("plyaerHp ", &Hp_);
	//ImGui::InputInt("enemyHp", &enemy_->Hp_);
	///*ImGui::SetWindowPos({ 800 , 100 });
	//ImGui::SetWindowSize({ 500,300 });
	//ImGui::InputInt("BulletSize", &bulletSize);
	//ImGui::InputFloat("pushTimer",&pushTimer);
	//ImGui::InputFloat("pressTimer", &pressTimer);
	//ImGui::InputInt("MAX", &MAX_BULLET);*/

	//ImGui::End();
	
	/*ImGui::Begin("Vanish");

	ImGui::SliderFloat("Gauge",&VanishGauge, -400.0f, 400.0f);
	ImGui::InputFloat("Timer", &vanishTimer);
	ImGui::End();*/


	CheckHitCollision();
}

void Player::Draw()
{
	if (GetisDead() == false)
	{
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
		{
			bullet->Draw();
		}
		

		/*fbxPlayerO_->Draw(dxCommon_->GetCommandList());*/
	}
	if ( blowAwayCount <90)
	{
		fbxPlayerO_->Draw(dxCommon_->GetCommandList());
		//if (scale_.x > 0.2)
		//{
		//	playerO_->Draw();
		//}
	}
}

void Player::Move(Input* input, GamePad* gamePad)
{
	velocity_ = {0, 0, 0};
	fbxVelocity_ = {0, 0, 0};


	if (gamePad->StickInput(L_UP) || input->PushKey(DIK_W))
	{

		velocity_ += { 0 , 0 , moveSpeed };

		fbxVelocity_ += {0, 0, (moveSpeed / fbxScale_)};

	}


	if (gamePad->StickInput(L_DOWN) || input->PushKey(DIK_S)) {

		velocity_ += {0, 0, backSpeed * -1};

		//敵との距離で移動制限の実装
		//上限に達していない時
		if (posDistance.z <= MAX_POSITION) {
			backSpeed = 0.5f;
			isMoveLimit = false;
		}
		else//達しているとき
		{
			backSpeed = MAX_POSITION - posDistance.z;
			isMoveLimit = true;

			stageBarriarParticle_->Barrier(fbxPlayerO_->worldTransform.translation_);
			

		}
		//上記のうちどちらか小さい値の方をspeedに代入
		backSpeed = min((MAX_POSITION - posDistance.z), 0.5f);
		//speedを加算
		fbxVelocity_ += {0, 0, (backSpeed / fbxScale_) * -1};

	}

	
	if (gamePad->StickInput(L_LEFT) || input->PushKey(DIK_A))
	{

		velocity_ += { moveSpeed * -1 , 0 , 0 };
		fbxVelocity_ += {(moveSpeed / fbxScale_) *-1, 0, 0};

		//	左移動のアニメーションの再生
		fbxPlayerO_->PlayAnimation(FbxAnimetion::LEFTMOVE, false);
	}

	if (gamePad->StickInput(L_RIGHT) || input->PushKey(DIK_D))
	{
		velocity_ += { moveSpeed , 0 , 0 };
		fbxVelocity_ += {(moveSpeed / fbxScale_), 0, 0};
		fbxPlayerO_->PlayAnimation(FbxAnimetion::RIGHTMOVE, false);
	}

	
	
	//// OBJの移動処理
	//if (GetisDead()==false)
	//{
	//	playerO_->worldTransform.rotation_ = cameraAngle;
	//}
	//playerO_->worldTransform.UpdateMatWorld();

	//velocity_ = MyMath::MatVector(velocity_, playerO_->worldTransform.matWorld_);

	//playerO_->worldTransform.translation_ += velocity_;

	//playerO_->SetPosition(playerO_->worldTransform.translation_);

	

	//FBXの移動処理

	if (GetisDead() == false) {
		fbxPlayerO_->worldTransform.rotation_ = cameraAngle;
	}

	fbxPlayerO_->worldTransform.UpdateMatWorld();

	fbxVelocity_ = MyMath::MatVector(fbxVelocity_, fbxPlayerO_->worldTransform.matWorld_);

	fbxPlayerO_->worldTransform.translation_ += fbxVelocity_;

	fbxPlayerO_->SetPosition(fbxPlayerO_->worldTransform.translation_);


#ifdef _DEBUG
	ImGui::Begin("playerPos");

	ImGui::SetWindowPos({ 600 , 200 });
	ImGui::SetWindowSize({ 400,200 });
	ImGui::InputFloat3("Distance_", &posDistance.x);

	ImGui::InputFloat3("fbxPos", &fbxPlayerO_->worldTransform.translation_.x);

	ImGui::End();
#endif
}


//ステップ移動(横移動だけ)
void Player::Step(Input* input, GamePad* gamePad)
{

	velocity_ = {0, 0, 0};
	fbxVelocity_ = {0, 0, 0};

	// 左スティックを弾いたとき
	if (gamePad->StickOffTrigger(L_LEFT) || input->TriggerKey(DIK_LEFT)) {
		// wtf.translation_.x -= 0.5f;
		// playerO_->SetPosition(wtf.translation_);
		if (isStep == false)
		{
			fbxPlayerO_->SetCurrentTimer(0);
			fbxPlayerO_->PlayAnimation(FbxAnimetion::LEFTSTEP, false);
			stepFlameCount = 30;
			stepDirection = StepDirection::Left;
			StepSpeed = 2.0f * -1;
			isStep = true;
		}
	}

	//右スティックを弾いたとき
	if (gamePad->StickOffTrigger(L_RIGHT) || input->TriggerKey(DIK_RIGHT)) {
		// wtf.translation_.x += 0.5f;
		// playerO_->SetPosition(wtf.translation_);
		
		if (isStep == false)
		{
			fbxPlayerO_->SetCurrentTimer(0);
			fbxPlayerO_->PlayAnimation(FbxAnimetion::RIGHTSTEP, false);
			stepFlameCount = 30;
			stepDirection = StepDirection::Right;
			StepSpeed = 2.0f;
			isStep = true;
		}
		
		
	}


	if (isStep == true)
	{
		isStandBy = false;
		fbxVelocity_ += {(StepSpeed / fbxScale_), 0, 0};
		//velocity_ += {StepSpeed, 0, 0};
		stepFlameCount--;
	}

	if ( stepDirection == StepDirection::Left )
	{
		StepSpeed += 0.1f;

		if (StepSpeed >= 0.0f)
		{
			StepSpeed = 0.0f;
		}
		if (stepFlameCount <= 0)
		{
			isStep = false;
		}
	}
	if (stepDirection == StepDirection::Right)
	{
		StepSpeed -= 0.1f;
		if (StepSpeed <= 0.0f)
		{
			StepSpeed = 0.0f;
		}
		if (stepFlameCount <= 0)
		{
			isStep = false;
		}
	}

	// FBXの移動処理
	if (GetisDead() == false) {
		fbxPlayerO_->worldTransform.rotation_ = cameraAngle;
	}

	fbxPlayerO_->worldTransform.UpdateMatWorld();

	fbxVelocity_ = MyMath::MatVector(fbxVelocity_, fbxPlayerO_->worldTransform.matWorld_);

	fbxPlayerO_->worldTransform.translation_ += fbxVelocity_;

	fbxPlayerO_->SetPosition(fbxPlayerO_->worldTransform.translation_);

	//playerO_->worldTransform.translation_ += fbxVelocity_;

	//playerO_->SetPosition(playerO_->worldTransform.translation_);

}

void Player::Shot(Input* input, GamePad* gamePad)
{
	//nowCount++;

	//elapsedCount = nowCount - startCount;
	//float elapsedTime = static_cast<float> (elapsedCount) / 1000000.0f;

	//timeRate = min(elapsedTime / maxTime, 1.0f);

#pragma region 弾の移動処理
	float speed = 0.5f;
	if (bulletType == PlayerBulletType::OneShot)
	{
		speed = 1.0f;
	}
	else if (bulletType == PlayerBulletType::RapidShot)
	{
		speed = 3.0f;
	}
	

	Vector3 playerPos;
	Vector3 enemyPos;
	Vector3 distance;


	playerPos = fbxPlayerO_->worldTransform.translation_;
	//playerPos = playerO_->worldTransform.translation_;

	enemyPos = enemy_->GetFbxObject3d()->GetWorldTransform().translation_;

	distance = enemyPos - playerPos;

	distance.nomalize();

	distance *= speed;

	/*Vector3 begieP1 = {0,10,-30};
	Vector3 begieP2 = { 0,-30,-10 };

	Vector3 a = a.lerp(playerPos,begieP1, timeRate);
	Vector3 b = b.lerp(begieP1, begieP2, timeRate);
	Vector3 c = c.lerp(begieP2, enemyPos, timeRate);

	Vector3 d = d.lerp(a, b, timeRate);
	Vector3 e = e.lerp(b, c, timeRate);
	distance = distance.lerp(d,e, timeRate);*/


#pragma endregion

	//押したとき
	if (gamePad->ButtonInput(A) || input->PushKey(DIK_SPACE))
	{
		pushTimer--;
		pressTimer--;
		
		//連射の時
		if (pushTimer < 0)
		{
			rapidShot = true;
		}
	}
	

	if (oneShot == true)
	{
		rapidShot = false;
		//弾の最大個数
		MAX_BULLET = 1;
		pushTimer = 15.0f;//押してる時間
		pressTimer = 0.0f;//連射用の時間
		oneShot = false;
	}
	else if (rapidShot == true)
	{
		oneShot = false;
		if (pressTimer < 0)
		{
			if (MAX_BULLET < 20)
			{
				MAX_BULLET++;
			}
			pressTimer = 6.0f;
		}
	}
	if (isShot == true)
	{
		bulletTimer--;

		isStandBy = false;
		// 射撃アニメーションの再生
		fbxPlayerO_->PlayAnimation(FbxAnimetion::SHOT, false);

		//弾が最大個数以下だった時
		if (bulletSize < MAX_BULLET)
		{
			if (bulletTimer <= 0)
			{
				if (isShot == true)
				{
					bulletSize++;
					// 弾を生成し初期化
					std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
					newBullet->Initialize(fbxPlayerO_->worldTransform.translation_, distance);
					//newBullet->Initialize(
					//    bulletM_, fbxPlayerO_->worldTransform.translation_, distance);

					if (bulletType == PlayerBulletType::OneShot)
					{
						bulletTimer = 15.0f;
					}
					else if (bulletType == PlayerBulletType::RapidShot)
					{
						bulletTimer = 5.0f;
					}
					
					//
					//newBullet->SetEnemy(enemy_);

					bullets_.push_back(std::move(newBullet));
					// 音声再生
					//if (soundCheckFlag == 0) {
					//	// 音声再生

					//	pSourceVoice[0] = audio->PlayWave("oto.wav");
					//	pSourceVoice[0]->SetVolume(0.5f);
					//	soundCheckFlag = 1;
					//}

					// 音声再生
					


					pSourceVoice[0] = audio->PlayWave("bullet.wav");
					pSourceVoice[0]->SetVolume(0.2f);
					
				}

			}
		}

		//撃っている弾の個数が上限に達した時
		if (bulletSize >= MAX_BULLET)
		{
			MAX_BULLET = 0;
			bulletSize = 0;
			bulletType = PlayerBulletType::None;
			coolTimer = 60.0f;
			pushTimer = 15.0f;
			pressTimer = 0.0f;
			isShot = false;
		}
	}

	//ボタン又はキーを離したとき
	if (gamePad->ButtonOffTrigger(A) || input->ReleaseKey(DIK_SPACE))
	{
		//単発の時
		if (pushTimer >= 0)
		{
			bulletType = PlayerBulletType::OneShot;
			oneShot = true;
		}
		else
		{
			bulletType = PlayerBulletType::RapidShot;
		}
		if (isShot == false)
		{
			isShot = true;
		}
	}

}

void Player::Vanish()
{

	// カメラの角度の更新
	moveAngle();

	Vector3 offSet = {10,0,25};
	offSet = MyMath::bVelocity(offSet, enemy_->GetFbxObject3d() ->GetWorldTransform().matWorld_);

	VanishPos = enemy_->GetFbxObject3d()->GetPosition() + offSet;

	playerPos_ = fbxPlayerO_->GetPosition();
	//playerPos_ = playerO_->GetPosition();

	VanishDis = VanishPos - playerPos_;

	//if (GetisDead() == false) {
	//	fbxPlayerO_->worldTransform.rotation_ = cameraAngle;
	//}
	//fbxPlayerO_->worldTransform.UpdateMatWorld();
	//VanishDis.nomalize();
	//敵が攻撃をしてきたとき
	//if (enemy_->GetisShot() == true)
	//{
	//	if (isTimerSet == false)
	//	{
	//		//回避できるタイマーをセット
	//		vanishTimer = 60.0f;
	//		isTimerSet = true;
	//	}
	//	
	//	
	//}
	// タイマーが0以上の時
	if (enemy_->GetVanishTimer() > 0) {
		// 特定の操作をしたら
		if (isStep == true && stepFlameCount > 15) {
			// 回避ゲージが満タンの時
			if (VanishGauge == MAX_VANISHCOUNT) {
				// 回避していなかったとき
				if (isVanising == false) {
					VanishGauge = 0.0f;
					/*VanishPos = enemy_->GetObject3d()->GetPosition() + offSet;*/
				
					isVanising = true;
				}
			}
		}

	} else {
		isTimerSet = false;
		vanishTimer = 60.0f;
	}
		
	if (vanishTimer > 0)
	{
		vanishTimer--;
	}
	//回避したら
	if (isVanising == true)
	{
		
		if ( VanishGauge <= 0.3f )
		{

			oldPos = fbxPlayerO_->worldTransform.translation_;
			/*VanishDis *= 0.05f;*/
			fbxPlayerO_->worldTransform.translation_ += VanishDis;
			//playerO_->worldTransform.translation_ += VanishDis;
		}

		//oldPos = fbxPlayerO_->worldTransform.translation_;
		////VanishDis *= 0.05f;
		//fbxPlayerO_->worldTransform.translation_ += VanishDis;
		//playerO_->worldTransform.translation_ += VanishDis;
		

		VanishGauge += 0.1f;
		if (VanishGauge >= MAX_VANISHCOUNT)
		{
			VanishGauge = MAX_VANISHCOUNT;
		}
	}
	//回避ゲージが満タンなら
	if (VanishGauge == MAX_VANISHCOUNT)
	{
		//回避が可能になる
		if (isVanising == true)
		{
			isVanising = false;
		}
	}
}



void Player::Damage()
{}

void Player::RemoveAttribute()
{
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {

		CollisionManager::GetInstance()->RemoveCollider(sphere[i]);
		// こいつはいらない
		/*sphere[i]->GetCollisionInfo().collider->RemoveAttribute(COLLISION_ATTR_PLAYERBULLETS);*/
	}
}

void Player::TitleAnime()
{
	CheckHitCollision();

	srand(( unsigned int ) time(nullptr));

	if ( animevanish == false )
	{
			
		scale_.x -= reduction;
		playerO_->SetScale(scale_);
		if ( playerO_->GetScale().x < 0 )
		{
			vstanCount--;
		
			playerO_->SetScale({ 0,0,0 });

		}

		if ( vstanCount <= 0 )
		{
			randPosX = rand() % 81 - 40;
			randPosZ = rand() % 81 - 40;
			animevanish = true;
		}
	}

	if ( animevanish == true )
	{
		playerO_->SetPosition({ ( float ) randPosX,0,( float ) randPosZ });
		scale_.x += expansion;
		playerO_->SetScale(scale_);
		if ( playerO_->GetScale().x >= 1 )
		{
			animevanish = false;
			vstanCount = 30.0f;
			playerO_->SetScale({ 1,1,1 });

		}
		
	}

}

void Player::GameOverAnime()
{
	blowAwayCount++;
	
	//水平投射をしながら自機を下に落下させる
	Affin::HorizontalProjection(fbxPlayerO_->worldTransform, transNormal, 1.0f, blowAwayCount);

	fbxPlayerO_->SetPosition(fbxPlayerO_->worldTransform.translation_);

}

void Player::CheckHitCollision()
{

#pragma region オブジェクト同士の押し出し処理
	class PlayerQueryCallBack : public QueryCallback
	{
	public:
		PlayerQueryCallBack(Sphere* sphere) : sphere(sphere) {};

		bool OnQueryHit(const QueryHit& info)
		{
			rejectDir = info.reject;
			rejectDir.nomalize();

			//上方向と排斥方向の角度差のコサイン値
			float cos = rejectDir.dot(up);

			//
			const float threshold = cosf(XMConvertToRadians(30.0f));
			//角度差によって天井または地面と判定される場合を除いて
			if (-threshold < cos && cos < threshold)
			{
				//押し出す
				sphere->center += info.reject;
				move += info.reject;
			}
			return true;
		}
		//void SphereQuery();

		//ワールドの上方向
		const Vector3 up = { 0,1,0 };
		//排斥方向
		Vector3 rejectDir;
		//クエリーに使用する球
		Sphere* sphere = nullptr;
		//排斥による移動量
		Vector3 move = {};

	};


	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		PlayerQueryCallBack callback(sphere[i]);

		//球と地形の交差を全探索する
		CollisionManager::GetInstance()->QuerySphere(*sphere[i], &callback);

		if (sphere[i]->GetIsHit() == true)
		{
			//当たったものの属性が敵だった時
			if (sphere[i]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_ENEMYS)
			{
				playerO_->worldTransform.translation_.x += callback.move.x;
				playerO_->worldTransform.translation_.y += callback.move.y;
				playerO_->worldTransform.translation_.z += callback.move.z;
				break;
			}
		}

	}
#pragma endregion 

	oldPos = wtf.translation_;

	Vector3 distance;

	if (hitDeley > 0) {	//毎フレームヒットを防止
		//playerO_->SetColor({ 0,0,1,1 });
		hitDeley--;
	}
	else
	{
		//playerO_->SetColor({ 1,1,1,1 });
	}

	//当たり判定の処理
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		if (hitDeley <= 0 && sphere[i]->GetIsHit() == true)
		{
			//当たったものの属性が敵の弾だった時
			if (sphere[i]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_ENEMYBULLETS)
			{

				isDamage = false;
				hitCountTime = 60;
				if ( isDamage == false )
				{
					 //Hp_ -= 1;
					hitDeley = 3;
					particle_->RandParticle(sphere[i]->GetCollisionInfo().inter_);
					//	被弾時のアニメーションの再生
					fbxPlayerO_->SetCurrentTimer(0);
					fbxPlayerO_->PlayAnimation(FbxAnimetion::HIT, false);
					isDamage = true;
				}
				
				

				

				break;
			}
		}

	}
	if (GetisDead() == true)
	{
		for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {

			CollisionManager::GetInstance()->RemoveCollider(sphere[i]);
			//こいつはいらない
			/*sphere[i]->GetCollisionInfo().collider->RemoveAttribute(COLLISION_ATTR_PLAYERBULLETS);*/
		}
	}
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {

		spherePos[i] = fbxPlayerO_->GetPosition();
		sphere[i]->Update();
	}
	if ( isDamage == true )
	{
		hitCountTime--;
	}

	if ( hitCountTime <= 0 )
	{
		isDamage = false;
	}


}

void Player::moveAngle()
{
	cameraAngle.y = 
		atan2(
	    fbxPlayerO_->GetCamera()->GetTarget().x - fbxPlayerO_->GetCamera()->GetEye().x,
	    fbxPlayerO_->GetCamera()->GetTarget().z - fbxPlayerO_->GetCamera()->GetEye().z);
}

void Player::Reset()
{
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Reset();

	}

	//デスフラグが立った球を削除
	bullets_.remove_if([ ] (std::unique_ptr<PlayerBullet>& bullet){return bullet->GetIsDead();});

	for ( int i = 0; i < SPHERE_COLISSION_NUM; i++ )
	{

		CollisionManager::GetInstance()->RemoveCollider(sphere[i]);
		//こいつはいらない
		/*sphere[i]->GetCollisionInfo().collider->RemoveAttribute(COLLISION_ATTR_PLAYERBULLETS);*/
	}
	ResetAttribute();

 //	playerO_->SetColor({1, 1, 1, 1});

	//playerO_->SetPosition({0,0,-50});
	//playerO_->SetRotation({0,0,0});

	fbxPlayerO_->SetScale({0.01f, 0.01f, 0.01f});
	fbxPlayerO_->SetRotate({0,0,0});
	fbxPlayerO_->SetPosition({0, 0, -50});

	oldPos = { 0,0,0 };
	playerPos_ = { 0,0,0 };
	enemyPos_ = { 0,0,0 };
	distance_ = { 0,0,0 };

	Hp_ = 10;
	isDead_ = false;
	isDamage = false;
	faceAngle_ = { 0 , 0 , 0 };
	cameraAngle = { 0,0,0 };
	velocity_ = { 0,0,0 };

	hitDeley = 0;

	VanishGauge = MAX_VANISHCOUNT;
	isVanising = false;
	VanishPos;

	stepFlameCount = 0;
	isStep = false;
	VanishDis = {0, 0, 0};

	isShot = false;
	//単発
	oneShot = false;
	//連射
	rapidShot = false;
	//弾の最大個数
	MAX_BULLET = 0;
	//現在の弾の個数
	bulletSize = 0;
	//弾のタイプ
	bulletType = PlayerBulletType::None;
	//弾と弾の間隔時間
	bulletTimer = 0.0f;
	//連射制限のためのクールタイム
	coolTimer = 60.0f;
	//ボタンを押してる時間
	pushTimer = 15.0f;
	//長押ししている時間
	pressTimer = 0.0f;

	//ベジエにしようとしたやつ
	startCount = 0;
	nowCount = 0;
	elapsedCount = 0;
	maxTime = 5.0f;				//全体時間[s]
	timeRate;						//何％時間が進んだか

	particle_->Reset();
	blowAwayCount = 0;
	blowAwayPos = fbxPlayerO_->GetPosition();

}

void Player::ResetAttribute()
{
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		spherePos[i] = fbxPlayerO_->GetPosition();
		sphere[i]->SetBasisPos(&spherePos[i]);
		sphere[i]->SetRadius(1.0f);
		sphere[i]->SetAttribute(COLLISION_ATTR_PLAYERS);
		sphere[i]->Update();
		////test
		//coliderPosTest_[i] = Object3d::Create();
		//coliderPosTest_[i]->SetModel(hpModel_.get());
		//coliderPosTest_[i]->SetPosition(sphere[i]->center);
		//coliderPosTest_[i]->SetScale({ sphere[i]->GetRadius(),sphere[i]->GetRadius() ,sphere[i]->GetRadius() });
		//coliderPosTest_[i]->SetRotate({ 0,0,0 });
		//coliderPosTest_[i]->Update();

	}

	//// FBX当たり判定初期化
	//for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
	//	sphere[i] = new SphereCollider;
	//	CollisionManager::GetInstance()->AddCollider(sphere[i]);
	//	spherePos[i] = fbxPlayerO_.get()->bonesMat[i].GetWorldPos();
	//	sphere[i]->SetBasisPos(&spherePos[i]);
	//	sphere[i]->SetRadius(1.0f);
	//	sphere[i]->SetAttribute(COLLISION_ATTR_ALLIES);
	//	sphere[i]->Update();
	//	////test
	//	// coliderPosTest_[i] = Object3d::Create();
	//	// coliderPosTest_[i]->SetModel(hpModel_.get());
	//	// coliderPosTest_[i]->SetPosition(sphere[i]->center);
	//	// coliderPosTest_[i]->SetScale({ sphere[i]->GetRadius(),sphere[i]->GetRadius()
	//	// ,sphere[i]->GetRadius() }); coliderPosTest_[i]->SetRotate({ 0,0,0 });
	//	// coliderPosTest_[i]->Update();
	//}

}

