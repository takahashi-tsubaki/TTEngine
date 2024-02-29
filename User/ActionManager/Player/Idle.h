#pragma once
#include "Action.h"

class PlayerCharacter;
class EnemyCharacter;

class Idle : public Action {
protected:
	PlayerActionManager* ActionManager_;

public:
	Idle(PlayerActionManager* ActionManager);
	~Idle() override;

	void Initialize(FbxObject3d* object, EnemyCharacter* enemy) override;
	void Update(Input* input, GamePad* gamePad) override;
	void Draw() override;

private:

	PlayerCharacter* player_ = nullptr;
	EnemyCharacter* enemy_ = nullptr;
	FbxObject3d* object_;
	Vector3 velocity_{0, 0, 0};
	float speed = 0.5f;
};
