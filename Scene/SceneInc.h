#pragma once
#include "IScene.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"
#include "PauseScene.h"
#include "StageSelectScene.h"

enum SceneState
{
	S_TITLE = 1,
	S_PLAY = 2,
	S_CLEAR = 3,
	S_OVER = 4,
	S_PAUSE = 5,
	S_SELECT = 6,
};