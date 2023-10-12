#pragma once
#include <Windows.h>

#include <tchar.h>
/// <summary>
/// WinAppクラス
/// </summary>
class WinApp
{
public:



	static LRESULT WindowProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam);


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	//getter
	HWND Gethwnd()const {return hwnd;}
	HINSTANCE GetInstance()const {return w.hInstance;}

	//解放処理
	void Finalize();

	//メッセージ処理
	bool ProcessMessage();

public:

	static const int window_width = 1280;
	static const int window_height = 720;

private:

	//ウィンドウクラスの設定
	WNDCLASSEX w = {};

	//ウィンドウハンドル
	HWND hwnd = nullptr;

};