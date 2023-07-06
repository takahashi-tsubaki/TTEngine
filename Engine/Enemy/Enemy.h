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

	void Initialize(DirectXCommon* dxCommon);

	void Update();

	void Draw();

	void Action();

	Vector3 GetPosition() { return enemy_.translation_; }

	void SetIsDead(bool isDead) { isDead_ = isDead; }

	bool GetIsDead() { return isDead_; }

private:

	bool isDead_ = false;


	DirectXCommon* dxCommon_ = nullptr;

	WorldTransform enemy_;

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


};

