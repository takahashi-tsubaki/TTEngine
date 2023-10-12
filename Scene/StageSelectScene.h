#pragma once
#include "IScene.h"
#include "SceneInc.h"
/// <summary>
/// ステージセレクトシーン
/// </summary>
class StageSelectScene :
    public IScene
{
protected:
	SceneManager* controller_;
	Input* input_ = nullptr;
	GamePad* gamePad_ = nullptr;

	Sprite* sprite_ = nullptr;
	Sprite* sprite2_ = nullptr;
	Sprite* alart = nullptr;

	Model* model = nullptr;
	Object3d* object = nullptr;

	PostEffect* postEffect = nullptr;

	Camera* camera_ = nullptr;

	GameCamera* gameCamera = nullptr;


	FbxModel* fbxModel = nullptr;
	FbxObject3d* fbxObject = nullptr;

	//天球
	Object3d* skydomeO_ = nullptr;
	Model* skydomeM_ = nullptr;


	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;

	CollisionManager* colMan = nullptr;

	WorldTransform followPos;
	WorldTransform targetPos;

	SceneObjects* sceneObj_;

	Sprite* playerHpSprite_ = nullptr;
	Sprite* enemyHpSprite_ = nullptr;

	Vector3 nowEye;

	bool isTransition = false;
	bool addScale = false;
	bool subScale = true;

	Vector3 scale = { 1,1,1 };

public:
	//コンストラクタとデストラクタ
	StageSelectScene(SceneManager* controller,SceneObjects* sceneObj);
	~StageSelectScene() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	//更新
	void Update(Input* input,GamePad* gamePad) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	//シーン遷移
	void SceneTransition();

	//void Pause(Input* input);
};

