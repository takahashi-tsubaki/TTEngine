#include "Enemy.h"
#include "ImguiManager.h"
void Enemy::Initialize(DirectXCommon* dxCommon)
{

	dxCommon_ = dxCommon;

	//デバイスのセット
	FbxObject3d::SetDevice(dxCommon_->GetDevice());
	//グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	////敵のFbx読み込み
	//enemyFbxM_.reset(FbxLoader::GetInstance()->LoadModelFromFile("boss_prot4"));
	//enemyFbxO_ = std::make_unique<FbxObject3d>();
	//enemyFbxO_->Initialize();
	//enemyFbxO_->SetModel(enemyFbxM_.get());
	//enemyFbxO_->PlayAnimetion(5);

	//enemyFbxO_->SetPosition(enemy_.translation_);

	enemyO_ = Object3d::Create();

	enemyM_ = Model::CreateFromOBJ("cube");

	enemyO_->SetModel(enemyM_);

	enemy_.translation_ = { 0,0,0 };
	enemyO_->SetPosition(enemy_.translation_);


	sphere.resize(SPHERE_COLISSION_NUM);
	spherePos.resize(SPHERE_COLISSION_NUM);

	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		spherePos[i] = enemyO_->GetPosition();
		sphere[i]->SetBasisPos(&spherePos[i]);
		sphere[i]->SetRadius(1.0f);
		sphere[i]->SetAttribute(COLLISION_ATTR_ENEMYS);
		sphere[i]->Update();
		////test
		//coliderPosTest_[i] = Object3d::Create();
		//coliderPosTest_[i]->SetModel(hpModel_.get());
		//coliderPosTest_[i]->SetPosition(sphere[i]->center);
		//coliderPosTest_[i]->SetScale({ sphere[i]->GetRadius(),sphere[i]->GetRadius() ,sphere[i]->GetRadius() });
		//coliderPosTest_[i]->SetRotate({ 0,0,0 });
		//coliderPosTest_[i]->Update();

	}

}

void Enemy::Update()
{
	GetIsHit();
	Action();
	enemyO_->Update();
}

void Enemy::Draw()
{
	enemyO_->Draw();
}

void Enemy::Action()
{
	if (hitDeley > 0) {	//毎フレームヒットを防止
		enemyO_->SetColor({ 0,0,1,1 });
		hitDeley--;
	}
	else
	{
		enemyO_->SetColor({ 1,1,1,1 });
	}
	
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		if (hitDeley <= 0 && sphere[i]->GetIsHit() == true)
		{
			if (sphere[i]->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_PLAYERBULLETS)
			{
				hitDeley = 30;
				SetIsHit(true);

				break;
			}
		}
		
	}
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		spherePos[i] = enemyO_->GetPosition();
		sphere[i]->Update();
	}

	/*ImGui::Begin("enemyDelay");
	ImGui::SetWindowPos({ 400 , 200 });
	ImGui::SetWindowSize({ 500,100 });
	ImGui::InputInt("isPause", &hitDeley);
	ImGui::DragFloat3("enemyPos",&enemyO_->worldTransform.translation_.x);
	ImGui::End();*/
}
