#pragma once
#include "FbxLoader.h"
#include "FbxModel.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include<DirectXMath.h>
#include <string>

class FbxObject3d
{
public:
	//ボーンの最大数
	static const int MAX_BONES = 1000;//HLSL側を合わせる

protected://エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectXを省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://サブクラス
	//定数バッファ用データ構造体(座標変換行列用)
	struct ConstBufferDataTransfrom
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};
public://情的メンバ関数
	//setter
	static void SetDevice(ID3D12Device* device) { FbxObject3d::device = device; }
	static void SetCamera(Camera* camera) { FbxObject3d::camera = camera; }

	static void CreateGraphicsPipeline();

private://情的メンバ変数
	//デバイス
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;

	static ComPtr<ID3D12RootSignature>rootsignature;
	//パイプラインステート
	static ComPtr<ID3D12PipelineState> pipelineState;

public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="fbxModel"></param>
	void SetModel(FbxModel* fbxModel) { this->fbxModel = fbxModel; }

	void SetScale(XMFLOAT3 scale) {this->scale = scale;}

	void SetPosition(XMFLOAT3 position) { this->position = position; }

	/// <summary>
	/// アニメーション開始
	/// </summary>
	void PlayAnimetion(int AnimNum);

protected:
	//定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform;

	//定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBuffSkin;

	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;

	FbxModel* fbxModel = nullptr;

	//1フレームの時間
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間
	FbxTime currentTime;
	//アニメーション再生中
	bool isPlay = false;

};