#pragma once

#include "Object3d.h"
#include "Model.h"

#include "ParticleManager.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"

class EnemyCharacter;
class PlayerCharacter;

class Laser
{
public:
	void Initialize(const Vector3& position,const Vector3& velocity,const unsigned short attribute);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 射撃
	/// </summary>
	void LaserShot();

	/// <summary>
	/// 衝突したかのチェック
	/// </summary>
	void CheckCollision();
	/// <summary>
	/// deadフラグのセット
	/// </summary>
	/// <param name="isDead"></param>
	void SetisDead(bool isDead) {
		isDead_ = isDead;
	}
/// <summary>
/// deadフラグのゲット
/// </summary>
/// <returns></returns>
	bool GetIsDead() {
		return isDead_;
	}

/// <summary>
/// 敵のセット
/// </summary>
/// <param name="enemy"></param>
	void SetEnemy(EnemyCharacter* enemy) {
		enemy_ = enemy;
	}

	void SetPlayer(PlayerCharacter* player) {
		player_ = player;
	}

	static void SetModel(Model* model) {
		Laser::model_ = model;
	}

private:

	float livingTimer;

	Vector3 position_;
	Vector3 velocity_;
	float angle_;
	unsigned short attribute_;
	bool isDead_ = false;

	Object3d* bulletO_ = nullptr;
	static Model* model_;


	EnemyCharacter* enemy_;
	PlayerCharacter* player_;

	int SPHERE_COLISSION_NUM = 1; // コライダー（スフィア）の数
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};
	// std::vector<Matrix4>* collisionBonesMat;	//当たり判定用のボーンのワールド行列

	//
	std::unique_ptr<ParticleManager> particle_;

	int hitDeley = 0; // 何フレーム連続で当たるか
};

