#pragma once
#include "IScene.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "ResultScene.h"

enum SceneState
{
	S_TITLE = 1,
	S_PLAY = 2,
	S_RESULT = 3,
};