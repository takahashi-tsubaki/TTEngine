#pragma once
#include <memory>
#include <stack>
#include "Input.h"
#include "GamePad.h"
#include "FbxObject3d.h"


class TutorialPlayer;
class TutorialEnemy;
class SceneObjects;

class TutorialManager
{

private:
	std::shared_ptr<TutorialAction> action_;

public:
	// コンストラクタ
	TutorialManager();
	// デストラクタ
	~TutorialManager();

	//std::list<std::unique_ptr<PlayerBullet>> GetBullets() { return bullets_; }
	//void SetBullets(std::list<std::unique_ptr<PlayerBullet>> bullet) { bullets_ = bullet; }

	void ActionInitialize(FbxObject3d* object,TutorialEnemy* enemy,SceneObjects* sceneObj);

	void ActionUpdate(Input* input,GamePad* gamePad);

	void ActionDraw();


	// シーンを変更する
	void ChangeAction(Action* Action);

public:
	void SetActionNum(int actionNum) {
		actionNum_ = actionNum;
	}

	int GetActionNum() {
		return actionNum_;
	}

public:
	bool KeyPushWASD(Input* input);

	bool isAnyLStick(GamePad* gamePad);

	bool KeyTriggerArrow(Input* input);

	bool isAnyLStickOffTrigger(GamePad* gamePad);


	TutorialAction* GetAction() {
		return action_.get();
	}

private:
	FbxObject3d* object_;

	TutorialPlayer* player_;
	TutorialEnemy* enemy_;

	SceneObjects* sceneObj_;

	//// プレイヤーの弾モデル関連
	//std::list<std::unique_ptr<PlayerBullet>> bullets_;

	int actionNum_ = 0;
	int stepDirect;
	int isNowAction = 0;

	bool pushWASD_ = false;
	bool padLStick_ = false;

};

