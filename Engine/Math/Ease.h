#pragma once
#include "Defineder.h"
#include "Pading.h"

ALICE_SUPPRESS_WARNINGS_BEGIN
#include <math.h>
ALICE_SUPPRESS_WARNINGS_END
#include "Vector3.h"
#include "MyMath.h"
/// <summary>
/// イージング
/// </summary>
class Ease
{	//イージングクラス定義
public:
	/// <summary>
	/// イージング
	/// </summary>
	/// <param name="change"></param>
	/// <param name="base"></param>
	/// <param name="duration"></param>
	/// <param name="time"></param>
	/// <returns></returns>
	static double InSine(double change,double base,double duration,double time);
	static double OutSine(double change,double base,double duration,double time);
	static double InOutSine(double change,double base,double duration,double time);

	static double InCubic(double change,double base,double duration,double time);
	static double OutCubic(double change,double base,double duration,double time);
	static double InOutCubic(double change,double base,double duration,double time);

	static double InQuint(double change,double base,double duration,double time);
	static double OutQuint(double change,double base,double duration,double time);
	static double InOutQuint(double change,double base,double duration,double time);

	static double InCric(double change,double base,double duration,double time);
	static double OutCric(double change,double base,double duration,double time);
	static double InOutCric(double change,double base,double duration,double time);

	static double InElastic(double change,double base,double duration,double time);
	static double OutElastic(double change,double base,double duration,double time);
	static double InOutElastic(double change,double base,double duration,double time);

	static double InQuad(double change,double base,double duration,double time);
	static double OutQuad(double change,double base,double duration,double time);
	static double InOutQuad(double change,double base,double duration,double time);

	static double InQuart(double change,double base,double duration,double time);
	static double OutQuart(double change,double base,double duration,double time);
	static double InOutQuart(double change,double base,double duration,double time);

	static double InExpo(double change,double base,double duration,double time);
	static double OutExpo(double change,double base,double duration,double time);
	static double InOutExpo(double change,double base,double duration,double time);

	static double InBack(double change,double base,double duration,double time);
	static double OutBack(double change,double base,double duration,double time);
	static double InOutBack(double change,double base,double duration,double time);

	static double InBounce(double change,double base,double duration,double time);
	static double OutBounce(double change,double base,double duration,double time);
	static double InOutBounce(double change, double base,double duration,double time);

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
