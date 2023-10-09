#include "SphereCollider.h"

void SphereCollider::Update()
{
	if (object3d)
	{
		//ワールド行列から座標を抽出
		const Matrix4& matWorld = object3d->GetMatWorld();

		//球のメンバ変数を更新
		Sphere::center =
		{
			matWorld.m[3][0] + offset.x ,
			matWorld.m[3][1] + offset.y ,
			matWorld.m[3][2] + offset.z
		};
	}
	else if (basisPos)
	{
		Sphere::center = *basisPos + offset;
	}
	else
	{
		Sphere::center = {0 , 0 , 0};
	}


	Sphere::radius = radius_;

	isHit = false;
}