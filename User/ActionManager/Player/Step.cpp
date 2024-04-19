#include "Step.h"

Step::Step(PlayerActionManager* ActionManager)
{
	ActionManager_ = ActionManager;
	isNowStep_ = true;
}

Step::~Step()
{
	//isLeft = false;
	//isRight = false;
	//isStep = false;
	//stepFlameCount = 30;
}

void Step::Initialize(FbxObject3d* object, EnemyCharacter* enemy, SceneObjects* sceneObj) {
	object_ = object;
	enemy_ = enemy;
	sceneObj_ = sceneObj;
	// 敵のFbxの取得
	enemy_->GetFbxObject3d();
}

void Step::Update(Input* input, GamePad* gamePad)
{
	//2点間の距離
	DistanceTwoPoints(object_->GetPosition(), enemy_->GetFbxObject3d()->GetPosition());

	// 左スティックを弾いたとき
	if (gamePad->StickOffTrigger(L_LEFT) || input->TriggerKey(DIK_LEFT))
	{

		if (isStep == false)
		{
			object_->PlayAnimation(FBXAnimetion::LeftSTEP, false);
			stepFlameCount = 30;
			stepDirection = STEPDirection::LEFTSTEP;
			stepSpeed = 2.0f * -1;
			isStep = true;
		}
		
	}
	// 右スティックを弾いたとき
	if (gamePad->StickOffTrigger(L_RIGHT) || input->TriggerKey(DIK_RIGHT))
	{
		if (isStep == false)
		{
			object_->PlayAnimation(FBXAnimetion::RightSTEP, false);
			stepFlameCount = 30;
			stepDirection = STEPDirection::RIGHTSTEP;
			stepSpeed = 2.0f;
			isStep = true;
		}
		
	}
	if (isStep == true)
	{
		velocity_ += {(stepSpeed / scale_), 0, 0};
		// velocity_ += {StepSpeed, 0, 0};
		stepFlameCount--;
	}

	if (stepDirection == STEPDirection::LEFTSTEP) {
		stepSpeed += 0.1f;

		if (stepSpeed >= 0.0f) {
			stepSpeed = 0.0f;
		}
		if (stepFlameCount <= 0) {
			isNowStep_ = false;
			isNowStandBy_ = true;
			isStep = false;
		}
	}
	if (stepDirection == STEPDirection::RIGHTSTEP) {
		stepSpeed -= 0.1f;

		if (stepSpeed <= 0.0f) {
			stepSpeed = 0.0f;
		}
		if (stepFlameCount <= 0) {
			isNowStep_ = false;
			isNowStandBy_ = true;
			isStep = false;
		}
	}


	object_->worldTransform.UpdateMatWorld();

	velocity_ = MyMath::MatVector(velocity_, object_->worldTransform.matWorld_);

	object_->worldTransform.translation_ += velocity_;

	object_->SetPosition(object_->worldTransform.translation_);

	// playerO_->worldTransform.translation_ += fbxVelocity_;

	// playerO_->SetPosition(playerO_->worldTransform.translation_);
}

void Step::Draw() {}

void Step::ParticleDraw(ID3D12GraphicsCommandList* cmdList) {
	if (cmdList) {
	}
}

void Step::Distance(Vector3 start, Vector3 goal)
{
	Distance_ = {goal.x - start.x, goal.y - start.y, goal.z - start.z};
}

void Step::DistanceTwoPoints(Vector3 start, Vector3 goal)
{
	Distance(start, goal);
	Distance_.z = sqrtf(pow(Distance_.x, 2.0f) + pow(Distance_.z, 2.0f));
}
