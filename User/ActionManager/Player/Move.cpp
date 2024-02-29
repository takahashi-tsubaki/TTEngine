#include "Move.h"
#include "Affin.h"
#include "Ease.h"
#include "MyMath.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

Move::Move(PlayerActionManager* ActionManager)
{
	ActionManager_ = ActionManager;
}

Move::~Move() {}

void Move::Initialize(FbxObject3d* object, EnemyCharacter* enemy) {
	object_ = object;
	enemy_ = enemy;
	//敵のFbxの取得
	enemy_->GetFbxObject3d();
}

void Move::Update(Input* input, GamePad* gamePad) {

	//enemy_ = player_->GetEnemy();
	//Distance = enemy_->GetFbxObject3d()->GetPosition() - object_->GetPosition();

	//2点間の距離(この書き方気持ち悪い)
	DistanceTwoPoints(object_->GetPosition(), enemy_->GetFbxObject3d()->GetPosition());


	
	if (input->PushKey(DIK_W) || gamePad->StickInput(L_UP)) {

		velocity_ += {0, 0, (speed_ / scale_)};
		
	}
	if (input->PushKey(DIK_A) || gamePad->StickInput(L_LEFT)) {

		velocity_ += {(speed_ / scale_)*-1, 0, 0};
		//	左移動のアニメーションの再生
		object_->PlayAnimation(FBXAnimetion::LeftMOVE, false);

	}
	if (input->PushKey(DIK_S) || gamePad->StickInput(L_DOWN)) {
		if (Distance_.z <= MAX_POSITION)
		{
			backSpeed = 0.5f;
			isMoveLimit = false;
		} else // 達しているとき
		{
			backSpeed = MAX_POSITION - Distance_.z;
			isMoveLimit = true;

		}
		// 上記のうちどちらか小さい値の方をspeedに代入
		backSpeed = min((MAX_POSITION - Distance_.z), 0.5f);
		// speedを加算
		velocity_ += {0, 0, (backSpeed / scale_) * -1};
	}
	if (input->PushKey(DIK_D) || gamePad->StickInput(L_RIGHT)) {
		velocity_ += {(speed_ / scale_), 0,0};
		//	左移動のアニメーションの再生
		object_->PlayAnimation(FBXAnimetion::RightMOVE, false);
	}

	object_->worldTransform.UpdateMatWorld();

	velocity_ = MyMath::MatVector(velocity_, object_->worldTransform.matWorld_);

	object_->worldTransform.translation_ += velocity_;

	object_->SetPosition(object_->worldTransform.translation_);
}

void Move::Draw() {}

void Move::Distance(Vector3 start, Vector3 goal)
{
	Distance_ = {goal.x - start.x, goal.y- start.y, goal.z - start.z};
}

void Move::DistanceTwoPoints(Vector3 start, Vector3 goal)
{
	Distance(start, goal);
	Distance_.z = sqrtf(pow(Distance_.x, 2.0f) + pow(Distance_.z, 2.0f));
}


