#pragma once
#include "Camera.h"
#include "Input.h"
#include "Object3d.h"

class GameCamera :
    public Camera
{
public:
    //コンストラクタ
    GameCamera(int width, int height, Input* input);
    //デストラクタ
    ~GameCamera();

    //初期化
    void Initialize();
    //更新
    void Update();

    //自機と敵を追従する
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

