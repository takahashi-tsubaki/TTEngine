#include "Action.h"
#include "Idle.h"
#include "Move.h"
#include "Shot.h"
#include "Step.h"
#include "PlayerActionManager.h"
#include "EnemyCharacter.h"

PlayerActionManager::PlayerActionManager() { action_.reset(new Idle(&*this)); }

PlayerActionManager::~PlayerActionManager() {}

void PlayerActionManager::ActionInitialize(
    FbxObject3d* object, EnemyCharacter* enemy,SceneObjects* sceneObj) {
	object_ = object;
	enemy_ = enemy;
	sceneObj_ = sceneObj;
	action_.get()->Initialize(object_,enemy_,sceneObj_);
}

void PlayerActionManager::ActionUpdate(Input* input, GamePad* gamePad)
{

	//移動
	if (KeyPushWASD(input) == true || isAnyLStick(gamePad)==true)
	{
		if ( action_->GetIsNowShot() == false && action_->GetIsNowStep()==false )
		{
			action_->SetIsStandBy(false);
			ChangeAction(new Move(&*this));
		}
		
	}
	//射撃
	if ( input->TriggerKey(DIK_SPACE) || gamePad->ButtonTrigger(A) )
	{
		if (action_->GetIsNowStep() == false)
		{
			if (action_->GetIsNowShot() == false) {
				action_->SetIsNowShot(true);
			}
			action_->SetIsStandBy(false);
			ChangeAction(new Shot(&*this));
		}
		
	}
	//ステップ移動
	if ( KeyTriggerArrow(input) == true || isAnyLStickOffTrigger(gamePad) == true )
	{
		if ( action_->GetIsNowShot() == false )
		{
			if ( action_->GetIsNowStep() == false )
			{
				action_->SetIsNowStep(true);
			}
			action_->SetIsStandBy(false);
			ChangeAction(new Step(&*this));
		}
	}
	if (action_->GetIsStandBy() == true)
	{
		ChangeAction(new Idle(&*this));
	}
	action_.get()->Update(input, gamePad);
}

void PlayerActionManager::ActionDraw() {
	action_.get()->Draw();
}

void PlayerActionManager::ChangeAction(Action* Action) {
	action_.reset(Action);
	ActionInitialize(object_, enemy_, sceneObj_);
	action_.get()->Initialize(object_, enemy_, sceneObj_);
}


bool PlayerActionManager::KeyPushWASD(Input* input) {
	if (input->PushKey(DIK_W) || input->PushKey(DIK_A) || input->PushKey(DIK_S) || input->PushKey(DIK_D)) {
		return true;
	}
	else
	{
		return false;
	}
}

bool PlayerActionManager::isAnyLStick(GamePad* gamePad) {
	if (gamePad->StickInput(L_UP) || gamePad->StickInput(L_LEFT) || gamePad->StickInput(L_DOWN) || gamePad->StickInput(L_RIGHT)) {
		return true;
	}
	else
	{
		return  false;
	}
}

bool PlayerActionManager::KeyTriggerArrow(Input* input)
{
	if ( input->TriggerKey(DIK_LEFT) || input->TriggerKey(DIK_RIGHT) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PlayerActionManager::isAnyLStickOffTrigger(GamePad* gamePad)
{
	if ( gamePad->StickOffTrigger(L_LEFT) || gamePad->StickOffTrigger(L_RIGHT) )
	{
		return true;
	}
	else
	{
		return false;
	}

}
