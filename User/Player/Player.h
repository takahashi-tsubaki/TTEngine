#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "GamePad.h"

#include "Sprite.h"

#include "worldTransform.h"
#include "Object3d.h"
#include "Model.h"

#include "FbxObject3d.h"
#include "FbxModel.h"

#include "PlayerBullet.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"

#include "ParticleManager.h"

class Enemy;

enum PlayerBulletType
{
	None,
	OneShot,
	RapidShot,
};


/// <summary>
/// 自機
/// </summary>
class Player
{

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxCommon"></param>
	/// <param name="enemy"></param>
	void Initialize(DirectXCommon* dxCommon,Enemy* enemy);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input"></param>
	/// <param name="gamePad"></param>
	void Update(Input* input,GamePad* gamePad);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="input"></param>
	/// <param name="gamePad"></param>
	void Move(Input* input,GamePad* gamePad);
	/// <summary>
	/// 射撃
	/// </summary>
	/// <param name="input"></param>
	/// <param name="gamePad"></param>
	void Shot(Input* input,GamePad* gamePad);
	/// <summary>
	/// 回避
	/// </summary>
	/// <param name="input"></param>
	/// <param name="gamePad"></param>
	void Vanish(Input* input,GamePad* gamePad);

	/// <summary>
	/// ダメージ
	/// </summary>
	void Damage();

	/// <summary>
	/// タイトルシーンのアニメーション
	/// </summary>
	void TitleAnime();
	/// <summary>
	/// 座標のゲット
	/// </summary>
	/// <returns></returns>
	Vector3 GetPosition() {
		return wtf.translation_;
	}
	/// <summary>
	/// 当たり判定のチェック
	/// </summary>
	void CheckHitCollision();

	//Hpのsetterとgetter
	void SetHp(int Hp) {
		Hp_ = Hp;
	}
	int GetHp() {
		return Hp_;
	}
//生きているかのsetterとgetter
	void SetisDead(bool isDead) {
		isDead_ = isDead;
	}
	bool GetisDead() {
		return isDead_;
	}
	/// <summary>
	/// object3dのゲット
	/// </summary>
	/// <returns></returns>
	Object3d* GetObject3d() {
		return playerO_;
	}

	/// <summary>
	/// worldTransformのゲット
	/// </summary>
	/// <returns></returns>
	WorldTransform GetWorldTransform() {
		return wtf;
	}

	/// <summary>
	/// カメラのから見た回転座標の移動
	/// </summary>
	void moveAngle();

	/// <summary>
	/// カメラアングルのゲット
	/// </summary>
	/// <returns></returns>
	Vector3 GetAngle() {
		return cameraAngle;
	}

	/// <summary>
	/// パーティクルのゲット
	/// </summary>
	/// <returns></returns>
	ParticleManager* GetParticle() {
		return particle_.get();
	}
	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 当たり判定属性のリセット
	/// </summary>
	void ResetAttribute();
	/// <summary>
	/// 弾のタイプのゲット
	/// </summary>
	/// <returns></returns>
	int GetBulletType() {
		return bulletType;
	}
	/// <summary>
	/// 回避したかどうかのフラグのゲット
	/// </summary>
	/// <returns></returns>
	bool GetVanish() {
		return isVanising;
	}

	/// <summary>
	/// 回避タイマーのゲット
	/// </summary>
	/// <returns></returns>
	float GetVanishTimer() {
		return vanishTimer;
	}

private:

	WorldTransform wtf;

	int damage = 1;
	int damageSize = 0;
	int decreaseHpCount = 60;

	Vector3 oldPos;
	Vector3 playerPos_;
	Vector3 enemyPos_;
	Vector3 distance_;

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	GamePad* gamePad_ = nullptr;

	int Hp_ = 10;
	bool isDead_ = false;
	bool isDamage = false;
	//自機の向き
	Vector3 faceAngle_ = { 0 , 0 , 0 };
	//カメラの角度
	Vector3 cameraAngle;
	Vector3 velocity_ = { 0,0,0 };

	const float moveSpeed = 0.5;
	//旋回速度
	const float kTurnSpeed_ = MyMath::Dig2Rad(10);



#pragma region 射撃関連
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
	int bulletType = PlayerBulletType::None;
	//弾と弾の間隔時間
	float bulletTimer = 0.0f;
	//連射制限のためのクールタイム
	float coolTimer = 60.0f;
	//ボタンを押してる時間
	float pushTimer = 15.0f;
	//長押ししている時間
	float pressTimer = 0.0f;

	//ベジエにしようとしたやつ
	long long startCount = 0;
	long long nowCount = 0;
	long long elapsedCount = 0;
	float maxTime = 5.0f;				//全体時間[s]
	float timeRate;						//何％時間が進んだか
#pragma endregion

#pragma region 回避関連

	float VanishGauge = 3.0f;
	bool isVanising = false;
	Vector3 VanishPos;

	//回避できる時間
	float vanishTimer = 0.0f;

	bool isTimerSet = false;

#pragma endregion

	bool animevanish = false;
	float reduction = 0.05f;
	float expansion = 0.05f;

	float vstanCount = 30.0f;

	Vector3 scale_ = { 1,1,1 };

	int randPosX =  0;
	int randPosZ =  0;
	Vector3 animepos = { 0,0,0 };

	//仮置きプレイヤーのモデル
	Object3d* playerO_ = nullptr;
	Model* playerM_ = nullptr;

	//プレイヤーの弾モデル関連
	std::list <std::unique_ptr<PlayerBullet>> bullets_;
	Model* bulletM_ = nullptr;

	//プレイヤーのFBXモデル
	std::unique_ptr<FbxObject3d> playerFbxO_;
	std::unique_ptr<FbxModel> playerFbxM_;

	Enemy* enemy_ = nullptr;

	int SPHERE_COLISSION_NUM = 1;	//コライダー（スフィア）の数
	std::vector<Matrix4>* collisionBonesMat;	//当たり判定用のボーンのワールド行列
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};

	int hitDeley = 0;	//何フレーム連続で当たるか

	std::unique_ptr<ParticleManager> particle_;
};

