#pragma once

#include "DirectXCommon.h"

#include "worldTransform.h"

#include "Object3d.h"
#include "Model.h"

#include "FbxObject3d.h"
#include "FbxModel.h"

class Enemy
{

public:

	void Initialize(DirectXCommon* dxCommon);

	void Update();

	void Draw();

	Vector3 GetPosition() { return enemy_.translation_; }

private:

	DirectXCommon* dxCommon_ = nullptr;

	WorldTransform enemy_;

	//仮置きプレイヤーのモデル
	Object3d* enemyO_ = nullptr;
	Model* enemyM_ = nullptr;

	//敵のFBXモデル
	std::unique_ptr<FbxObject3d> enemyFbxO_;
	std::unique_ptr<FbxModel> enemyFbxM_;


};

