#pragma once
#include "EnemyState.h"
#include "worldTransform.h"
#include "Player.h"
#include "Enemy.h"
class EnemyStandby :
    public EnemyState
{

public:

    void Update(EnemyState* action, Player* player, Enemy* enemy);

    void SetWorldTransform(WorldTransform wtf) { wtf_ = wtf; }
    WorldTransform GetWorldTransform() { return wtf_; }

    void SetMatworld(Matrix4 matworld) { wtf_.matWorld_ = matworld; }
    Matrix4 GetMatworld() { return wtf_.matWorld_; }

    void SetPostion(Vector3& pos) { pos = wtf_.translation_; }
    Vector3 GetPosition() { wtf_.translation_; }

    void SetState(EnemyState* action) { action_ = action; }

private:
    EnemyState* action_ = nullptr;
    WorldTransform wtf_;

    Vector3 playerPos;
    Vector3 enemyPos;

};

