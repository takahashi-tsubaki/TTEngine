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

#include "EnemyBullet.h"

class Player;

enum EnemyBulletType
{
	NONE,
	ONESHOT,
	RAPIDSHOT,
};

class Enemy
{
public:
	WorldTransform wtf;
	int Hp_ = 30;
public:

	void Initialize(DirectXCommon* dxCommon, Player* player);

	void Update();

	void Draw();

	void Action();

	void CheckHitCollision();

	Vector3 GetPosition() { return wtf.translation_; }


	void SetIsHit(bool isHit) { isHit_ = isHit; }
	bool GetIsHit() { return isHit_; }

	void SetHp(int Hp) { Hp_ = Hp; }
	int GetHp() { return Hp_; }

	void SetisDead(bool isAlive) { isAlive_ = isAlive; }
	bool GetisDead() { return isAlive_; }

	Object3d* GetObject3d() {return enemyO_;}

	bool GetisShot() { return isShot; }
	bool GetisRapidShot() { return rapidShot;}
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

	float angle;
	
	bool isAlive_ = false;
	
	/// <summary>
	/// �ړ��֘A�n
	/// </summary>
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

	int MoveflameCount = 0;

	/// <summary>
	/// �U���֘A�n
	/// </summary>
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
	int bulletType = EnemyBulletType::NONE;
	//�e�ƒe�̊Ԋu����
	float bulletTimer = 0.0f;
	//�A�ː����̂��߂̃N�[���^�C��
	float coolTimer = 60.0f;
	//�{�^���������Ă鎞��
	float pushTimer = 15.0f;
	//���������Ă��鎞��
	float pressTimer = 0.0f;

	int ShotflameCount = 0;


	int rapidCount = 0;

	//�������t���O
	bool isCharge = false;

#pragma endregion

	bool isHit_ = false;
	DirectXCommon* dxCommon_ = nullptr;
	//���u���G�̃��f��
	Object3d* enemyO_ = nullptr;
	Model* enemyM_ = nullptr;

	//�G�̒e���f���֘A
	std::list <std::unique_ptr<EnemyBullet>> bullets_;
	Model* bulletM_ = nullptr;

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

