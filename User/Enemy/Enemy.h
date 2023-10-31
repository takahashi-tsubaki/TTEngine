#pragma once

#include "DirectXCommon.h"

#include "worldTransform.h"

#include "Sprite.h"

#include "Object3d.h"
#include "Model.h"

#include "FbxObject3d.h"
#include "FbxModel.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"

#include "EnemyBullet.h"
#include "ParticleManager.h"

class Player;

enum EnemyBulletType
{
	NONE,
	ONESHOT,
	RAPIDSHOT,
};
/// <summary>
/// 敵
/// </summary>
class Enemy
{

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxCommon"></param>
	/// <param name="player"></param>
	void Initialize(DirectXCommon* dxCommon,Player* player);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 当たり判定のチェック
	/// </summary>
	void CheckHitCollision();

	void isDeadAnime();

	/// <summary>
	/// worldTransformのゲット
	/// </summary>
	/// <returns></returns>
	WorldTransform GetWorldTransform() {
		return wtf;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		wtf.translation_ = position;
	}

	/// <summary>
	/// 座標のゲット
	/// </summary>
	/// <returns></returns>
	Vector3 GetPosition() {
		return wtf.translation_;
	}

	/// <summary>
	/// 当たったかどうかのフラグのセット
	/// </summary>
	/// <param name="isHit"></param>
	void SetIsHit(bool isHit) {
		isHit_ = isHit;
	}
	/// <summary>
	/// 当たったかどうかのフラグのゲット
	/// </summary>
	/// <returns></returns>
	bool GetIsHit() {
		return isHit_;
	}
	/// <summary>
	/// 体力のセット
	/// </summary>
	/// <param name="Hp"></param>
	void SetHp(int Hp) {
		Hp_ = Hp;
	}
	/// <summary>
	/// 体力のゲット
	/// </summary>
	/// <returns></returns>
	int GetHp() {
		return Hp_;
	}
	/// <summary>
	/// 死んだかどうかのフラグのセット
	/// </summary>
	/// <param name="isDead"></param>
	void SetisDead(bool isDead) {
		isDead_ = isDead;
	}
	/// <summary>
	/// 死んだかどうかのフラグのゲット
	/// </summary>
	bool GetisDead() {
		return isDead_;
	}
	/// <summary>
	/// object3dのゲット
	/// </summary>
	/// <returns></returns>
	Object3d* GetObject3d() {
		return enemyO_;
	}
	/// <summary>
	/// 射撃したかどうかのフラグのゲット
	/// </summary>
	/// <returns></returns>
	bool GetisShot() {
		return isShot;
	}
	/// <summary>
	/// 単発射撃をしたかのフラグのゲット
	/// </summary>
	/// <returns></returns>
	bool GetisOneShot() {
		return oneShot;
	}
	/// <summary>
	/// 連続射撃をしたかどうかのフラグ
	/// </summary>
	bool GetisRapidShot() {
		return rapidShot;
	}
	/// <summary>
	/// パーティクルのゲット
	/// </summary>
	/// <returns></returns>
	ParticleManager* GetParticle() {
		return particle_.get();
	}
#pragma region 

	//敵の攻撃関数
	void Attack();

	//敵の回避関数
	void Vanish();
	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	void Step();

	void Damage();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();
	/// <summary>
	/// 当たり判定属性のリセット
	/// </summary>
	void ResetAttribute();

	/// <summary>
	/// デバックモードのフラグのセット
	/// </summary>
	/// <param name="mode"></param>
	void SetDebugMode(bool mode) {
		isDebugMode = mode;
	}
	/// <summary>
	/// デバックモードのフラグのゲット
	/// </summary>
	/// <returns></returns>
	bool GetDebugMode() {
		return isDebugMode;
	}

	int GetDamageSize() {
		return damageSize;
	}
	/// <summary>
	/// カメラアングルの移動処理
	/// </summary>
	void moveAngle();

#pragma endregion


private:

#pragma region

	WorldTransform wtf;
	int Hp_ = 30;
	int damage = 1;
	int damageSize = 0;

	int decreaseHpCount = 60;

	Vector3 oldPos;
	Vector3 playerPos_;
	Vector3 enemyPos_;
	Vector3 distance_;
	Vector3 cameraAngle;

	Vector3 velocity_;
	float moveSpeed = 0.5f;

	float angle;

	bool isDead_ = false;
	bool isDamage = false;

	/// <summary>
	/// 移動関連系
	/// </summary>
	//各行動のフラグ
	bool isAttack = false;
	bool isVanish = false;
	bool isMove = false;

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

	Vector3 bulletPlayerPos;
	Vector3 bulletEnemyPos;
	Vector3 bulletDistance;
	/// 
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

	float ShotflameCount = 0;


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

	bool isDebugMode = false;

	std::unique_ptr<ParticleManager> particle_;

#pragma region 演出関連

	Vector3 blowAwayPos;

	Vector3 blowAwayRotate;

	int blowAwayCount = 0;

	float addblowSpeed = 2.0f;
	float gravity = 0.3f;

#pragma endregion
};

