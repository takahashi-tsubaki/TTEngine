#pragma once
#include "Character.h"
#include "EnemyActionManager.h"
#include "ParticleManager.h"

#include "EnemyBullet.h"

class PlayerCharacter;
class EnemyCharacter : public Character
{
public:
	// 初期化
	void Initialize(TTEngine::DirectXCommon* dxCommon, Vector3 position, PlayerCharacter* player,SceneObjects* sceneObj);

	// 更新
	void Update();

	// 描画
	void Draw() override;

	void ObjectUpdate();
	void ObjectDraw();

	void Damage();

	void Reset();

		// Fbxオブジェクトの取得
	FbxObject3d* GetFbxObject3d() { return fbxObject_.get(); }

	PlayerCharacter* GetPlayer() { return player_; }
	Vector3 GetOldPos() { return oldPos; }

	void ResetAttribute();

	/// 体力のセット
	void SetHp(int Hp) { Hp_ = Hp; }
	/// 体力のゲット
	int GetHp() { return Hp_; }
	void SetisDead(bool isDead) { isDead_ = isDead; }
	/// <summary>
	/// 死んだかどうかのフラグのゲット
	/// </summary>
	bool GetisDead() { return isDead_; }
	/// <summary>
	/// 当たったかどうかのフラグのセット
	/// </summary>
	/// <param name="isHit"></param>
	void SetIsHit(bool isHit) { isHit_ = isHit; }
	/// <summary>
	/// 当たったかどうかのフラグのゲット
	/// </summary>
	/// <returns></returns>
	bool GetIsHit() { return isHit_; }

	///// <summary>
	///// パーティクルのゲット
	///// </summary>
	///// <returns></returns>
	 ParticleManager* GetParticle() {
		return particle_.get();
	 }

	int GetDamageSize() { return damageSize_; }
	/// 体力のセット
	void SetDamageSize(int damageSize) {
		damageSize_ = damageSize;
	}

	void CheckHitCollision();

	void IsDeadAnime();

	void SetAttribute();

private:
	std::unique_ptr<EnemyActionManager> ActManager_;
	PlayerCharacter* player_ = nullptr;

	Vector3 distance_;
	Vector3 oldPos;
	Vector3 transNormal;

	int Hp_;
	int damage = 1;
	int damageSize_ = 0;
	int decreaseHpCount = 60;
	int hitDeley = 0; // 何フレーム連続で当たるか

	// プレイヤーのFBXモデル
	// fbxの大きさ
	float fbxScale_;
	int blowAwayCount;
	bool isDamage = false;
	bool isDead_ = false;
	bool isHit_ = false;

	size_t SPHERE_COLISSION_NUM;             // コライダー（スフィア）の数
	std::vector<Matrix4>* collisionBonesMat; // 当たり判定用のボーンのワールド行列
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};

	ObjParticleManager* particleObj_;
	//
	std::unique_ptr<ParticleManager> particle_;
};
