#include "EnemyAction.h"
#include "EnemyIdle.h"
#include "EnemyMove.h"
#include "EnemyActionManager.h"

#include "ImguiManager.h"
	

EnemyActionManager::EnemyActionManager() { action_.reset(new EnemyIdle(&*this)); }

EnemyActionManager::~EnemyActionManager() {}

void EnemyActionManager::ActionInitialize(FbxObject3d* object, PlayerCharacter* player) {
	object_ = object;
	player_ = player;
	action_.get()->Initialize(object_,player_);

}

void EnemyActionManager::ActionUpdate() {
	moveDirectTimer--;
	
	if ( moveDirectTimer < 0 )
	{
		moveDirectTimer = 120;
		
		actionNum_ = generateRandomAction(1, 5);
		
	}

	if ( actionNum_ == 1 )
	{
		ChangeAction(new EnemyIdle(&*this));
	}
	else
	{
		ChangeAction(new EnemyMove(&*this, actionNum_));
	}

	action_.get()->Update();
#ifdef _DEBUG
	ImGui::Begin("action");

	ImGui::SetWindowPos({600, 200});
	ImGui::SetWindowSize({400, 200});
	ImGui::InputInt("actionNum_", &actionNum_);
	ImGui::InputFloat("moveDirectTimer", &moveDirectTimer);
	ImGui::End();
#endif
}

void EnemyActionManager::ActionDraw() { action_.get()->Draw(); }

void EnemyActionManager::ChangeAction(EnemyAction* Action) {
	action_.reset(Action);
	ActionInitialize(object_,player_);
	action_.get()->Initialize(object_, player_);
}

int EnemyActionManager::generateRandomAction(int min, int max)
{
	std::mt19937 gen(random());
	std::uniform_int_distribution<> distrib(min, max);
	return distrib(gen);
}
