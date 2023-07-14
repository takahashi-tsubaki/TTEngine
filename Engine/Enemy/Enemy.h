#pragma once

#include "DirectXCommon.h"

#include "worldTransform.h"

#include "Object3d.h"
#include "Model.h"

#include "FbxObject3d.h"
#include "FbxModel.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"

class Player;

class Enemy
{
public:
	WorldTransform wtf;

public:

	void Initialize(DirectXCommon* dxCommon, Player* player);

	void Update();

	void Draw();

	void Action();

	Vector3 GetPosition() { return wtf.translation_; }

	void SetIsHit(bool isHit) { isHit_ = isHit; }

	bool GetIsHit() { return isHit_; }

	void CheckHitCollision();

#pragma region 敵のStateパターン行動の仮のもの

	//敵の攻撃関数
	void Attack();

	//敵の回避関数
	void Vanish();

	void Move();

	void Step();

#pragma endregion


private:

#pragma region 敵のStateパターン行動の仮のもの

	Vector3 oldPos;
	Vector3 playerPos;
	Vector3 enemyPos;
	Vector3 distance;

	//各行動のフラグ
	bool isAttack = false;
	bool isVanish = false;
	bool isMove   = false;

	bool isLeft = false;
	bool isRight = false;
	bool isApproach = false;

	//行動パターンの格納変数
	int actionNum = 0;
	//移動パターンの格納変数
	int moveActionNum = 0;

	int flameCount = 0;



#pragma endregion

	bool isHit_ = false;
	DirectXCommon* dxCommon_ = nullptr;
	//仮置きプレイヤーのモデル
	Object3d* enemyO_ = nullptr;
	Model* enemyM_ = nullptr;

	//敵のFBXモデル
	std::unique_ptr<FbxObject3d> enemyFbxO_;
	std::unique_ptr<FbxModel> enemyFbxM_;

	int SPHERE_COLISSION_NUM = 1;	//コライダー（スフィア）の数
	std::vector<Matrix4>* collisionBonesMat;	//当たり判定用のボーンのワールド行列
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};

	int hitDeley = 0;	//何フレーム連続で当たるか

	Player* player_ = nullptr;
};

