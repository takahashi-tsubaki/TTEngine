#include "EnemyMove.h"

EnemyMove::EnemyMove(EnemyActionManager* ActionManager)
{
	ActionManager_ = ActionManager;
}

EnemyMove::~EnemyMove() {}

void EnemyMove::Initialize(FbxObject3d* object)
{
	object_ = object;
}

void EnemyMove::Update()
{

	object_->Update();
}

void EnemyMove::Draw() {}


	
