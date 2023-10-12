#pragma once
#include "IScene.h"
#include "SceneInc.h"
/// <summary>
/// ゲームプレイシーン
/// </summary>
class PlayScene :
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

	SceneObjects* sceneObj_;

	Sprite* playerHpSprite_ = nullptr;
	Sprite* enemyHpSprite_ = nullptr;

	Vector3 nowEye;

	bool isTransition = true;
	Vector3 scale = { 1,1,1 };

public:
	//コンストラクタとデストラクタ
	PlayScene(SceneManager* controller,SceneObjects* sceneObj);
	~PlayScene() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input"></param>
	/// <param name="gamePad"></param>
	void Update(Input* input,GamePad* gamePad) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// シーン遷移
	/// </summary>
	void SceneTransition();

	//void Pause(Input* input);
};

