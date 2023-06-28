#include "GameCamera.h"

GameCamera::GameCamera(int width, int height, Input* input) : Camera(width, height)
{
	input_ = input;

	//ƒJƒƒ‰‚Ì‰Šú‰»
	Vector3 eye = eye_;
	Vector3 target = target_;
	Vector3 up = up_;

	this->SetEye(eye);
	this->SetTarget(target);
	this->SetUp(up);



}

GameCamera::~GameCamera()
{

}

void GameCamera::Initialize()
{

}

void GameCamera::Update()
{
	FollowPtoE();
}

void GameCamera::FollowPtoE()
{
	/*dir = */

	Vector3 distanceEye;
	distanceEye = playerPos - enemyPos;

	Vector3 distanceTarget;
	distanceTarget = distanceEye * Camera_Distance;

	SetEye(distanceEye);
	SetTarget(distanceTarget);

}
