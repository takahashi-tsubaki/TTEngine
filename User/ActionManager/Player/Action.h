#pragma once
#include "FbxObject3d.h"
#include "Input.h"
#include "GamePad.h"
#include "PlayerActionManager.h"

#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

#include "BulletManager.h"

#include "Audio.h"
class SceneObjects;
class Action {

public:

	Action();
	virtual ~Action();

	virtual void Initialize(FbxObject3d* object, EnemyCharacter* enemy, SceneObjects* sceneObj) = 0;

	virtual void Update(Input* input, GamePad* gamePad) = 0;

	virtual void Draw() = 0;

	bool GetIsNowShot() { return isNowShot_; }
	void SetIsNowShot(bool shot) { isNowShot_ = shot; }

	bool GetIsNowStep() { return isNowStep_; }
	void SetIsNowStep(bool step) { isNowStep_ = step; }

	bool GetIsStandBy() { return isNowStandBy_; }
	void SetIsStandBy(bool StandBy) { isNowStandBy_ = StandBy; }

	//音声のgetterとsetter
	TTEngine::Audio* GetAudio() { return audio_; }
	void SetAudio(TTEngine::Audio* audio) { audio_ = audio; }

	void LoadSound();

protected:

	PlayerActionManager* actionManager_;

	bool isNowShot_;

	bool isNowStep_;

	bool isNowStandBy_;

#pragma region 音関連

	// 音関係まとめ

	TTEngine::Audio* audio_ = nullptr;

	// 音を止める関数
	IXAudio2SourceVoice* pSourceVoice[10] = {0};

	int soundCheckFlag = 0;

#pragma endregion


};
