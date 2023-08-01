#pragma once
#include "IScene.h"
#include "SceneInc.h"
class PlayScene :
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

	Camera* camera_ = nullptr;

	GameCamera* gameCamera = nullptr;


	FbxModel* fbxModel = nullptr;
	FbxObject3d* fbxObject = nullptr;

	//“V‹…
	Object3d* skydomeO_ = nullptr;
	Model* skydomeM_ = nullptr;


	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;

	CollisionManager* colMan = nullptr;
public:

    PlayScene(SceneManager* controller);
    ~PlayScene() override;

    void Initialize()override;

    void Update(Input*input) override;

    void Draw() override;

    //void Pause(Input* input);
};

