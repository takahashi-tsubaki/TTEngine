#pragma once
#include "FbxObject3d.h"
#include "GamePad.h"
#include "Input.h"
#include "EnemyActionManager.h"
#include "PlayerCharacter.h"

#include "BulletManager.h"

class EnemyAction {

public:
	EnemyAction();
	virtual ~EnemyAction();

	virtual void Initialize(FbxObject3d* object,PlayerCharacter* player) = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

	bool GetIsNowShot() { return isNowShot_; }

	void SetIsNowShot(bool shot) { isNowShot_ = shot; }

protected:
	EnemyActionManager* actionManager_;
	bool isNowShot_;

};
