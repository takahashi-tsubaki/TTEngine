#pragma once

#include "DirectXCommon.h"
#include "GamePad.h"
#include "Input.h"

#include "Vector2.h"
#include "Vector3.h"

#include "FbxObject3d.h"
#include "FbxModel.h"

#include "worldTransform.h"
#include "Object3d.h"
#include "Model.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"

#include "ParticleM.h"
#include "OBJParticle.h"


class SceneObjects;

enum FBXAnimetion {
	IdeL = 0,
	LeftMOVE,
	RightMOVE,
	LeftSTEP,
	RightSTEP,
	ShoT,
	HiT,
	Charge,
};

class Character
{
public:

	Character();
	
	virtual void Draw() = 0;

	// ワールド行列の取得
	WorldTransform GetPlayerWorldTransform() { return worldTransform; }

	// Fbxオブジェクトの取得
	FbxObject3d* GetFbxObject3d() { return fbxObject_.get(); }

	Object3d* GetObject3d() { return object_.get();}

	/// <summary>
	/// カメラのから見た回転座標の移動
	/// </summary>
	void moveAngle();

		/// <summary>
	/// カメラアングルのゲット
	/// </summary>
	/// <returns></returns>
	Vector3 GetAngle() { return cameraAngle; }

	void Distance(Vector3 start, Vector3 goal);

	//2点間の距離
	void DistanceTwoPoints(Vector3 start, Vector3 goal);

	void SetisDead(bool isDead) { isDead_ = isDead; }
	bool GetisDead() { return isDead_; }

protected:
	WorldTransform worldTransform;

	TTEngine::DirectXCommon* dxCommon_ = nullptr;
			// カメラの角度
	Vector3 cameraAngle;
	Vector3 distance;
	
	// プレイヤーのFBXモデル
	std::unique_ptr<FbxObject3d> fbxObject_;
	std::unique_ptr<FbxModel> fbxModel_;

	std::unique_ptr<Object3d> object_;
	std::unique_ptr<Model> model_;
	// fbxの大きさ
	float fbxScale_;
	SceneObjects* sceneObject_;

#pragma region フラグ関連

	bool isDead_ = false; // 自機が倒れたかどうか

#pragma endregion
};
