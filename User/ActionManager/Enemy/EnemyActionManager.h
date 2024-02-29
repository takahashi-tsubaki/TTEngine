#pragma once
#include "FbxObject3d.h"
#include "GamePad.h"
#include "Input.h"
#include <memory>
#include <stack>

class EnemyAction;

class EnemyActionManager
{
private:

	std::shared_ptr<EnemyAction> action_;

public:
	// コンストラクタ
	EnemyActionManager();
	// デストラクタ
	~EnemyActionManager();

	void ActionInitialize(FbxObject3d* object);

	void ActionUpdate();

	void ActionDraw();
	// シーンを変更する
	void ChangeAction(EnemyAction* Action);

public:
	void SetActionNum(int actionNum) { actionNum_ = actionNum; }

	int GetActionNum() { return actionNum_; }

private:
	FbxObject3d* object_;
	int actionNum_ = 0;
};
