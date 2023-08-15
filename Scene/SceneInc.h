#pragma once
#include "IScene.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "ResultScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"

enum SceneState
{
	S_TITLE = 1,
	S_PLAY = 2,
	S_CLEAR = 3,
	S_OVER = 4,
};