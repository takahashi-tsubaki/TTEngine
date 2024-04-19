#include "TutorialIdle.h"

TutorialIdle::TutorialIdle(TutorialManager* ActionManager)
{
	ActionManager_ = ActionManager;
}


TutorialIdle::~TutorialIdle()
{
}

void TutorialIdle::Initialize(FbxObject3d* object, TutorialEnemy* enemy, SceneObjects* sceneObj)
{
	object_ = object;
	enemy_ = enemy;
	sceneObj_ = sceneObj;
}

void TutorialIdle::Update(Input* input, GamePad* gamePad)
{
	object_->PlayAnimation(FBXAnimetion::IdeL, true);
	if (gamePad->StickInput(L_UP) || input->PushKey(DIK_W)) {

		/*velocity_ += {0, 0, moveSpeed};

		fbxVelocity_ += {0, 0, (moveSpeed / fbxScale_)};*/
	}

	else
	{

	}
}

void TutorialIdle::Draw()
{

}
