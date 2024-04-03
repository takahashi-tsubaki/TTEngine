#pragma once
#include "Character.h"
#include "PlayerActionManager.h"
#include "PlayerBullet.h"

class SceneObjects;

class EnemyCharacter;

class PlayerCharacter : public Character
{
public:

	//初期化
	void Initialize(TTEngine::DirectXCommon* dxCommon, Vector3 position, EnemyCharacter* enemy,SceneObjects* sceneObj);

	//更新
	void Update(Input* input, GamePad* gamePad);

	//描画
	void Draw() override;

	EnemyCharacter* GetEnemy() {return enemy_;}

	Vector3 GetDistance() { return distance_; }

	// Hpのsetterとgetter
	void SetHp(int Hp) { Hp_ = Hp; }
	int GetHp() { return Hp_; }

	// 生きているかのsetterとgetter
	void SetisDead(bool isDead) { isDead_ = isDead; }
	bool GetisDead() { return isDead_; }

	Vector3 GetOldPos() { return oldPos; }

	void Reset();

	void ResetAttribute();
	void RemoveAttribute();

	void CheckHitCollision();

	ParticleManager* GetParticle() { return particle_.get(); }

	void GetSceneObj(SceneObjects* sceneObj) { sceneObj_ = sceneObj; }

private:

	int Hp_;
	int hitDeley = 0; // 何フレーム連続で当たるか

	float hitCountTime = 60;

	bool isDead_;
	bool isDamage = false; // 自機がダメージを受けているかどうか

	Vector3 distance_;
	Vector3 oldPos;

	std::unique_ptr<PlayerActionManager> ActManager_;
	EnemyCharacter* enemy_ = nullptr;

	//// プレイヤーの弾モデル関連
	//std::list<std::unique_ptr<PlayerBullet>> bullets_;

	size_t SPHERE_COLISSION_NUM;             // コライダー（スフィア）の数
	std::vector<Matrix4>* collisionBonesMat; // 当たり判定用のボーンのワールド行列
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};

	std::unique_ptr<ParticleManager> particle_;


	SceneObjects* sceneObj_;
};
