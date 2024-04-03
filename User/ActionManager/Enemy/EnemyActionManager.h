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

// 自機との距離
enum RangePattern
{
	CROSSRange = 1,
	MIDDLERange,
	LONGRange,
};


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

	void DistanceTwoPoints();

public:

	//プレイヤーとの距離に応じての行動パターン

	void crossRangePattern();//近距離

	void middleRangePattern();//中距離

	void longRangePattern();//遠距離


public:
	void SetActionNum(int actionNum) { actionNum_ = actionNum; }

	int GetActionNum() { return actionNum_; }

private:
	FbxObject3d* object_;
	PlayerCharacter* player_;
	Vector3 distance_;
	int actionNum_ = 0;

	float moveDirectTimer = 90;
	int moveDirect_;

	int rangepattern;

	int crossRange = 33;
	int middleRange = 66;
	int longRange = 100;
	
	// ランダム生成器の初期化
	std::random_device random;

};
