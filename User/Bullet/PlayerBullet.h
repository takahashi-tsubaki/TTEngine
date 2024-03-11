#pragma once

#include "Object3d.h"
#include "Model.h"



#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"

class EnemyCharacter;
/// <summary>
/// 自機の弾
/// </summary>
class PlayerBullet
{
public:
	PlayerBullet();
	~PlayerBullet();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	/// <param name="velocity"></param>
	void Initialize(const Vector3& position,const Vector3& velocity);

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
	void Shot();

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

	/// <summary>
	/// リセット処理
	/// </summary>
	void Reset();

private:

	float livingTimer;

	bool isDead_ = false;

	Vector3 velocity_;

	Object3d* bulletO_ = nullptr;
	Model* bulletM_ = nullptr;
	EnemyCharacter* enemy_ = nullptr;

	int SPHERE_COLISSION_NUM = 1;	//コライダー（スフィア）の数
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};
	//std::vector<Matrix4>* collisionBonesMat;	//当たり判定用のボーンのワールド行列

	int hitDeley = 0;	//何フレーム連続で当たるか

	float angle_ = 0;
};

