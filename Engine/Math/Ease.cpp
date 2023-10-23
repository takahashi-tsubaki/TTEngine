#include "Ease.h"
//changeは移動量。baseは最初の位置。durationは移動時間で、timeが現在の経過時間
double Ease::InSine(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return 1 - cos((time * MyMath::PI) / 2) + base;
}

double Ease::OutSine(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return sin((time * MyMath::PI) / 2) + base;
}

double Ease::InOutSine(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return -(cos(time * MyMath::PI) - 1) / 2 + base;
}

double Ease::InCubic(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return time * time * time + base;
}

double Ease::OutCubic(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return change * 1 - pow(1 - time , 3)+ base;
}

double Ease::InOutCubic(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return time < 0.5 ? 4 * pow(time, 3) + base: 1 - pow(-2 * time  + 2, 3) / 2 + base;
}

double Ease::InQuint(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return powf((float)time, 5.0f)+base;
}

double Ease::OutQuint(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return 1 - pow(1- (float)time,5)+base;
}

double Ease::InOutQuint(double change,double base,double duration,double time)
{
	change;
	time /= duration;
	return time < 1 ? 16 * powf((float)time,5.0f) + base : 1 - powf(-2 * (float)time + 2,5) / 2+base;
}

double Ease::InCric(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return 1 - sqrt(1 - pow(time, 2)) + base;
}

double Ease::OutCric(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return  change * sqrt(1 - pow(time - 1, 2)) + base;
}

double Ease::InOutCric(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return time < 0.5 ? (1 - sqrt(1 - pow(2 * time, 2))) / 2 + base :
		(sqrt(1 - pow(-2 * time + 2, 2)) + 1) / 2 + base;
}

double Ease::InElastic(double change, double base, double duration, double time)
{
	const float c4 = ( 2 * MyMath::PI ) / 3;
	
	time /= duration;
	return change * time == 0 ? 0 :
		change * time == 1 ? 1 :
		-pow(2,10 * time - 10) * sin((time*10 - 10.75f)*c4) +base;
}

double Ease::OutElastic(double change, double base, double duration, double time)
{
	const float c4 = ( 2 * MyMath::PI ) / 3;
	change;
	time /= duration;
	return change * time == 0 ? 0 + base :
		change * time == 1 ? 1 + base :
		pow(2,10 * time - 10) * sin(( time * 10 - 0.75f ) * c4) +1+ base;
}

double Ease::InOutElastic(double change, double base, double duration, double time)
{
	const float c5 = ( 2 * MyMath::PI ) / 4.5f;
	change;
	time /= duration;
	return change * time == 0 ? 0 + base :
		change * time == 1 ? 1 + base :
		time < 0.5 ? -pow(2,20 * time - 10) * sin(( time * 20 - 11.125f ) * c5) + base :
		pow(2,20 * time - 10) * sin(( time * 20 - 11.125f ) * c5) + base;
}


double Ease::InQuad(double change, double base, double duration, double time) {	//イージングイン
	time /= duration;
	return change * time * time + base;	//changeは移動量。baseは最初の位置。durationは移動時間で、timeが現在の経過時間
}

double Ease::OutQuad(double change, double base, double duration, double time)
{	//イージングアウト
	time /= duration;
	double ans = change * (1 - (1 - time) * (1 - time) + base);
	return ans;
}

double Ease::InOutQuad(double change, double base, double duration, double time)
{	//イージングインアウト
	time /= duration / 2;
	if (time < 1) return change / 2 * time * time + base;
	return -change / 2 * ((--time) * (time - 2) - 1) + base;
}


double Ease::InQuart(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return pow(time, 4) + base;
}

double Ease::OutQuart(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return 1 - pow(1 - time, 4) + base;
}

double Ease::InOutQuart(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return time < 0.5 ? 8 * pow(time,4)+base :
		1 - pow(-2*time+2,4)/2+base;
}

double Ease::InExpo(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return 0.0 + base;
}

double Ease::OutExpo(double change, double base, double duration, double time)
{
	
	time /= duration;
	return change * time==1 ? 1 + base:
		1 - pow(2, -10 * time) + base;
}

double Ease::InOutExpo(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return 0.0 + base;
}

double Ease::InBack(double change, double base, double duration, double time)
{

	const float c1 = 1.70158f;
	const float c3 = c1 + 1;
	change;
	time /= duration;
	return c3 * pow(time,3) - c1 * pow(time,2) + base;
}

double Ease::OutBack(double change, double base, double duration, double time)
{

	const float c1 = 1.70158f;
	const float c3 = c1 + 1;
	change;
	time /= duration;
	return 1 + c3 * pow(time - 1,3) + c1 * pow(time - 1,2) + base;
}

double Ease::InOutBack(double change, double base, double duration, double time)
{
	const float c1 = 1.70158f;
	const float c3 = c1 * 1.525f;
	time /= duration;
	return change *  time < 0.5 ? ( pow(2 * time,2) * ( ( c3 + 1 ) * 2 * time - c3 ) ) / 2 :
		change * ( pow(2 * time - 2,2) * ( ( c3 + 1 ) * ( c3 * 2 - 2 ) + c3 ) + 2 ) / 2 + base;
}

double Ease::InBounce(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return 1 - OutBounce(change, base, duration,1 - time) + base;
}

double Ease::OutBounce(double change, double base, double duration, double time)
{

	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	change;
	time /= duration;


	if (time < 1 / d1)
	{
		return n1 * time * time + base;
	}
	else if (time < 2 / d1)
	{
		return n1 * (time -= 1.5 / d1) * (time + 0.75f) + base;
	}
	else if (time < 2.5 / d1)
	{
		return n1* (time -= 2.25/ d1)* (time + 0.9375f) + base;
	}
	else
	{
		return n1* (time -= 2.625 / d1)* (time + 0.984375f) + base;
	}


}

double Ease::InOutBounce(double change, double base, double duration, double time)
{
	change;
	time /= duration;
	return time < 0.5 ? (1 - OutBounce(change, base, duration, 1 - 2 * time)) / 2 + base :
		(1 + OutBounce(change, base, duration, 2 * time - 1)) / 2 + base;
}

void Ease::Initialize()
{
}

void Ease::Update()
{
}


