#pragma once

#include "Object3d.h"
#include "Model.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"

class PlayerCharacter;
/// <summary>
/// 敵の弾
/// </summary>
class EnemyBullet
{
public:

	void Initialize(Model* model,const Vector3& position,const Vector3& velocity,const Vector3& Rotate);

	void Update();

	void Draw();

	void Shot();

	void CheckCollision();

	void SetisDead(bool isDead) {
		isDead_ = isDead;
	}

	bool GetIsDead() {
		return isDead_;
	}

	void SetPlayer(PlayerCharacter* player) {
		player_ = player;
	}

	void Reset();


private:


	float livingTimer;

	Vector3 velocity_;

	Object3d* bulletO_ = nullptr;

	bool isDead_;

	PlayerCharacter* player_;

	int SPHERE_COLISSION_NUM = 1;	//コライダー（スフィア）の数
	std::vector<Matrix4>* collisionBonesMat;	//当たり判定用のボーンのワールド行列
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};

	int hitDeley = 0;	//何フレーム連続で当たるか
};

