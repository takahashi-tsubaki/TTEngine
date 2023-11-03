#pragma once
#include "Camera.h"
#include "Input.h"
#include "Object3d.h"

/// <summary>
/// ゲームカメラ
/// </summary>
class GameCamera :
	public Camera
{
public:
	//コンストラクタ
	GameCamera(int width,int height,Input* input);
	//デストラクタ
	~GameCamera();

	//初期化
	void Initialize();
	//更新
	void Update();
	/// <summary>
	/// 注視点座標のセット
	/// </summary>
	/// <param name="targetPos"></param>
	void SetTargetPos(WorldTransform* targetPos) {
		targetPos_ = targetPos;
	}
	/// <summary>
	/// 注視点座標のゲット
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetTargetPos() {
		return targetPos_;
	}
	/// <summary>
	/// 注視点座標の削除
	/// </summary>
	void RemoveTargetPos() {
		targetPos_ = nullptr;
	}
	/// <summary>
	/// 視点座標のゲット
	/// </summary>
	/// <param name="eyePos"></param>
	void SetEyePos(WorldTransform* eyePos) {
		eyePos_ = eyePos;
	}
	/// <summary>
	/// 追従する対象の座標をゲット
	/// </summary>
	/// <param name="wtf"></param>
	void SetFollowerPos(WorldTransform* wtf) {
		followerPos_ = wtf;
	}
	/// <summary>
	/// 視点座標のゲット
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetEyePos() {
		return eyePos_;
	}

	Vector3 GetEyeVec() { return eyeVec; }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="eVec"></param>
	void SetEyeVec(Vector3& eVec) { eyeVec = eVec; }

	/// <summary>
	/// 視点座標の削除
	/// </summary>
	void RemoveEyePos() {
		eyePos_ = nullptr;
	}
	//void ViewPointMovement();

	/// <summary>
	/// カメラの移動処理
	/// </summary>
	void MoveCamera();

	//自機と敵を追従する
	void IsFollow(bool isFollow) {
		isFollow_ = isFollow;
	}

	float GetCameraDis() { return cameraDistance_; }

	float GetCameraHeight() { return cameraHeight_; }

private:

	Input* input_ = nullptr;

	WorldTransform* targetPos_ = nullptr;
	WorldTransform* eyePos_ = nullptr;
	WorldTransform* followerPos_ = nullptr;

	float cameraHeight_ = 6;
	//カメラ距離関係
	const float  MAX_CAMERA_DISTANCE = 50.0f;
	const float  MIN_CAMERA_DISTANCE = 7.0f;
	float cameraDistance_ = 20.0f;
	const float MAX_CHANGE_TIMER = 30;
	int cameraModeChangeCountTimer = 30;

	Vector3 eyeVec = {};

	Vector3 loolAtPos = {};

	bool isFollow_ = false;

	bool isstanby = false;

};

