#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"

#include "Input.h"
#include "GamePad.h"
#include "FPS.h"

#include "ImguiManager.h"
#include "Sprite.h"
#include"Light.h"
#include "ParticleManager.h"
#include "FbxLoader.h"
/// <summary>
/// フレームワーク
/// </summary>
class Framework
{
public:

	//初期化
	virtual void Initialize();

	//終了
	virtual void Finalize();

	//毎フレーム更新
	virtual void Update();

	//描画
	virtual void Draw() = 0;

	/// <summary>
	/// 終了するか豪華のフラグのセット
	/// </summary>
	/// <param name="request"></param>
	virtual void SetRequest(bool request) {
		endRequest_ = request;
	}

//終了チェック
	virtual bool IsEndRequest() {
		return endRequest_;
	}

//デストラクタ
	virtual ~Framework() = default;

	//実行
	void Run();

protected:
	//FPS
	TTEngine::FPS* fps = new TTEngine::FPS;
	WinApp* winApp = nullptr;
	Input* input = nullptr;
	GamePad* gamePad_ = nullptr;
	TTEngine::DirectXCommon* dxCommon_ = nullptr;
	ImguiManager* imgui = nullptr;
	bool endRequest_ = false;
};
