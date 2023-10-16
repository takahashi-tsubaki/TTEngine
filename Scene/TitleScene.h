#pragma once
#include "IScene.h"
#include "SceneInc.h"
#include "SceneObjects.h"
#include <map>
/// <summary>
/// タイトルシーン
/// </summary>
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

	//シーン遷移用のフラグ
	bool isTransition = false;
	Vector3 scale = { 1,1,1 };

	//タイトルアニメーション用の天球回転に必要な変数
	Vector3 skydomeRotate = { 0,0,0 };
	float rotateSpeed = 0.001f;

	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;

	//シーン遷移用の変数たち
	float reduction = 0.05f;//縮小
	float expansion = 0.05f;//拡大

	Vector3 transScale_ = { 1,1,1 };//シーン遷移用のスケール
	Vector3 playerScale = { 1,1,1 };
	Vector3 enemyScale = { 1,1,1 };

	Vector3 transPos = { 0,600,0 };
	float transSpeed = 15;

	bool cameraRise = false;//カメラの上昇するかどうかのフラグ
	float riseSpeed = 0;//上昇用変数

	const float change = 1.0f;
	float easeTimer = 0;

	Vector2 spritePos = {100,-1000};

public:
	//コンストラクタとデストラクタ
	TitleScene(SceneManager* controller,SceneObjects* sceneObj);
	~TitleScene() override;

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
	/// パラメーターのリセット
	/// </summary>
	void ParamReset();
	//シーン遷移
	void SceneTransition();

	//シーン遷移フラグのセット
	void SetTransition(bool Transition) {
		isTransition = Transition;
	}
		//シーン遷移フラグのゲット
	bool GetIsTransiton() {
		return isTransition;
	}

//void Pause(Input* input);
};

