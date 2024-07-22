#pragma once
#pragma once
#include "Object3d.h"
#include "MathUtility.h"


class OBJParticle
{
public:
	/// <summary>
	/// 作成
	/// </summary>
	/// <returns></returns>
	static OBJParticle* Create(const Vector3& pos_,Model* model_,const Vector3& velocity,float scale = 1.0f,Vector4 color = { 1,1,1,1 },bool changeScale = false);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos_"></param>
	/// <param name="model_"></param>
	/// <param name="pipeline_"></param>
	void Init(const Vector3& pos_,Model* model_,const Vector3& velocity,float scale,Vector4 color,bool changeScale);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 死んだかゲッター
	/// </summary>
	/// <returns></returns>
	const bool GetIsDead() const
	{
		return isDead;
	}

private:
	// オブジェクト
	std::unique_ptr<Object3d> object3d = nullptr;

	Vector3 velocity = { 0,0,0 };

	// 生存時間
	uint32_t lifeTimer = 0;
	const uint32_t MAXLIFETIME = 30;

	float easeTimer = 0;
	const float MAXEASETIME = 30;

	// 終了かどうか
	bool isDead = false;

	bool changeScale_ = false;

};



class ObjParticleManager
{
public:
	// インスタンス
	static ObjParticleManager* GetInstance();

	struct ParticlePreset
	{
		Vector3 pos_;
		Vector2 velocityMinMax = { -1.0,1.0 };
		size_t volume = 10;
		float scale = 1.0f;
		Vector4 color = { 1,1,1,1 };
	};

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 爆発演出
	/// </summary>
	/// <param name="pos">発生位置</param>
	/// <param name="minMax">広がり値</param>
	/// <param name="volume">一回で出す量</param>
	/// <param name="scale">オブジェクトスケール</param>
	/// <param name="color">オブジェクトカラー</param>
	void SetAnyExp(const Vector3& pos,Vector2 velocityMinMax = { -1.0,1.0 },size_t volume = 10,float scale = 1.0f,Vector4 color = { 1,1,1,1 },bool changeScale = true);
	/// <param name="ParticlePreset">プリセット</param>
	void SetAnyExp(ParticlePreset preset);


		/// </summary>
	/// <param name="pos">発生位置</param>
	/// <param name="minMax">広がり値</param>
	/// <param name="volume">一回で出す量</param>
	/// <param name="scale">オブジェクトスケール</param>
	/// <param name="color">オブジェクトカラー</param>
	void SetCharge(const Vector3& pos,Vector2 velocityMinMax = { -10.0,10.0 },size_t volume = 2,float scale = 0.1f,Vector4 color = { 1,1,1,1 },bool changeScale = false);
	/// <param name="ParticlePreset">プリセット</param>
	void SetCharge(ParticlePreset preset);

private:
	std::list<std::unique_ptr<OBJParticle>> objParticles_;

	// モデル
	Model* model_ = nullptr;

public:
	ObjParticleManager() = default;
	~ObjParticleManager() = default;
	ObjParticleManager(const ObjParticleManager&) = default;
	const ObjParticleManager& operator=(const ObjParticleManager&) = delete;
};

