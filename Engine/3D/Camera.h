﻿#pragma once

#include <DirectXMath.h>
#include <Vector3.h>
#include<Vector2.h>
#include<Matrix4.h>
#include "MyMath.h"

/// <summary>
/// カメラ基本機能
/// </summary>
class Camera {
  protected: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

  public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	Camera(int window_width, int window_height);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Camera() = default;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	void UpdateViewMatrix();

	/// <summary>
	/// 射影行列を更新
	/// </summary>
	void UpdateProjectionMatrix();

	/// <summary>
	/// ビュー行列の取得
	/// </summary>
	/// <returns>ビュー行列</returns>
	inline const Matrix4& GetViewMatrix() { return matView; }

	/// <summary>
	/// 射影行列の取得
	/// </summary>
	/// <returns>射影行列</returns>
	inline const Matrix4& GetProjectionMatrix() { return matProjection; }

	/// <summary>
	/// ビュー射影行列の取得
	/// </summary>
	/// <returns>ビュー射影行列</returns>
	inline const Matrix4& GetViewProjectionMatrix() { return matViewProjection; }

	/// <summary>
	/// ビルボード行列の取得
	/// </summary>
	/// <returns>ビルボード行列</returns>
	inline const Matrix4& GetBillboardMatrix() { return matBillboard; }

	/// <summary>
	/// 視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	inline const Vector3& GetEye() { return eye; }

	/// <summary>
	/// 視点座標の設定
	/// </summary>
	/// <param name="eye">座標</param>
	inline void SetEye(Vector3 eye) {
		this->eye = eye;
		viewDirty = true;
	}

	/// <summary>
	/// 注視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	inline const Vector3& GetTarget() { return target; }

	/// <summary>
	/// 注視点座標の設定
	/// </summary>
	/// <param name="target">座標</param>
	inline void SetTarget(Vector3 target) {
		this->target = target;
		viewDirty = true;
	}

	/// <summary>
	/// 上方向ベクトルの取得
	/// </summary>
	/// <returns>上方向ベクトル</returns>
	inline const Vector3& GetUp() { return up; }

	/// <summary>
	/// 上方向ベクトルの設定
	/// </summary>
	/// <param name="up">上方向ベクトル</param>
	inline void SetUp(Vector3 up) {
		this->up = up;
		viewDirty = true;
	}

	inline const float& GetSensor() {
		return sensor;
	}
	inline void SetSensor(float sensor) {
		this->sensor = sensor; viewDirty = true;
	}
	// FL
	inline const float& GetForcalLengs() {
		return focalLengs;
	}
	inline void SetFocalLengs(float focalLengs) {
		this->focalLengs = focalLengs; viewDirty = true;
	}

	/// <summary>
	/// ベクトルによる視点移動
	/// </summary>
	/// <param name="move">移動量</param>
	void MoveEyeVector(const Vector3& move);

	/// <summary>
	/// ベクトルによる移動
	/// </summary>
	/// <param name="move">移動量</param>
	void MoveVector(const Vector3& move);

	float FieldOfViewY();

	static void MakePerspectiveL(float fovAngleY, float aspect, float near_, float far_, Matrix4& matrix);
	
	static void MakeLookL(const Vector3& eye, const Vector3& target, const Vector3& up, Matrix4& mat);
	static Camera* GetInstance();

  protected: // メンバ変数
	// ビュー行列
	Matrix4 matView = MyMath::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// ビルボード行列
	static Matrix4 matBillboard;
	// Y軸回りビルボード行列
	static Matrix4 matBillboardY;
	// 射影行列
	Matrix4 matProjection = MyMath::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// ビュー射影行列
	Matrix4 matViewProjection = MyMath::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// ビュー行列ダーティフラグ
	bool viewDirty = false;
	// 射影行列ダーティフラグ
	bool projectionDirty = false;
	
	// 上方向ベクトル
	Vector3 up = {0, 1, 0};
	// アスペクト比
	float aspectRatio = 1.0f;

public:
	// 視点座標
	Vector3 eye = { 0, 0, -1 };
	// 注視点座標
	Vector3 target = { 0, 0, 0 };

	float focalLengs = 50;
	float sensor = 35;
};
