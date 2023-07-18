#pragma once

#include "Object3d.h"
#include "Model.h"



#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"

class Enemy;

class PlayerBullet
{
public:

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw();

	void Shot();

	void CheckCollision();

	void SetisDead(bool isDead) { isDead_ = isDead; }

	bool GetIsDead() { return isDead_; }

	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }


private:

	float livingTimer = 540.0f;

	bool isDead_ = false;

	Vector3 velocity_;

	Object3d* bulletO_ = nullptr;

	Enemy* enemy_ = nullptr;

	int SPHERE_COLISSION_NUM = 1;	//コライダー（スフィア）の数
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};
	//std::vector<Matrix4>* collisionBonesMat;	//当たり判定用のボーンのワールド行列

	int hitDeley = 0;	//何フレーム連続で当たるか
};

