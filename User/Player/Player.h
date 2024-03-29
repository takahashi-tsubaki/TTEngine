#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "GamePad.h"
#include "Fps.h"
#include "Audio.h"

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

enum StepDirection
{
	Left = 1,
	Right= 2,
};

enum FbxAnimetion
{
	IDEL = 0,
	LEFTMOVE,
	RIGHTMOVE,
	LEFTSTEP,
	RIGHTSTEP,
	SHOT,
	HIT,
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
	void Initialize(TTEngine::DirectXCommon* dxCommon, Enemy* enemy);

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
	/// ステップ移動
	/// </summary>
	/// <param name="input"></param>
	/// <param name="gamePad"></param>
	void Step(Input* input, GamePad* gamePad);

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
	void Vanish();

	/// <summary>
	/// ダメージ
	/// </summary>
	void Damage();


	void RemoveAttribute();

	/// <summary>
	/// タイトルシーンのアニメーション
	/// </summary>
	void TitleAnime();

	//

	void GameOverAnime();

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

	Vector3 GetOldPos() {
		return oldPos;
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

	ParticleManager* GetBarriarParticle() { return stageBarriarParticle_.get(); }

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

	bool GetIsShot() {
		return isShot;
	}

	//移動限界フラグのゲッター
	bool GetIsMoveLimit()
	{
		return isMoveLimit;
	}

	FbxObject3d* GetFbxObject3d()
	{
		return fbxPlayerO_.get();
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
	Vector3 stepVel_;

	TTEngine::DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	GamePad* gamePad_ = nullptr;

	int Hp_ = 10;
#pragma region フラグ関連
	bool isDead_ = false;//自機が倒れたかどうか
	bool isDamage = false;//自機がダメージを受けているかどうか
	bool isStep = false;//自機がステップ移動をしているかどうか
	bool isStandBy = false;

	bool isBack = false;//自機が後ろ方向に動いているかどうか

	bool isMoveLimit = false;//移動限界に達しているかどうか

#pragma endregion 

	
	float hitCountTime = 60;

	//自機の向き
	Vector3 faceAngle_ = { 0 , 0 , 0 };

	//fbxの大きさ
	float fbxScale_;

	//カメラの角度
	Vector3 cameraAngle;
	Vector3 velocity_ = { 0,0,0 };
	Vector3 fbxVelocity_ = {0, 0, 0};

	Vector3 farthest_ = {0, 0, 0};
	Vector3 Distance_ = {0, 0, 0};

	Vector3 maxVec;

	Vector3 StepVelocity_ = {0, 0, 0};

	const float moveSpeed = 0.5;
	float backSpeed = 0.5;

	Vector3 posDistance = {0, 0, 0};

	const float MAX_POSITION = 100;

	float StepSpeed;
	int stepDirection;

	int stepFlameCount;
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

	const float MAX_VANISHCOUNT = 6.0f;

	float VanishGauge = MAX_VANISHCOUNT;
	bool isVanising = false;
	Vector3 VanishPos;

	Vector3 VanishDis;
	//回避できる時間
	float vanishTimer = 0.0f;

	bool isTimerSet = false;
	float vanishBeforeCount ;


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
	std::unique_ptr<FbxObject3d> fbxPlayerO_;
	std::unique_ptr<FbxModel> fbxPlayerM_;

	Enemy* enemy_ = nullptr;

	size_t SPHERE_COLISSION_NUM;	//コライダー（スフィア）の数
	std::vector<Matrix4>* collisionBonesMat;	//当たり判定用のボーンのワールド行列
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};

	int hitDeley = 0;	//何フレーム連続で当たるか

	std::unique_ptr<ParticleManager> particle_;
	std::unique_ptr<ParticleManager> stageBarriarParticle_; // 移動限界時のパーティクル

#pragma region 演出関連

	Vector3 blowAwayPos;

	Vector3 blowAwayRotate;

	int blowAwayCount = 0;

	float addblowSpeed = 2.0f;
	float gravity = 0.3f;

	Vector3 transNormal;
#pragma endregion

#pragma region 音関連

	// 音関係まとめ

	TTEngine::Audio* audio = nullptr;

	// 音を止める関数
	IXAudio2SourceVoice* pSourceVoice[10] = {0};

	int soundCheckFlag = 0;
#pragma endregion

};

