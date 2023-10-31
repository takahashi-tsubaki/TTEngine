#include "SceneObjects.h"
#include "SceneManager.h"
void SceneObjects::Initialize(SceneManager* controller)
{
	Sprite::LoadTexture(1, L"Resources/kuribo-.jpg");
	Sprite::LoadTexture(2, L"Resources/mario.jpg");
	Sprite::LoadTexture(3, L"Resources/sprite/hp.png");//hpバー
	Sprite::LoadTexture(4, L"Resources/sprite/clear.png");
	Sprite::LoadTexture(5, L"Resources/sprite/over.png");
	Sprite::LoadTexture(6,L"Resources/sprite/title2.png");//タイトル
	Sprite::LoadTexture(7, L"Resources/sprite/alart2.png");// !マーク
	Sprite::LoadTexture(8, L"Resources/black.jpg");
	Sprite::LoadTexture(9,L"Resources/sprite/Space2.png");//space
	Sprite::LoadTexture(10,L"Resources/sprite/stageSelect.png");//stage select
	Sprite::LoadTexture(11,L"Resources/sprite/damageRed.png");//hp減少バー

	Sprite::LoadTexture(12,L"Resources/sprite/Fight.png");//Fight
	Sprite::LoadTexture(13,L"Resources/sprite/StartSign.png");//敵を倒せ
	Sprite::LoadTexture(14, L"Resources/sprite/Win.png"); // 敵を倒せ
	Sprite::LoadTexture(15, L"Resources/sprite/Lose.png"); // 敵を倒せ
	Sprite::LoadTexture(16, L"Resources/sprite/Finish.png"); // 敵を倒せ

	selectSp_ = Sprite::Create(10,{100,100});
	spaceButton_ = Sprite::Create(9,{ 525,500 });

	controller_ = controller;

	skydomeO_ = Object3d::Create();
	skydomeM_ = Model::CreateFromOBJ("skydome");
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

	fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("boss_prot4");

	fbxObject = new FbxObject3d();
	fbxObject->Initialize();
	fbxObject->SetModel(fbxModel);
	//fbxObject->SetScale({0.1f,0.1f,0.1f});
	//fbxObject->SetPosition({ 0,-50,0 });

	fbxObject->SetPosition({ 0,-10,10 });

	player_ = new Player();
	enemy_ = new Enemy();

	player_->Initialize(controller_->dxCommon_, enemy_);
	enemy_->Initialize(controller_->dxCommon_,player_);
}

void SceneObjects::Delete()
{
	delete skydomeO_;
	delete skydomeM_;
	delete selectSkydomeO_;
	delete selectSkydomeM_;
	delete fbxObject;
	delete fbxModel;
	delete player_;
	delete enemy_;
}

void SceneObjects::Reset()
{
	enemy_->Reset();
	player_->Reset();
}


