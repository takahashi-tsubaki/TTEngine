#pragma once
#include "IScene.h"
#include "SceneInc.h"
/// <summary>
/// ゲームオーバーシーン
/// </summary>
class GameOverScene :
	public IScene
{
protected:
	SceneManager* controller_;
	Input* input_ = nullptr;
	GamePad* gamePad_ = nullptr;

	Sprite* sprite_ = nullptr;
	Sprite* sprite2_ = nullptr;

	Sprite* loseSP_ = nullptr; // LOSEの文字

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

	float loseSpSize = 1.0f;
	float loseSPAlpha = 1.0f;

	int gameOverCount = 0;
	int gameOverAnimeCount = 0;
	Vector3 enemyPos;
	float addPos = 0.5f;
	float color = 1.0f;

public:
	//コンストラクタとデストラクタ
	GameOverScene(SceneManager* controller,SceneObjects* sceneObj);
	~GameOverScene() override;

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

	void GameOverAnime();

	void ResetParam();

	//void Pause(Input* input);
};


