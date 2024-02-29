#pragma once
#include "EnemyAction.h"
class EnemyIdle : public EnemyAction
{

protected:
	EnemyActionManager* ActionManager_;

public:
	EnemyIdle(EnemyActionManager* ActionManager);
	~EnemyIdle() override;

	void Initialize(FbxObject3d* object) override;
	void Update() override;
	void Draw() override;

private:
	FbxObject3d* object_;
	Vector3 velocity_{0, 0, 0};
	float speed = 0.5f;
};
