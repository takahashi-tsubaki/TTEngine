#pragma once
#include "EnemyAction.h"
class EnemyMove : public EnemyAction
{
protected:
	EnemyActionManager* ActionManager_;

public:
	EnemyMove(EnemyActionManager* ActionManager);
	~EnemyMove() override;

	void Initialize(FbxObject3d* object) override;
	void Update() override;
	void Draw() override;


private:
	FbxObject3d* object_;
	Vector3 velocity_{0, 0, 0};
	float speed = 0.5f;

	bool pushWASD_ = false;
	bool padLStick_ = false;
};
