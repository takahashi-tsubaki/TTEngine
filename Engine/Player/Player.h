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

enum PlayerBulletType
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

	void Initialize(DirectXCommon* dxCommon, Enemy* enemy);

	void Update(Input* input, GamePad* gamePad);

	void Draw();
	void Move(Input* input, GamePad* gamePad);

	void Shot(Input* input, GamePad* gamePad);

	void Vanish(Input* input, GamePad* gamePad);

	Vector3 GetPosition() { return wtf.translation_; }

	void CheckHitCollision();

	//Hp��setter��getter
	void SetHp(int Hp) { Hp_ = Hp; }
	int GetHp() { return Hp_; }
	//�����Ă��邩��setter��getter
	void SetisDead(bool isDead) { isDead_ = isDead; }
	bool GetisDead() { return isDead_; }

	Object3d* GetObject3d() { return playerO_; }

	void moveAngle();

	Vector3 GetAngle() {return cameraAngle;}

	void Reset();

private:
	Vector3 oldPos;
	Vector3 playerPos;
	Vector3 enemyPos;
	Vector3 distance;

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	GamePad* gamePad_ = nullptr;

	int Hp_ = 10;
	bool isDead_ = false;

	//���@�̌���
	Vector3 faceAngle_ = { 0 , 0 , 0 };
	//�J�����̊p�x
	Vector3 cameraAngle;
	Vector3 velocity_ = { 0,0,0 };

	const float moveSpeed = 0.5;
	//���񑬓x
	const float kTurnSpeed_ = MyMath::Dig2Rad(10);



#pragma region �ˌ��֘A
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
	int bulletType = PlayerBulletType::None;
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
#pragma endregion

#pragma region ����֘A

	float VanishGauge = 3.0f;
	bool isVanising = false;
	Vector3 VanishPos;

#pragma endregion



	//���u���v���C���[�̃��f��
	Object3d* playerO_ = nullptr;
	Model* playerM_ = nullptr;

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

	int hitDeley = 0;	//���t���[���A���œ����邩

};

