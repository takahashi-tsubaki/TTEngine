#pragma once

#include "Particle.h"

/// <summary>
/// パーティクルマネージャー
/// </summary>
class ParticleM
{


public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParticleM();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ParticleM();

	void LoadTexture(const std::string& fileName);

		/// <summary>
	/// 初期化
	/// </summary>
	void Init();

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
	/// 爆発
	/// </summary>
	void CallExp(const Vector3& pos);

	/// <summary>
	/// 小爆発
	/// </summary>
	void CallSmallExp(const Vector3& pos);

	void SetDrawBlendMode(int SetBlendMode);

	void Charge(const Vector3& pos);

	/// <summary>
	/// マネージャーの座標をもとにランダムに放出する
	/// </summary>

	void RandParticle(Vector3 pos);

	/// <summary>
	/// パーティクルのリセット
	/// </summary>
	void Reset();


private: // メンバ変数


		// インスタンス
	static ParticleM* particleManager;

	// パーティクル
	std::unique_ptr<Particles> particles1;
	std::unique_ptr<Particles> particles2;


	// ローカルスケール
	WorldTransform wtf_;
	Matrix4 bill;
	Vector4 color_ = { 1,1,1,1 };
	Vector3 Distance_;
	float speed_ = 1.0f;
};
