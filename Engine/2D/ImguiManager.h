#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"

#include <imgui_impl_win32.h>
#include "imgui_impl_dx12.h"
#include <imgui.h>
/// <summary>
/// ImGuiクラス
/// </summary>
class ImguiManager
{
public:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="winApp"></param>
	/// <param name="dxCommon"></param>
	void Initialize(WinApp* winApp, TTEngine::DirectXCommon* dxCommon);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static ImguiManager* GetInstance();

	/// <summary>
	/// 受付開始
	/// </summary>
	void Begin();

	/// <summary>
	/// 受付終了
	/// </summary>
	void End();

	/// <summary>
	/// 画面への描画
	/// </summary>
	void Draw();

private:

	TTEngine::DirectXCommon* dxCommon_ = nullptr;

	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

};
