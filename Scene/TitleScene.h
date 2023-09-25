#pragma once
#include "IScene.h"
#include "SceneInc.h"
#include "SceneObjects.h"
#include <map>

struct LevelEditer;
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

	//天球
	Object3d* skydomeO_ = nullptr;
	Model* skydomeM_ = nullptr;


	WorldTransform followPos;
	WorldTransform targetPos;

	SceneObjects* sceneObj_;

	Model* modelBume_ = nullptr;
	//Object3d* objBume_ = nullptr;
	LevelEditer* levelEditer = nullptr;
	std::map<std::string, Model*> models;
	std::vector<Object3d*> objects;

	//シーン遷移用のフラグ
	bool isTransition = false;
	Vector3 scale = { 1,1,1 };

public:

    TitleScene(SceneManager* controller, SceneObjects* sceneObj);
    ~TitleScene() override;

    void Initialize()override;

    void Update(Input* input, GamePad* gamePad) override;

    void Draw() override;

	void SceneTransition();

	void SetTransition(bool Transition) { isTransition = Transition; }
	bool GetIsTransiton() { return isTransition; }

    //void Pause(Input* input);
};

