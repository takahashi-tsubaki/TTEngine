#pragma once
#include "IScene.h"
#include "SceneInc.h"

class PlayScene;

/// <summary>
/// ポーズシーン
/// </summary>
class PauseScene :
	public IScene
{
protected:
	SceneManager* controller_;
	Input* input_ = nullptr;
	GamePad* gamePad_ = nullptr;

	Sprite* sprite_ = nullptr;
	Sprite* sprite2_ = nullptr;
	Sprite* operationSP_ = nullptr;
	Sprite* fCheckSP_ = nullptr;


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


	PlayerCharacter* player = nullptr;
	EnemyCharacter* enemy = nullptr;


	CollisionManager* colMan = nullptr;

	SceneObjects* sceneObj_;

	Sprite* playerHpSprite_ = nullptr;
	Sprite* enemyHpSprite_ = nullptr;

	Vector3 nowEye;

	bool finalCheck = false;
	int checkNum = 0;

public:
	//コンストラクタとデストラクタ
	PauseScene(SceneManager* controller,SceneObjects* sceneObj);
	~PauseScene() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

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

	//void Pause(Input* input);
};

