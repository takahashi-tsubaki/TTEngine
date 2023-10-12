#pragma once
#include "Defineder.h"
#include "Pading.h"

ALICE_SUPPRESS_WARNINGS_BEGIN
#include <math.h>
ALICE_SUPPRESS_WARNINGS_END
#include "Vector3.h"

/// <summary>
/// イージング
/// </summary>
class Ease
{	//イージングクラス定義
public:
	static double InQuad(double change,double base,double duration,double time);
	static double OutQuad(double change,double base,double duration,double time);
	static double InOutQuad(double change,double base,double duration,double time);

	static double InOutQuint(double change, double base,double duration,double time);

	//↓ここから下は使いません。。↓
	void Initialize();
	void Update();

private:
	int maxTime_ = 50;	//最大時間
	int time_ = 0;//経過時間
	Vector3 startVec_ = {};
	Vector3 endVec_ = {};
	double start_ = 0;
	double end_ = 0;
	bool isChange_ = false;
};
