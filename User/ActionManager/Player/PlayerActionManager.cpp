#include "Action.h"
#include "Idle.h"
#include "Move.h"
#include "Shot.h"
#include "PlayerActionManager.h"
#include "EnemyCharacter.h"

PlayerActionManager::PlayerActionManager() { action_.reset(new Idle(&*this)); }

PlayerActionManager::~PlayerActionManager() {}

void PlayerActionManager::ActionInitialize(
    FbxObject3d* object, EnemyCharacter* enemy) {
	object_ = object;
	enemy_ = enemy;
	action_.get()->Initialize(object_,enemy_);
}

void PlayerActionManager::ActionUpdate(Input* input, GamePad* gamePad)
{
	//移動
	if (KeyPushWASD(input) == true || isAnyLStick(gamePad)==true)
	{
		ChangeAction(new Move(&*this));
	}
	//射撃
	if ( input->TriggerKey(DIK_SPACE) || gamePad->ButtonTrigger(A) )
	{
		ChangeAction(new Shot(&*this));
	}

	action_.get()->Update(input, gamePad);
}

void PlayerActionManager::ActionDraw() {
	action_.get()->Draw();
}

void PlayerActionManager::ChangeAction(Action* Action) {
	action_.reset(Action);
	ActionInitialize(object_, enemy_);
	action_.get()->Initialize(object_, enemy_);
}


bool PlayerActionManager::KeyPushWASD(Input* input) {
	if (input->PushKey(DIK_W) || input->PushKey(DIK_A) || input->PushKey(DIK_S) || input->PushKey(DIK_D)) {
		return true;
	} else {
		return false;
	}
}

bool PlayerActionManager::isAnyLStick(GamePad* gamePad) {
	if (gamePad->StickInput(L_UP) || gamePad->StickInput(L_LEFT) || gamePad->StickInput(L_DOWN) || gamePad->StickInput(L_RIGHT)) {
		return true;
	} else {
		return  false;
	}
}
