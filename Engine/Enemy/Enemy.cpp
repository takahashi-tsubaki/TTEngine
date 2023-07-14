#include "Enemy.h"
#include "ImguiManager.h"
#include "Player.h"
void Enemy::Initialize(DirectXCommon* dxCommon, Player* player)
{

	dxCommon_ = dxCommon;

	player_ = player;

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

	wtf.translation_ = { 0,0,0 };
	enemyO_->SetPosition(wtf.translation_);


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
	CheckHitCollision();
	Move();

	enemyO_->UpdateMatrix();

	class EnemyQueryCallBack : public QueryCallback
	{
	public:
		EnemyQueryCallBack(Sphere* sphere) : sphere(sphere) {};

		bool OnQueryHit(const QueryHit& info)
		{
			rejectDir = info.reject;
			rejectDir.nomalize();

			//上方向と排斥方向の角度差のコサイン値
			float cos = rejectDir.dot(up);

			//地面判定しきい値角度
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
		void SphereQuery();

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
		EnemyQueryCallBack callback(sphere[i]);

		//球と地形の交差を全探索する
		CollisionManager::GetInstance()->QuerySphere(*sphere[i], &callback);

		wtf.translation_.x += callback.move.x;
		wtf.translation_.y += callback.move.y;
		wtf.translation_.z += callback.move.z;

		enemyO_->SetPosition(wtf.translation_);
		enemyO_->UpdateMatrix();
		sphere[i]->Update();
	}


	enemyO_->Update();

	/*ImGui::Begin("enemyDelay");
	ImGui::SetWindowPos({ 400 , 200 });
	ImGui::SetWindowSize({ 500,100 });
	ImGui::InputInt("isPause", &hitDeley);
	ImGui::DragFloat3("enemyPos",&enemyO_->worldTransform.translation_.x);
	ImGui::End();*/
}

void Enemy::Draw()
{
	enemyO_->Draw();
}

void Enemy::Action()
{
	


}

void Enemy::CheckHitCollision()
{
	oldPos = wtf.translation_;

	Vector3 distance;

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
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		if (sphere[i]->GetIsHit() == true)
		{
			if (sphere[i]->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_PLAYERS)
			{
				wtf.translation_ = oldPos;
				enemyO_->SetPosition(wtf.translation_);
				break;
			}
		}

	}
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		spherePos[i] = enemyO_->GetPosition();
		sphere[i]->Update();
	}
}

void Enemy::Attack()
{
}

void Enemy::Vanish()
{
}

void Enemy::Move()
{
	//１秒に一回実行する1
	flameCount++;
	if (flameCount < 30)
	{
		srand(time(nullptr));
		moveActionNum = rand() % 3+1;
		flameCount = 0;
	}
	
	if (moveActionNum == 1)
	{
		isLeft = true;
		isRight = false;
		isApproach = false;
	}
	else if(moveActionNum == 2)
	{
		isLeft = false;
		isRight = true;
		isApproach = false;
	}
	else
	{
		isLeft = false;
		isRight = false;
		isApproach = true;
	}

	if (isLeft == true)
	{
		wtf.translation_.x -= 0.5f;
		enemyO_->SetPosition(wtf.translation_);
	}
	if (isRight == true)
	{
		wtf.translation_.x += 0.5f;
		enemyO_->SetPosition(wtf.translation_);
	}
	if (isApproach == true)
	{
		enemyPos = enemyO_->GetPosition();
		distance = player_->GetPosition()-enemyO_->GetPosition()  ;
		distance.nomalize();
		distance *= 0.5f;
		wtf.translation_ += distance;
		enemyO_->SetPosition(wtf.translation_);
	}
}

void Enemy::Step()
{
}
