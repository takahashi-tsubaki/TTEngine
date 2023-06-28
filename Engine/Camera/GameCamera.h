#pragma once
#include "Camera.h"
#include "Input.h"
#include "Object3d.h"

class GameCamera :
    public Camera
{
public:
    //�R���X�g���N�^
    GameCamera(int width, int height, Input* input);
    //�f�X�g���N�^
    ~GameCamera();

    //������
    void Initialize();
    //�X�V
    void Update();

    //���@�ƓG��Ǐ]����
    void FollowPtoE();

    void IsFollow(bool isFollow) { isFollow_ = isFollow; }

private:

    Input* input_ = nullptr;
    WorldTransform* targetPos = nullptr;
    Vector3 playerPos; 
    Vector3 enemyPos;
    Vector3 dir = { 0,0,1 };

    const float Camera_Distance = 10.0f;

    bool isFollow_ = false;

};

