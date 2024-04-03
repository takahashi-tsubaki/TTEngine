#pragma once
#include "EnemyAction.h"
#include <list>
#include <memory>
enum EnemyShotType
{
	NONE,
	ONESHOT,
	RAPIDSHOT,
};

class PlayerCharacter;
class EnemyCharacter;

class EnemyShot : public EnemyAction
{
protected:

	EnemyActionManager* ActionManager_;

public:
	EnemyShot(EnemyActionManager* ActionManager, int actionNum);
	~EnemyShot() override;

	void StaticInitialize();

	void Initialize(FbxObject3d* object, PlayerCharacter* enemy) override;
	void Update() override;
	void Draw() override;

	void Reset();

	// std::list<std::unique_ptr<PlayerBullet>> GetBulletList() { return bullets_; }

	// void Distance(Vector3 start, Vector3 goal);

	//// 2点間の距離
	// void DistanceTwoPoints(Vector3 start, Vector3 goal);

private:
	FbxObject3d* object_;
	Vector3 velocity_{0, 0, 0};
	PlayerCharacter* player_ = nullptr;
	EnemyCharacter* enemy_ = nullptr;
	Vector3 Distance_{0, 0, 0};
	Vector3 enemyPos;
	Vector3 playerPos;

	Model* bulletM_ = nullptr;

	int MaxBulletSize_;
	int bulletSize_;
	int shotType_;
	int actionNum_;

	float speed_;
	float scale_ = 0.01f;
	float bulletSizeUpTimer_;
	float bulletCoolTimer_;

	float koutyokuTimer_ = 120.0f;

	bool isShot = false;

};
