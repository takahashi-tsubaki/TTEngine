#pragma once

#include "CollisionTypes.h"
#include "Object3d.h"
#include "CollisionInfo.h"


//コライダー基底クラス
class BaseCollider
{
public:
	friend class CollisionManager;

public:
	BaseCollider() = default;
	//仮想デストラクタ
	virtual ~BaseCollider() = default;

	inline void SetObject3d(Object3d* object)
	{
		this->object3d = object;
	}
	/// <summary>
	/// object3dのゲット
	/// </summary>
	/// <returns></returns>
	inline Object3d* GetObject3d()
	{
		return object3d;
	}

	//更新
	virtual void Update() = 0;
	//形状タイプ取得
	inline CollisionShapeType GetShapeType()
	{
		return shapeType;
	}
	/// <summary>
	/// 当たったかどうか
	/// </summary>
	/// <param name="info"></param>
	inline void OnCollision(const CollisionInfo& info)
	{
		isHit = true;
		info_ = info;
	}
	/// <summary>
	/// 当たり判定属性のセット
	/// </summary>
	/// <param name="attribute"></param>
	inline void SetAttribute(unsigned short attribute)
	{
		attribute_ = attribute;
	}

	/// <summary>
	/// 当たり判定属性のゲット
	/// </summary>
	/// <returns></returns>
	unsigned short GetAttribute() {
		return attribute_;
	}
	/// <summary>
	/// 当たり判定属性の追加
	/// </summary>
	/// <param name="attribute"></param>
	inline void AddAttribute(unsigned short attribute)
	{
		attribute_ |= attribute;
	}

	/// <summary>
	/// 当たり判定属性の削除
	/// </summary>
	/// <param name="attribute"></param>
	inline void RemoveAttribute(unsigned short attribute)
	{
		attribute_ &= !attribute;
	}

	/// <summary>
	/// 当たったフラグのゲット
	/// </summary>
	/// <returns></returns>
	inline bool GetIsHit()
	{
		return isHit;
	}

	/// <summary>
	/// 当たり判定情報のゲット
	/// </summary>
	/// <returns></returns>
	inline CollisionInfo GetCollisionInfo()
	{
		return info_;
	}

protected:
	Object3d* object3d = nullptr;

	bool isHit = false;

	CollisionInfo info_ =
	{
		nullptr ,
		nullptr ,
		{0 , 0 , 0}
	};

	//形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
	//当たり判定属性
	unsigned short attribute_ = 0b1111111111111111;
};