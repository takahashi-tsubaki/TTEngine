#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include "MyMath.h"

/// <summary>
/// スフィアコライダー
/// </summary>
class SphereCollider : public BaseCollider,public Sphere
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="offset"></param>
	/// <param name="radius"></param>
	SphereCollider(Vector3 offset = { 0 , 0 , 0 },float radius = 1.0f) :
		offset(offset),
		radius_(radius)
	{
		//球形状をセット
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	//更新
	void Update() override;
	/// <summary>
	/// 半径のセット
	/// </summary>
	/// <param name="rad"></param>
	inline void SetRadius(float rad)
	{
		radius_ = rad;
	}
	/// <summary>
	/// 半径のゲット
	/// </summary>
	/// <returns></returns>
	float GetRadius()
	{
		return radius_;
	}
	/// <summary>
	/// 座標のセット
	/// </summary>
	/// <param name="pos"></param>
	inline void SetBasisPos(Vector3* pos)
	{
		basisPos = pos;
	}
	/// <summary>
	/// 座標のゲット
	/// </summary>
	/// <returns></returns>
	Vector3 GetBasisPos()
	{
		return *basisPos;
	}

private:
	//オブジェクト中心からのオフセット
	Vector3 offset;

	//モデルがない場合の基準となるベクトル
	Vector3* basisPos = nullptr;

	//半径
	float radius_;

};

