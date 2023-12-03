#pragma once
#pragma warning(push)
#pragma warning(disable : 4820)
#pragma warning(disable : 4514)
#pragma warning(disable : 4061)
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include <wrl.h>

#pragma warning(pop)
#include "Camera.h"
#include "FBXLoader.h"
#include "FBXModel.h"
#include "WorldTransform.h"

class FbxObject3d {
protected: // エイリアス
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 定数バッファ用データ構造体（座標変換行列用）
	struct ConstBufferDataTransform {
		XMMATRIX viewproj;  // ビュープロジェクション行列
		XMMATRIX world;     // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
	};
	// ボーンの最大数
	static const uint32_t MAX_BONES = 320;

	// 定数バッファ用のデータ構造体(スキニング)
	struct ConstBufferDataSkin {
		XMMATRIX bones[MAX_BONES];
	};

public: // 静的メンバ関数
	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	// setter
	static void SetDevice(ID3D12Device* device) { FbxObject3d::device_ = device; }
	static void SetCamera(Camera* camera) { FbxObject3d::camera_ = camera; }

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device_;
	// カメラ
	static Camera* camera_;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// インスタンス取得
	/// </summary>
	static std::unique_ptr<FbxObject3d> Create();

	/// <summary>
	/// 大きさの設定
	/// </summary>
	/// <param name="m_Pos">座標</param>
	void SetScale(const Vector3& scale) { worldTransform.scale_ = scale; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="m_Pos">座標</param>
	void SetPosition(const Vector3& position) { worldTransform.translation_ = position; }

	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rotate"></param>
	void SetRotate(const Vector3& rotate) { worldTransform.rotation_ = rotate; }

	/// <summary>
	/// モデルをセット
	/// </summary>
	/// <param name="fbxmodel"></param>
	void SetModel(FbxModel* fbxmodel) { fbxmodel_ = fbxmodel; }

	/// <summary>
	/// フレームカウント指定
	/// </summary>
	/// <param name="flame"></param>
	void SetFlame(uint32_t flame);

	/// <summary>
	/// アニメーション再生用
	/// </summary>
	void AnimPlay();
	void AnimStop();
	void AnimIsRotateChange();

	/// <summary>
	/// カメラのゲッター
	/// </summary>
	/// <returns></returns>
	Camera GetCamera();

	/// <summary>
	/// アニメーションタイムゲッター
	/// </summary>
	/// <returns></returns>
	FbxTime GetCurrentTimer();
	FbxTime GetEndTime();
	bool GetIsAnimRot();
	uint32_t ConvertFbxTimeToInt(FbxTime time); // FbxTime型変数をintに変換

	/// <summary>
	/// ワールドトランスフォーム取得
	/// </summary>
	/// <returns></returns>
	WorldTransform GetWorldTransform();
	WorldTransform* GetWorldTransformPtr();

	/// <summary>
	/// ボーン用
	/// </summary>
	std::vector<Matrix4> bonesMat;
	void ResizeBonesMat(
	    std::vector<FbxModel::Bone> bones); // ボーンのサイズをクラスターボーンに合わせる
	bool isBonesWorldMatCalc = false; // ボーンのワールド座標上での計算をするかどうか

	std::vector<Matrix4>* GetBonesMatPtr();   // ボーンのワールド行列ポインタを渡す
	void SetIsBonesWorldMatCalc(bool isCalc); // ボーン計算フラグのセッター

	/// <summary>
	/// アニメーション開始
	/// </summary>
	void PlayAnimation(uint32_t animationNum);

	// 補間アニメーションカウント
	void AnimFlameInter(FbxTime nowCount, FbxTime maxCount);

	WorldTransform worldTransform;

	ID3D12Resource* GetConstBuff() { return constBuffTransform.Get(); };

protected: // メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform;
	// 定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBuffSkin;
	//// ローカルスケール
	// XMFLOAT3 scale = { 1,1,1 };
	//// X,Y,Z軸回りのローカル回転角
	// XMFLOAT3 rotation = { 0,0,0 };
	//// ローカル座標
	// XMFLOAT3 m_Pos = { 0,0,0 };
	//// ローカルワールド変換行列
	// XMMATRIX matWorld;
	//  モデル
	FbxModel* fbxmodel_ = nullptr;

	// 1フレームの時間
	FbxTime frameTime;
	// アニメーション開始時間
	FbxTime startTime;
	// アニメーション終了時間
	FbxTime endTime;
	// 現在時間(アニメーション)
	FbxTime currentTime;
	// アニメーション再生中
	bool isPlay = false;
	// アニメーションフレーム指定時フラグ
	bool isChangeFlame = false;
	// アニメーションフラグ
	bool isAnim = true;
	// アニメーション繰り返すか
	bool animRot = true;
};
