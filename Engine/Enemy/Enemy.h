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
#pragma region 敵のStateパターン行動の仮のもの

	//敵の攻撃関数
	void Attack();

	//敵の回避関数
	void Vanish();

	void Move();

	void Step();




#pragma endregion


private:

#pragma region 敵のStateパターン行動の仮のもの

	Vector3 oldPos;
	Vector3 playerPos;
	Vector3 enemyPos;
	Vector3 distance;

	float angle;
	
	bool isAlive_ = false;
	
	/// <summary>
	/// 移動関連系
	/// </summary>
	//各行動のフラグ
	bool isAttack = false;
	bool isVanish = false;
	bool isMove   = false;

	bool isLeft = false;
	bool isRight = false;
	bool isApproach = false;

	//行動パターンの格納変数
	int actionNum = 0;
	//移動パターンの格納変数
	int moveActionNum = 0;

	int MoveflameCount = 0;

	/// <summary>
	/// 攻撃関連系
	/// </summary>
	bool isShot = false;
	//単発
	bool oneShot = false;
	//連射
	bool rapidShot = false;
	//弾の最大個数
	int MAX_BULLET = 0;
	//現在の弾の個数
	int bulletSize = 0;
	//弾のタイプ
	int bulletType = EnemyBulletType::NONE;
	//弾と弾の間隔時間
	float bulletTimer = 0.0f;
	//連射制限のためのクールタイム
	float coolTimer = 60.0f;
	//ボタンを押してる時間
	float pushTimer = 15.0f;
	//長押ししている時間
	float pressTimer = 0.0f;

	int ShotflameCount = 0;


	int rapidCount = 0;

	//長押しフラグ
	bool isCharge = false;

#pragma endregion

	bool isHit_ = false;
	DirectXCommon* dxCommon_ = nullptr;
	//仮置き敵のモデル
	Object3d* enemyO_ = nullptr;
	Model* enemyM_ = nullptr;

	//敵の弾モデル関連
	std::list <std::unique_ptr<EnemyBullet>> bullets_;
	Model* bulletM_ = nullptr;

	//敵のFBXモデル
	std::unique_ptr<FbxObject3d> enemyFbxO_;
	std::unique_ptr<FbxModel> enemyFbxM_;

	int SPHERE_COLISSION_NUM = 1;	//コライダー（スフィア）の数
	std::vector<Matrix4>* collisionBonesMat;	//当たり判定用のボーンのワールド行列
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};

	int hitDeley = 0;	//何フレーム連続で当たるか

	Player* player_ = nullptr;
};

