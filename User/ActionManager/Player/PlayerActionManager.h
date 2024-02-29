#pragma once
#include <memory>
#include <stack>
#include "Input.h"
#include "GamePad.h"
#include "FbxObject3d.h"

class EnemyCharacter;

class Action;

class PlayerActionManager {

private:
	std::shared_ptr<Action> action_;

public:
	// コンストラクタ
	PlayerActionManager();
	// デストラクタ
	~PlayerActionManager();

	void ActionInitialize(FbxObject3d* object, EnemyCharacter* enemy);

	void ActionUpdate(Input* input, GamePad* gamePad);

	void ActionDraw();
	// シーンを変更する
	void ChangeAction(Action* Action);

public:
	void SetActionNum(int actionNum) { actionNum_ = actionNum; }

	int GetActionNum() { return actionNum_; }

public:
	bool KeyPushWASD(Input* input);

	bool isAnyLStick(GamePad* gamePad);

private:
	FbxObject3d* object_;

	EnemyCharacter* enemy_;

	int actionNum_ = 0;

	bool pushWASD_ = false;
	bool padLStick_ = false;
};
