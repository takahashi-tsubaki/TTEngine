#pragma once
#include "DirectXCommon.h"
#include "SceneManager.h"
#include "Input.h"
class IScene
{

protected:

	SceneManager* controller;

public:

	IScene();

	virtual ~IScene();

	virtual void Initialize(DirectXCommon* dxcommon) = 0;

	virtual void Update(Input*input) = 0;

	virtual void Draw(DirectXCommon* dxcommon) = 0;

};

