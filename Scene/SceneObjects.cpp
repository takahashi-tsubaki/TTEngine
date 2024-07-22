#include "SceneObjects.h"
#include "SceneManager.h"
void SceneObjects::Initialize(SceneManager* controller,SceneObjects* sceneObj)
{
	sceneObj_ = sceneObj;
#pragma region ゲーム前シーン関連
	Sprite::LoadTexture(SpriteNumber::TITLE, L"Resources/sprite/title2.png"); // タイトル
	Sprite::LoadTexture(SpriteNumber::STAGESELECT, L"Resources/sprite/stageSelect.png");  // stage select

	Sprite::LoadTexture(SpriteNumber::SCENETRANS, L"Resources/black.jpg");
	Sprite::LoadTexture(SpriteNumber::SPACE, L"Resources/sprite/Space2.png"); // space


#pragma endregion 

#pragma region

	Sprite::LoadTexture(SpriteNumber::FIGHT, L"Resources/sprite/Fight.png"); // Fight
	Sprite::LoadTexture(SpriteNumber::STARTSIGN, L"Resources/sprite/StartSign.png");   // 敵を倒せ
	Sprite::LoadTexture(SpriteNumber::STARTTUTORIAL,L"Resources/sprite/StartTutorial.png"); // 操作を覚えよう
	Sprite::LoadTexture(SpriteNumber::TUTORIALTEXT,L"Resources/sprite/Tutorial2.png"); // 操作を覚えよう
	Sprite::LoadTexture(SpriteNumber::BATTLETEXT,L"Resources/sprite/Battle2.png"); // 操作を覚えよう
#pragma endregion

#pragma region 戦闘中のスプライト

	Sprite::LoadTexture(SpriteNumber::HPBAR, L"Resources/sprite/hp.png"); // hpバー
	Sprite::LoadTexture(SpriteNumber::DAMAGEBAR, L"Resources/sprite/damageRed.png"); // hp減少バー

	Sprite::LoadTexture(SpriteNumber::ALART, L"Resources/sprite/alart2.png"); // !マーク
	Sprite::LoadTexture(SpriteNumber::PAUSE, L"Resources/sprite/Pause.png");        // ポーズ マーク
	Sprite::LoadTexture(SpriteNumber::OPERATION, L"Resources/sprite/Operation.png"); // ポーズ中のスプライト
	Sprite::LoadTexture(SpriteNumber::SOUSA, L"Resources/sprite/sousahouhou.png"); // 操作説明のスプライト
	Sprite::LoadTexture(SpriteNumber::FINALCHECK, L"Resources/sprite/finalCheck.png");//最終確認用のスプライト

#pragma endregion

#pragma region リザルト関連

	Sprite::LoadTexture(SpriteNumber::WIN, L"Resources/sprite/Win.png"); // WIN
	Sprite::LoadTexture(SpriteNumber::LOSE, L"Resources/sprite/Lose.png");    // LOSE
	Sprite::LoadTexture(SpriteNumber::FINISH, L"Resources/sprite/Finish.png");    // FINISH

#pragma endregion
	 

	selectSp_ = Sprite::Create(SpriteNumber::STAGESELECT, {100, 100});
	spaceButton_ = Sprite::Create(SpriteNumber::SPACE, {550, 575});

	controller_ = controller;

	skydomeO_ = Object3d::Create();
	skydomeM_ = Model::CreateFromOBJ("skydome2");
	skydomeO_->SetModel(skydomeM_);
	skydomeO_->SetScale({ 10,10,10 });

	selectSkydomeO_ = Object3d::Create();
	selectSkydomeM_ = Model::CreateFromOBJ("selectSkydome");
	selectSkydomeO_->SetModel(selectSkydomeM_);

	selectSkydomeO_->SetScale({ 10,10,10 });

	transitionM_ = Model::CreateFromOBJ("transition");
	transitionO_ = Object3d::Create();
	transitionO_->SetModel(transitionM_);
	transitionO_->SetScale({ 1,1,1 });

	////弾モデルの生成
	bombBulletM_ = Model::CreateFromOBJ("bullet");
	BombBullet::SetModel(bombBulletM_);
	bulletM_ = Model::CreateFromOBJ("bullet");
	Bullet::SetModel(bulletM_);
	homingBulletM_ = Model::CreateFromOBJ("bullet");
	HomingBullet::SetModel(homingBulletM_);
	laserM_ = Model::CreateFromOBJ("laser");
	Laser::SetModel(laserM_);


	//fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("boss_prot4");

	fbxObject = new FbxObject3d();
	fbxObject->Initialize();
	fbxObject->SetModel(fbxModel);
	//fbxObject->SetScale({0.1f,0.1f,0.1f});
	//fbxObject->SetPosition({ 0,-50,0 });

	fbxObject->SetPosition({ 0,-10,10 });


	particle_ = std::make_unique<ParticleManager>();
	particle_->SetDrawBlendMode(1);
	particle_->Initialize();
	particle_->LoadTexture("sprite/particle.png");
	particle_->Update();


	particleObj_ = new ObjParticleManager();
	particleObj_->Init(transitionM_);


	// 音声データの初期化と読み取り
	 audio_ = new TTEngine::Audio();
	 audio_->Initialize();

	 audio_->LoadWave("bullet.wav");

	 effectO_ = Object3d::Create();
	 effectM_ = Model::CreateFromOBJ("Wind");
	 effectO_->SetModel(effectM_);
	 effectO_->SetScale({1, 1, 1});

	 player = new PlayerCharacter();
	 enemy = new EnemyCharacter();

	 player->Initialize(controller_->GetDxCommon(),{0,0,0},enemy,this);
	 enemy->Initialize(controller_->GetDxCommon(),{ 0,0,0 },player,this);
}

void SceneObjects::Delete()
{
	delete skydomeO_;
	delete skydomeM_;
	delete selectSkydomeO_;
	delete selectSkydomeM_;
	delete effectM_;
	delete effectO_;
	delete fbxObject;
	delete fbxModel;

	delete player;
	delete enemy;
}

void SceneObjects::Reset()
{
	//enemy_->Reset();
	//player_->Reset();
	transitionO_->SetScale({ 400, 400, 60 });
	transitionO_->SetPosition({ 0,-600,0 });

}


