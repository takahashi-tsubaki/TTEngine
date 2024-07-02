#include "EnemyMove.h"
#include "PlayerCharacter.h"
EnemyMove::EnemyMove(EnemyActionManager* ActionManager, int moveDirect)
{

	moveDirect_ = moveDirect;
	ActionManager_ = ActionManager;

}

EnemyMove::~EnemyMove() {}

void EnemyMove::Initialize(FbxObject3d* object, PlayerCharacter* player)
{

	object_ = object;
	player_ = player;
	player_->GetFbxObject3d();

}

void EnemyMove::Update()
{

	// 2点間の距離
	DistanceTwoPoints(player_->GetFbxObject3d()->GetPosition(), object_->GetPosition());

	//後ろ方向
	if (moveDirect_ == 2 || moveDirect_ > 4) {
		// 移動限界に達していない時
		if (Distance_.z <= MAX_POSITION) {
			backSpeed = 0.5f;
			isMoveLimit = false;
		} else // 達しているとき
		{
			backSpeed = 0;
			isMoveLimit = true;
		}
		object_->PlayAnimation(FBXAnimetion::BackStep,false);
		// 上記のうちどちらか小さい値の方をspeedに代入
		backSpeed = min((MAX_POSITION - Distance_.z), 0.5f);
		// speedを加算
		velocity_ += {0, 0, (backSpeed / scale_)};
	}
	//右方向
	 else if (moveDirect_ == 3) {
		object_->PlayAnimation(FBXAnimetion::RightMOVE,false);
		velocity_ += {(speed_ / scale_), 0, 0};
	}
	//左方向
	 else if (moveDirect_ == 4) {
		object_->PlayAnimation(FBXAnimetion::LeftMOVE,false);
		velocity_ += {(speed_ / scale_) * -1, 0, 0};
	}

	//行列更新
	object_->worldTransform.UpdateMatWorld();

	velocity_ = MyMath::MatVector(velocity_, object_->worldTransform.matWorld_);

	object_->worldTransform.translation_ += velocity_;

	object_->SetPosition(object_->worldTransform.translation_);

}

void EnemyMove::Draw() {}


void EnemyMove::Distance(Vector3 start, Vector3 goal)
{

	Distance_ = {goal.x - start.x, goal.y - start.y, goal.z - start.z};

}

void EnemyMove::DistanceTwoPoints(Vector3 start, Vector3 goal)
{

	Distance(start, goal);
	Distance_.z = sqrtf(pow(Distance_.x, 2.0f) + pow(Distance_.z, 2.0f));

}

	
