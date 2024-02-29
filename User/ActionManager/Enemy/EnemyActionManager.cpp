#include "EnemyAction.h"
#include "EnemyIdle.h"
#include "EnemyActionManager.h"

EnemyActionManager::EnemyActionManager() { action_.reset(new EnemyIdle(&*this)); }

EnemyActionManager::~EnemyActionManager() {}

void EnemyActionManager::ActionInitialize(FbxObject3d* object) {
	object_ = object;
	action_.get()->Initialize(object_);
}

void EnemyActionManager::ActionUpdate() {
	
	action_.get()->Update();
}

void EnemyActionManager::ActionDraw() { action_.get()->Draw(); }

void EnemyActionManager::ChangeAction(EnemyAction* Action) {
	action_.reset(Action);
	ActionInitialize(object_);
	action_.get()->Initialize(object_);
}
