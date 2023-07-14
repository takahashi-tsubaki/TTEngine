#pragma once

#include "DirectXCommon.h"

#include "worldTransform.h"

#include "Object3d.h"
#include "Model.h"

#include "FbxObject3d.h"
#include "FbxModel.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"

class Player;

class Enemy
{
public:
	WorldTransform wtf;

public:

	void Initialize(DirectXCommon* dxCommon, Player* player);

	void Update();

	void Draw();

	void Action();

	Vector3 GetPosition() { return wtf.translation_; }

	void SetIsHit(bool isHit) { isHit_ = isHit; }

	bool GetIsHit() { return isHit_; }

	void CheckHitCollision();

#pragma region �G��State�p�^�[���s���̉��̂���

	//�G�̍U���֐�
	void Attack();

	//�G�̉���֐�
	void Vanish();

	void Move();

	void Step();

#pragma endregion


private:

#pragma region �G��State�p�^�[���s���̉��̂���

	Vector3 oldPos;
	Vector3 playerPos;
	Vector3 enemyPos;
	Vector3 distance;

	//�e�s���̃t���O
	bool isAttack = false;
	bool isVanish = false;
	bool isMove   = false;

	bool isLeft = false;
	bool isRight = false;
	bool isApproach = false;

	//�s���p�^�[���̊i�[�ϐ�
	int actionNum = 0;
	//�ړ��p�^�[���̊i�[�ϐ�
	int moveActionNum = 0;

	int flameCount = 0;



#pragma endregion

	bool isHit_ = false;
	DirectXCommon* dxCommon_ = nullptr;
	//���u���v���C���[�̃��f��
	Object3d* enemyO_ = nullptr;
	Model* enemyM_ = nullptr;

	//�G��FBX���f��
	std::unique_ptr<FbxObject3d> enemyFbxO_;
	std::unique_ptr<FbxModel> enemyFbxM_;

	int SPHERE_COLISSION_NUM = 1;	//�R���C�_�[�i�X�t�B�A�j�̐�
	std::vector<Matrix4>* collisionBonesMat;	//�����蔻��p�̃{�[���̃��[���h�s��
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};

	int hitDeley = 0;	//���t���[���A���œ����邩

	Player* player_ = nullptr;
};

