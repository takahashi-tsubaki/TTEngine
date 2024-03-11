#pragma once
#include "EnemyAction.h"

class PlayerCharacter;

class EnemyMove : public EnemyAction
{
protected:
	EnemyActionManager* ActionManager_;

public:
	EnemyMove(EnemyActionManager* ActionManager,int moveDirect);
	~EnemyMove() override;

	void Initialize(FbxObject3d* object, PlayerCharacter* player) override;
	void Update() override;
	void Draw() override;

	void Distance(Vector3 start, Vector3 goal);

	// 2点間の距離
	void DistanceTwoPoints(Vector3 start, Vector3 goal);

private:
	FbxObject3d* object_;
	PlayerCharacter* player_;
	Vector3 velocity_{0, 0, 0};
	Vector3 Distance_{0, 0, 0};

	
	float speed_ = 0.5f;
	float scale_ = 0.01f;

	int moveDirect_ = 0;

	bool pushWASD_ = false;
	bool padLStick_ = false;
};
