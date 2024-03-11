#pragma once
#include "FbxObject3d.h"
#include "GamePad.h"
#include "Input.h"
#include <memory>
#include <stack>

#include <random>

// クラスの前方宣言
class PlayerCharacter;
class EnemyCharacter;
class EnemyAction;
class EnemyShot;


class EnemyActionManager
{
private:

	std::shared_ptr<EnemyAction> action_;

public:
	// コンストラクタ
	EnemyActionManager();
	// デストラクタ
	~EnemyActionManager();

	void ActionInitialize(FbxObject3d* object, PlayerCharacter*player);

	void ActionUpdate();

	void ActionDraw();
	// シーンを変更する
	void ChangeAction(EnemyAction* Action);

	int generateRandomAction(int min, int max);

public:
	void SetActionNum(int actionNum) { actionNum_ = actionNum; }

	int GetActionNum() { return actionNum_; }

private:
	FbxObject3d* object_;
	PlayerCharacter* player_;
	int actionNum_ = 0;

	float moveDirectTimer = 0;
	int moveDirect_;

	
	// ランダム生成器の初期化
	std::random_device random;

};
