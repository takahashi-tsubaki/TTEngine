#include "StateManager.h"


StateManager::StateManager(TTEngine::DirectXCommon* dxCommon, Player2* player) {
	dxCommon_ = dxCommon;
	player_ = player;
}

void StateManager::StateInitialize() {

}

void StateManager::StateUpdate(Input* input)
{
	if (input) {
	}
}

void StateManager::StateDraw()
{
	;
}
