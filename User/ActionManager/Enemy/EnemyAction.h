#pragma once
#include "FbxObject3d.h"
#include "GamePad.h"
#include "Input.h"
#include "EnemyActionManager.h"
#include "PlayerCharacter.h"
class EnemyAction {

public:
	EnemyAction();
	virtual ~EnemyAction();

	virtual void Initialize(FbxObject3d* object,PlayerCharacter* player) = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

protected:
	EnemyActionManager* actionManager_;
};
