#include "EnemyIdle.h"

EnemyIdle::EnemyIdle(EnemyActionManager* ActionManager) { ActionManager_ = ActionManager; }

EnemyIdle::~EnemyIdle() {}

void EnemyIdle::Initialize(FbxObject3d* object, PlayerCharacter* player) {
	object_ = object;
	player_ = player;
}

void EnemyIdle::Update() {
	
	
	object_->Update();
}

void EnemyIdle::Draw() {}
