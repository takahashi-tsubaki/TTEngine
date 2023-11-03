#pragma once
#include "Defineder.h"
#include "Pading.h"



ALICE_SUPPRESS_WARNINGS_BEGIN

#pragma warning(push)
#pragma warning(disable:4514)

#include <DirectXMath.h>
#pragma warning(pop)
ALICE_SUPPRESS_WARNINGS_END
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

/// <summary>
/// math.h
/// </summary>
namespace MyMath
{

	const float PI = 3.141592654f;
	// 度数法->ラジアン変換
	float Dig2Rad(float value);
	// ラジアン->度数法変換 
	float Rad2Dig(float value);

	// ノルム(長さ)を求める
	float Vector2Length(const Vector2& v);

	// 零ベクトルを返す
	const Vector3 Vector3Zero();
	// 2ベクトルが一致しているか調べる
	bool Vector3Equal(const Vector3& v1,const Vector3& v2);
	// ノルム(長さ)を求める
	float Vector3Length(const Vector3& v);
	// 正規化する
	Vector3& Vector3Normalize(Vector3& v);
	// 内積を求める
	float Vector3Dot(const Vector3& v1,const Vector3& v2);
	// 外積を求める
	Vector3 Vector3Cross(const Vector3& v1,const Vector3& v2);

	// 単位行列を求める
	Matrix4 Matrix4Identity();
	// 転置行列を求める
	Matrix4 Matrix4Transpose(const Matrix4& m);
	// 逆行列を求める
	Matrix4 Matrix4Inverse(const Matrix4& m,float* det = nullptr);

	// 拡大縮小行列の作成
	Matrix4 Matrix4Scaling(float sx,float sy,float sz);

	// 回転行列の作成
	Matrix4 Matrix4RotationX(float angle);
	Matrix4 Matrix4RotationY(float angle);
	Matrix4 Matrix4RotationZ(float angle);

	// 平行移動行列の作成
	Matrix4 Matrix4Translation(float tx,float ty,float tz);

	// ビュー行列の作成
	Matrix4 Matrix4LookAtLH(const Vector3& eye,const Vector3& target,const Vector3& up);
	// 並行投影行列の作成
	Matrix4 Matrix4Orthographic(
		float viewLeft,float viewRight,float viewBottom,float viewTop,float nearZ,float farZ);
	// 透視投影行列の作成
	Matrix4 Matrix4Perspective(float fovAngleY,float aspectRatio,float nearZ,float farZ);

	// 座標変換（w除算なし）
	Vector3 Vector3Transform(const Vector3& v,const Matrix4& m);
	// 座標変換（w除算あり）
	Vector3 Vector3TransformCoord(const Vector3& v,const Matrix4& m);
	// ベクトル変換
	Vector3 Vector3TransformNormal(const Vector3& v,const Matrix4& m);

	// 2項演算子オーバーロード
	Matrix4 operator*(const Matrix4& m1,const Matrix4& m2);
	Vector3 operator*(const Vector3& v,const Matrix4& m);

	// 線形補間
	float Lerp(float a,float b,float t);

	Matrix4 Initialize();
	Matrix4 Scale(Vector3 scale);
	Matrix4 Rotate(Vector3 rotation,int X_1_Y_2_Z_3_XYZ_6);
	Matrix4 Move(Vector3 move);
	//void  affin(WorldTransform& affin);

	/// <summary>
	/// vevtor3とmatrix4の計算
	/// </summary>
	/// <param name="vector3"></param>
	/// <param name="matrix4"></param>
	/// <returns></returns>
	Vector3 MatVector(const Vector3 vector3,const Matrix4 matrix4);

	/// <summary>
	/// worldTransformのゲット
	/// </summary>
	/// <param name="matrix4"></param>
	/// <returns></returns>
	Vector3 GetWorldtransform(const Matrix4 matrix4);

	/// <summary>
	/// ベクトル同士の計算
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	Vector3 AddVector(const Vector3 v1,const Vector3 v2);
	/// <summary>
	/// vevtor3とmatrix4の計算
	/// </summary>
	/// <param name="velocity"></param>
	/// <param name="mat"></param>
	/// <returns></returns>
    Vector3 bVelocity(const Vector3& velocity, const Matrix4& mat);

	//座標変換
	Vector3 wDivision(const Vector3& v,const Matrix4& m);

	/// <summary>
	/// 地面とY軸回転の計算
	/// </summary>
	/// <param name="focalLengs"></param>
	/// <param name="sensor"></param>
	/// <returns></returns>
	float FieldOfViewY(float focalLengs,float sensor);

	/// <summary>
	/// XMMATRIXとMatrix4の計算
	/// </summary>
	/// <param name="XMMatrix"></param>
	/// <returns></returns>
	Matrix4 ConvertXMMATtoMat4(DirectX::XMMATRIX XMMatrix);
	DirectX::XMMATRIX ConvertMat4toXMMat(Matrix4 m);

	/// <summary>
	/// 逆行列の生成
	/// </summary>
	/// <param name="mat"></param>
	/// <returns></returns>
	Matrix4 MakeInverse(const Matrix4* mat);


} // namespace MathUtility