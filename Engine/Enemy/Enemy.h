#pragma once

#include "DirectXCommon.h"

#include "worldTransform.h"

#include "FbxObject3d.h"
#include "FbxModel.h"

class Enemy
{

public:

	void Initialize(DirectXCommon* dxCommon);

	void Update();

	void Draw();

private:

	DirectXCommon* dxCommon_ = nullptr;

	WorldTransform enemy_;

	std::unique_ptr<FbxObject3d> enemyFbxO_;
	std::unique_ptr<FbxModel> enemyFbxM_;


};

