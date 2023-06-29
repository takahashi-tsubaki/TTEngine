#pragma once
#include "DirectXCommon.h"
#include "Input.h"

#include "worldTransform.h"

#include "FbxObject3d.h"
#include "FbxModel.h"
class Player
{
public:

	void Initialize(DirectXCommon* dxCommon, Input* input);

	void Update();

	void Draw();
	void Move();

private:

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;

	WorldTransform enemy_;

	std::unique_ptr<FbxObject3d> playerFbxO_;
	std::unique_ptr<FbxModel> playerFbxM_;

};

