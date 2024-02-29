#pragma once
#include "FbxObject3d.h"
#include "Input.h"
#include "GamePad.h"
#include "PlayerActionManager.h"

#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

class Action {

public:

	Action();
	virtual ~Action();

	virtual void Initialize(FbxObject3d* object, EnemyCharacter* enemy) = 0;

	virtual void Update(Input* input, GamePad* gamePad) = 0;

	virtual void Draw() = 0;

protected:
	PlayerActionManager* actionManager_;
};
