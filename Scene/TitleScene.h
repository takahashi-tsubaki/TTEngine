#pragma once
#include "IScene.h"
#include "SceneInc.h"
class TitleScene :
    public IScene
{
protected:
    SceneManager* controller_;
	Input* input_ = nullptr;
	GamePad* gamePad_ = nullptr;

	Sprite* sprite_ = nullptr;
	Sprite* sprite2_ = nullptr;

	Model* model = nullptr;
	Object3d* object = nullptr;

	PostEffect* postEffect = nullptr;


	FbxModel* fbxModel = nullptr;
	FbxObject3d* fbxObject = nullptr;

	//“V‹…
	Object3d* skydomeO_ = nullptr;
	Model* skydomeM_ = nullptr;


	WorldTransform followPos;
	WorldTransform targetPos;

public:

    TitleScene(SceneManager* controller);
    ~TitleScene() override;

    void Initialize()override;

    void Update(Input* input) override;

    void Draw() override;

    //void Pause(Input* input);
};

