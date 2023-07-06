#pragma once

#include "Object3d.h"
#include "Model.h"

#include "Enemy.h"

class PlayerBullet
{
public:

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw();

	void Shot();

	void SetisDead(bool isDead) { isDead_ = isDead; }

	bool GetIsDead() { return isDead_; }

	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }

private:

	float livingTimer = 540.0f;

	bool isDead_ = false;

	Vector3 velocity_;

	Object3d* bulletO_ = nullptr;

	Enemy* enemy_ = nullptr;

};

