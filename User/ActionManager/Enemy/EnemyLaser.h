#pragma once
#include "EnemyAction.h"
class PlayerCharacter;

class EnemyLaser : public EnemyAction
{
protected:

	EnemyActionManager* ActionManager_;
public:

	EnemyLaser(EnemyActionManager* ActionManager);
	~EnemyLaser() override;

	void Initialize(FbxObject3d* object,PlayerCharacter* player) override;
	void Update() override;
	void Draw() override;

	void Distance(Vector3 start,Vector3 goal);

	// 2点間の距離
	void DistanceTwoPoints(Vector3 start,Vector3 goal);

	void ChargeReset();


private:

	FbxObject3d* object_;
	PlayerCharacter* player_;
	Vector3 velocity_{ 0, 0, 0 };
	Vector3 Distance_{ 0, 0, 0 };
	Vector3 playerPos = { 0,0,0 };
	Vector3 enemyPos = { 0,0,0 };

	float speed_;

	float chargeCount = 90.0f;

	int moveDirect_ = 0;

	int MaxBulletSize_ = 1;
	int bulletSize_ = 0;

	//撃ったかどうか
	bool isShot_ = false;
};

