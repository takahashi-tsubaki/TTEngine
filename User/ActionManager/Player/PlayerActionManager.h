#pragma once
#include <memory>
#include <stack>
#include "Input.h"
#include "GamePad.h"
#include "FbxObject3d.h"

#include "PlayerBullet.h"


//クラスの前方宣言
class PlayerCharacter;
class EnemyCharacter;
class Action;

class Shot;

class PlayerActionManager {

private:
	std::shared_ptr<Action> action_;

public:
	// コンストラクタ
	PlayerActionManager();
	// デストラクタ
	~PlayerActionManager();

	//std::list<std::unique_ptr<PlayerBullet>> GetBullets() { return bullets_; }
	//void SetBullets(std::list<std::unique_ptr<PlayerBullet>> bullet) { bullets_ = bullet; }

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

	PlayerCharacter* player_;
	EnemyCharacter* enemy_;

	//// プレイヤーの弾モデル関連
	//std::list<std::unique_ptr<PlayerBullet>> bullets_;

	int actionNum_ = 0;

	bool pushWASD_ = false;
	bool padLStick_ = false;
};
