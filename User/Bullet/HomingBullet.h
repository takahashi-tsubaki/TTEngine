#pragma once

#include "Object3d.h"
#include "Model.h"

#include "ParticleManager.h"

#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "SphereCollider.h"

class EnemyCharacter;
class PlayerCharacter;

enum BulletTimer
{
	UpTime = 60,
	HomingTime = 120,
	LivingTime = 150,

};

enum ShotPhase
{
	Fire ,
	Homing ,
};

class HomingBullet {
public:
	HomingBullet();
	~HomingBullet();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	/// <param name="velocity"></param>
	void Initialize(
	    const Vector3& position, PlayerCharacter* player, const Vector3& velocity,
	    const unsigned short attribute);

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
	void SetisDead(bool isDead) { isDead_ = isDead; }
	/// <summary>
	/// deadフラグのゲット
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() { return isDead_; }

	/// <summary>
	/// 敵のセット
	/// </summary>
	/// <param name="enemy"></param>
	void SetEnemy(EnemyCharacter* enemy) { enemy_ = enemy; }

	void SetPlayer(PlayerCharacter* player) { player_ = player; }

	/// <summary>
	/// リセット処理
	/// </summary>
	void Reset();

	static void SetModel(Model* model) { HomingBullet::model_ = model; }

	///// <summary>
	///// パーティクルのゲット
	///// </summary>
	///// <returns></returns>
	// ParticleManager* GetParticle() { return particle_.get(); }

	Vector3 GetPosition() { return bulletO_->GetPosition(); }

	void Fire();

	void Homing();

private:
	float livingTimer;

	float upTimer;

	float homingTimer;
		//ホーミング精度
	float homingAccuary_ = 0;

	bool isDead_ = false;

	const float change = 1.0f;

	float kBulletSpeed = change;

	int kHomingTimer_ = 0;

	int phase = ShotPhase::Fire;
	


	int kFireTimer = 0;

	int kStartHomingTime = 30;

	Vector3 angle = {0 , 0 , 1};

	Vector3 velocity_;
	Vector3 Distance_;
	Vector3 moveVelocity;
	Vector3 currentPlayerPos_;
	Vector3 playerPos_;

	Object3d* bulletO_ = nullptr;
	// Model* bulletM_ = nullptr;
	EnemyCharacter* enemy_ = nullptr;
	PlayerCharacter* player_ = nullptr;

	int SPHERE_COLISSION_NUM = 1; // コライダー（スフィア）の数
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};
	// std::vector<Matrix4>* collisionBonesMat;	//当たり判定用のボーンのワールド行列

	//
	std::unique_ptr<ParticleManager> particle_;

	int hitDeley = 0; // 何フレーム連続で当たるか

	float angle_ = 0;
	float speed = 1.0f;

	static Model* model_;
};


