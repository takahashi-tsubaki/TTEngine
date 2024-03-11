#include "EnemyMove.h"
#include "PlayerCharacter.h"
EnemyMove::EnemyMove(EnemyActionManager* ActionManager, int moveDirect) {

	moveDirect_ = moveDirect;
	ActionManager_ = ActionManager;
}

EnemyMove::~EnemyMove() {}

void EnemyMove::Initialize(FbxObject3d* object, PlayerCharacter* player) {

	object_ = object;
	player_ = player;
	player_->GetFbxObject3d();
}

void EnemyMove::Update()
{
	// 2点間の距離
	DistanceTwoPoints(object_->GetPosition(), player_->GetFbxObject3d()->GetPosition());
	if (moveDirect_ == 2) {
		velocity_ += {0, 0, (speed_ / scale_)};
	}
	else if (moveDirect_ == 3) {
		velocity_ += {0, 0, (speed_ / scale_)*-1};
	}
	 else if (moveDirect_ == 4) {
		velocity_ += {(speed_ / scale_), 0, 0};
	}
	 else if (moveDirect_ == 5) {
		velocity_ += {(speed_ / scale_) * -1, 0, 0};
	}

	object_->worldTransform.UpdateMatWorld();

	velocity_ = MyMath::MatVector(velocity_, object_->worldTransform.matWorld_);

	object_->worldTransform.translation_ += velocity_;

	object_->SetPosition(object_->worldTransform.translation_);
}

void EnemyMove::Draw() {}


void EnemyMove::Distance(Vector3 start, Vector3 goal) {
	Distance_ = {goal.x - start.x, goal.y - start.y, goal.z - start.z};
}

void EnemyMove::DistanceTwoPoints(Vector3 start, Vector3 goal) {
	Distance(start, goal);
	Distance_.z = sqrtf(pow(Distance_.x, 2.0f) + pow(Distance_.z, 2.0f));
}

	
