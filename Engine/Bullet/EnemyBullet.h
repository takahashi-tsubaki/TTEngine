#pragma once

#include "Object3d.h"
#include "Model.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"

class Player;

class EnemyBullet
{
public:

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw();

	void Shot();

	void CheckCollision();

	void SetisDead(bool isDead) { isDead_ = isDead; }

	bool GetIsDead() { return isDead_; }

	void SetPlayer(Player* player) { player_ = player; }


private:


	float livingTimer = 540.0f;

	Vector3 velocity_;

	Object3d* bulletO_ = nullptr;

	bool isDead_;

	Player* player_ = nullptr;

	int SPHERE_COLISSION_NUM = 1;	//�R���C�_�[�i�X�t�B�A�j�̐�
	std::vector<Matrix4>* collisionBonesMat;	//�����蔻��p�̃{�[���̃��[���h�s��
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};

	int hitDeley = 0;	//���t���[���A���œ����邩
};

