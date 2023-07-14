#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "GamePad.h"

#include "worldTransform.h"
#include "Object3d.h"
#include "Model.h"

#include "FbxObject3d.h"
#include "FbxModel.h"

#include "PlayerBullet.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"

class Enemy;

enum BulletType
{
	None,
	OneShot,
	RapidShot,
};



class Player
{
public:

	WorldTransform wtf;

public:

	void Initialize(DirectXCommon* dxCommon, Input* input, GamePad* gamePad, Enemy* enemy);

	void Update();

	void Draw();
	void Move();

	void Shot();

	Vector3 GetPosition() { return wtf.translation_; }

	void CheckHitCollision();

private:
	Vector3 oldPos;
	Vector3 playerPos;
	Vector3 enemyPos;
	Vector3 distance;

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	GamePad* gamePad_ = nullptr;

	

	//���u���v���C���[�̃��f��
	Object3d* playerO_ = nullptr;
	Model* playerM_ = nullptr;

	bool isShot = false;
	//�P��
	bool oneShot = false;
	//�A��
	bool rapidShot = false;
	//�e�̍ő��
	int MAX_BULLET = 0;
	//���݂̒e�̌�
	int bulletSize = 0;
	//�e�̃^�C�v
	int bulletType = BulletType::None;
	//�e�ƒe�̊Ԋu����
	float bulletTimer = 0.0f;
	//�A�ː����̂��߂̃N�[���^�C��
	float coolTimer = 60.0f;
	//�{�^���������Ă鎞��
	float pushTimer = 15.0f;
	//���������Ă��鎞��
	float pressTimer = 0.0f;

	//�x�W�G�ɂ��悤�Ƃ������
	long long startCount = 0;
	long long nowCount = 0;
	long long elapsedCount = 0;
	float maxTime = 5.0f;				//�S�̎���[s]
	float timeRate;						//�������Ԃ��i�񂾂�



	//�v���C���[�̒e���f���֘A
	std::list <std::unique_ptr<PlayerBullet>> bullets_;
	Model* bulletM_ = nullptr;

	//�v���C���[��FBX���f��
	std::unique_ptr<FbxObject3d> playerFbxO_;
	std::unique_ptr<FbxModel> playerFbxM_;

	Enemy* enemy_ = nullptr;

	int SPHERE_COLISSION_NUM = 1;	//�R���C�_�[�i�X�t�B�A�j�̐�
	std::vector<Matrix4>* collisionBonesMat;	//�����蔻��p�̃{�[���̃��[���h�s��
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};
};

