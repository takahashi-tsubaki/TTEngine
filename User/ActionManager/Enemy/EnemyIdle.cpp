#include "EnemyIdle.h"

EnemyIdle::EnemyIdle(EnemyActionManager* ActionManager) { ActionManager_ = ActionManager; }

EnemyIdle::~EnemyIdle() {}

void EnemyIdle::Initialize(FbxObject3d* object) { object_ = object; }

void EnemyIdle::Update() {
	
	
	object_->Update();
}

void EnemyIdle::Draw() {}
