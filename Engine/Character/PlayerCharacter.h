#pragma once
#include "Character.h"
#include "PlayerActionManager.h"

class EnemyCharacter;

class PlayerCharacter : public Character
{
public:

	//初期化
	void Initialize(TTEngine::DirectXCommon* dxCommon, Vector3 position, EnemyCharacter* enemy);

	//更新
	void Update(Input* input, GamePad* gamePad);

	//描画
	void Draw() override;

	EnemyCharacter* GetEnemy() {return enemy_;}

	Vector3 GetDistance() { return distance_; }

private:

	Vector3 distance_;

	std::unique_ptr<PlayerActionManager> ActManager_;
	EnemyCharacter* enemy_ = nullptr;

	size_t SPHERE_COLISSION_NUM;             // コライダー（スフィア）の数
	std::vector<Matrix4>* collisionBonesMat; // 当たり判定用のボーンのワールド行列
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};

};
