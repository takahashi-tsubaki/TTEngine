#pragma once
#include "Action.h"

class PlayerCharacter;
class EnemyCharacter;

enum STEPDirection {
	LEFTSTEP = 1,
	RIGHTSTEP = 2,
};


class Step : public Action {
protected:
	PlayerActionManager* ActionManager_;

public:
	Step(PlayerActionManager* ActionManager);
	~Step() override;

	void Initialize(FbxObject3d* object, EnemyCharacter* enemy, SceneObjects* sceneObj) override;
	void Update(Input* input, GamePad* gamePad) override;
	void Draw() override;

	void Distance(Vector3 start, Vector3 goal);

	// 2点間の距離
	void DistanceTwoPoints(Vector3 start, Vector3 goal);

private:
	FbxObject3d* object_;
	SceneObjects* sceneObj_;
	Vector3 velocity_{0, 0, 0};
	EnemyCharacter* enemy_ = nullptr;
	Vector3 Distance_{0, 0, 0};

	int stepDirection;
	int stepFlameCount = 30;

	float speed_ = 0.5f;
	float stepSpeed = 0.5f;
	float scale_ = 0.01f;
	const float MAX_POSITION = 100;

	bool isLeft = false;
	bool isRight = false;
	bool isStep = false;
};
