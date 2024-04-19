#include "TutorialManager.h"
#include "TutorialAction.h"
#include "TutorialIdle.h"
#include "TutorialMove.h"
TutorialManager::TutorialManager()
{
	action_.reset(new TutorialIdle(&*this));
}

TutorialManager::~TutorialManager()
{
}

void TutorialManager::ActionInitialize(FbxObject3d* object, TutorialEnemy* enemy, SceneObjects* sceneObj)
{
	object_ = object;
	enemy_ = enemy;
	sceneObj_ = sceneObj;
	action_.get()->Initialize(object_, enemy_, sceneObj_);
}

void TutorialManager::ActionUpdate(Input* input, GamePad* gamePad)
{

}

void TutorialManager::ActionDraw()
{
}

void TutorialManager::ChangeAction(Action* Action)
{
}

bool TutorialManager::KeyPushWASD(Input* input)
{
	return false;
}

bool TutorialManager::isAnyLStick(GamePad* gamePad)
{
	return false;
}

bool TutorialManager::KeyTriggerArrow(Input* input)
{
	return false;
}

bool TutorialManager::isAnyLStickOffTrigger(GamePad* gamePad)
{
	return false;
}
