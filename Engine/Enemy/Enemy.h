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

	void Initialize(DirectXCommon* dxCommon);

	void Update();

	void Draw();

	void Action();

	Vector3 GetPosition() { return enemy_.translation_; }

	void SetIsHit(bool isHit) { isHit_ = isHit; }

	bool GetIsHit() { return isHit_; }

private:

	bool isHit_ = false;


	DirectXCommon* dxCommon_ = nullptr;

	WorldTransform enemy_;

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
};

