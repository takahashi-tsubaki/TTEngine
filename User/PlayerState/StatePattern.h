#pragma once
#include "StateManager.h"
#include "Input.h"
#include "GamePad.h"

class StatePattern
{
protected:
	StateManager* controller;

public:
	// コンストラクタ
	StatePattern();
	// デストラクタ
	virtual ~StatePattern();
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input"></param>
	/// <param name="gamePad"></param>
	virtual void Update(Input* input, GamePad* gamePad) = 0;
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;
};
