#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "GamePad.h"

#include "worldTransform.h"
#include "Object3d.h"
#include "Model.h"

#include "FbxObject3d.h"
#include "FbxModel.h"

#include "PlayerBullet.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"

class Enemy;

class Player
{
public:

	void Initialize(DirectXCommon* dxCommon, Input* input, GamePad* gamePad, Enemy* enemy);

	void Update();

	void Draw();
	void Move();

	void Shot();

	Vector3 GetPosition() { return player_.translation_; }

private:

	Vector3 playerPos;
	Vector3 enemyPos;
	Vector3 distance;

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	GamePad* gamePad_ = nullptr;

	WorldTransform player_;

	//仮置きプレイヤーのモデル
	Object3d* playerO_ = nullptr;
	Model* playerM_ = nullptr;

	bool isShot = false;
	int bulletSize = 0;

	float bulletTimer = 60.0f;

	//プレイヤーの弾モデル関連
	std::list <std::unique_ptr<PlayerBullet>> bullets_;
	Model* bulletM_ = nullptr;

	//プレイヤーのFBXモデル
	std::unique_ptr<FbxObject3d> playerFbxO_;
	std::unique_ptr<FbxModel> playerFbxM_;

	Enemy* enemy_ = nullptr;

	int SPHERE_COLISSION_NUM = 1;	//コライダー（スフィア）の数
	std::vector<Matrix4>* collisionBonesMat;	//当たり判定用のボーンのワールド行列
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};
};

