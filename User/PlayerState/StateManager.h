#pragma once
#include "Defineder.h"
#include "Pading.h"

ALICE_SUPPRESS_WARNINGS_BEGIN
#include <memory>
#include <stack>
ALICE_SUPPRESS_WARNINGS_END

#include "DirectXCommon.h"
#include "Player2.h"


class StatePattern;
class Input;

class StateManager
{
	

protected:

	TTEngine::DirectXCommon* dxCommon_;
	
	Player2* player_;
	std::stack<std::shared_ptr<StatePattern>>_state;

public:
	// コンストラクタとデストラクタ
	StateManager(TTEngine::DirectXCommon* dxCommon, Player2* player);
	~StateManager();

	TTEngine::DirectXCommon* GetDxCommon() { return dxCommon_; }

	// 各シーンのInitializeを呼び出す
	void StateInitialize();
	// 各シーンのUpdateを呼び出す
	void StateUpdate(Input* input);
	// 各シーンのDrawを呼び出す
	void StateDraw();

	// シーンを変更する番号をセット
	void ChangeStateNum(int number);
	// シーンを変更する
	void ChangeState();
};
