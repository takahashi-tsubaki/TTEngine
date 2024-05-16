#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include "Defineder.h"
#include "Pading.h"

ALICE_SUPPRESS_WARNINGS_BEGIN
#include <d3dx12.h>
#include <forward_list>

#include <array>
ALICE_SUPPRESS_WARNINGS_END

#include "MyMath.h"
#include "GameCamera.h"


enum SetBlendMODE
{
	NOBlend = 0,//無し
	ADDBlend,//加算
	SUBBlend,//減算
};

/// <summary>
/// パーティクルマネージャー
/// </summary>
class Particles
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


public: // サブクラス
	// 定数バッファ用データ構造体（マテリアル）
	struct ConstBufferDataMaterial
	{
		Vector4 color; // 色 (RGBA)
	};

	// 頂点データ構造体
	struct VertexPos
	{
		Vector3 pos; // xyz座標
		float scale;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Matrix4 mat;
		Matrix4 matBillboard;	// ビルボード行列
		//float pad1;
		Vector4 color; // 色 (RGBA)


	};

	//パーティクル一粒
	struct Particle
	{

		//座標
		Vector3 position = {};
		//速度
		Vector3 velocity = {};
		//加速度
		Vector3 accel = {};
		//現在フレーム
		int frame = 0;
		//過去フレーム
		int num_frame = 0;

		//スケール
		float scale = 1.0f;
		//初期値
		float s_scale = 1.0f;
		//最終値
		float e_scale = 0.0f;

		Vector4 color; // 色 (RGBA)
	};

private: // 定数

	const int vertexCount = _countof(vertices);

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	static void StaticInitialize(ID3D12Device* device,ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();

	void SetDrawBlendMode(int SetBlendMode);

private: // 静的メンバ変数

	// デバイス
	static Microsoft::WRL::ComPtr<ID3D12Device> device;
	// コマンドリスト
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList_;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

	std::string kDefaultTextureDirectoryPath = "Resources/";

private: // メンバ変数
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource>texbuff;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// 頂点データ配列
	VertexPos vertices[ 2024 ];
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;



	//パーティクル配列
	std::forward_list<Particle>particles;

		// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;

	//ConstBufferDataMaterial* constMapMaterial = nullptr;
private:// メンバ関数

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	void InitializeDescriptorHeap();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture();

	/// <summary>
	/// モデル作成
	/// </summary>
	void CreateModel();

public: // メンバ関数
	static Particles* Create();
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Particles();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Particles();

	void LoadTexture(const std::string& fileName);
	bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// マネージャーの座標をもとにランダムに放出する
	/// </summary>

	//void RandParticle(Vector3 pos);

	/// <summary>
	/// パーティクルのリセット
	/// </summary>
	void Reset();

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	///	<param name="life">生存時間</param>
	///	<param name="position">初期座標</param>
	///	<param name="velocity">速度</param>
	///	<param name="accel">加速度</param>
	/// <param name="color">色/param>
	void Add(int life,Vector3 position,Vector3 velociy,Vector3 accel,float start_scale,float end_scale,float color);

	//void Charge(int life,Vector3 pos,Vector3 velocity,float speed);

	//void Barrier(Vector3 pos);

	double lerp(double a,double b,double t);

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="camera"></param>
	static void SetCamera(Camera* camera) {
		camera_ = camera;
	}

// ワールドトランスフォームの設定
	void SetWorldTransform(WorldTransform wtf) {
		wtf_ = wtf;
	};
// パーティクル発生位置の設定
	void Setposition(Vector3 position) {
		wtf_.translation_ = position;
	};
	/// <summary>
	/// 行列のセット
	/// </summary>
	/// <param name="mat"></param>
	void SetMatWorld(Matrix4 mat) {
		wtf_.matWorld_ = mat;
	};
	/// <summary>
	/// ビルボード行列のセット
	/// </summary>
	/// <param name="mat"></param>
	void SetBillboardMatWorld(Matrix4 mat) {
		bill = mat;
	};

	/// <summary>
	/// worldTransformのゲット
	/// </summary>
	/// <returns></returns>
	WorldTransform GetWorldTransform()
	{
		return wtf_;
	};

	void SetColor(Vector4 color) {
		color_ = color;
	}


	void subColor(float subcolor,Particle& p);

private: // メンバ変数
	static Camera* camera_;
	// ローカルスケール
	WorldTransform wtf_;
	Matrix4 bill;
	Vector4 color_ = { 1,1,1,1 };
	Vector3 Distance_;
	float speed_ = 1.0f;
};
