#pragma once
#include "IScene.h"
#include "SceneInc.h"
#include "BulletManager.h"


/// <summary>
/// ゲームプレイシーン
/// </summary>
///

enum Number
{
	Zero = 0,
	One = 1,
	Two = 2,
	Twenty = 20, 
	Thirty = 30,
	Fifty = 50,
	Sixty = 60,
	Ninety = 90,
	HundredTwenty = 120,
};

enum Size {
	OneTimes = 1,
	TwoTimes = 2,

	TenTimes = 10,


};



class PlayScene :
	public IScene
{
protected:
	SceneManager* controller_;
	Input* input_ = nullptr;
	GamePad* gamePad_ = nullptr;

#pragma region スプライト関連

	Sprite* sprite_ = nullptr;
	Sprite* sprite2_ = nullptr;
	Sprite* alart = nullptr;//!マーク

	Sprite* damageSP_ = nullptr;//hp減少バー
	Sprite* isFightSP_ = nullptr;//Fightの文字
	Sprite* startSp_ = nullptr;//敵を倒せのスプライト
	Sprite* finishSP_ = nullptr;//FINISHの文字

	Sprite* transSP_ = nullptr;

	Sprite* pauseSP_ = nullptr;

	Sprite* sousaSP_ = nullptr;



#pragma endregion スプライト関連

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


	/*Character* Player_ = nullptr;
	Character* Enemy_ = nullptr;*/

	PlayerCharacter* player = nullptr;
	EnemyCharacter* enemy = nullptr;
#pragma region 演出面で必要なもの

	bool isTransition = true;
	Vector3 scale = { 1,1,1 };

	bool isStartSign = true;
	int startSignCount = 0;
	Vector3 cameraDis;
	float addSpeed = 0.1f;
	Vector3 GoalPos;

	bool isReady = false;
	float startSpSize = 0.0f;
	int readyCount = 0;

	float addSize = 0.05f;
	float decSize = 0.1f;

	bool isFight = false;
	int fightSpCount = 0; // スプライト用のカウント
	float SpAlpha = 1.0f;
	float decreaseAlpha = 0.05f;
	float SpSize = 1.0f;



	const float change = 0.5f;

	float hpSpSize = 0.0f;
	float addHpSize = 0.1f;

	bool isFinish = false;
	Vector2 finishSpPos = {WinApp::window_width, WinApp::window_height / 2 };
	int isFinishSpCount = 0;//FIHISHSP_が画面に描画される時間
	float addfinishSpeed = 0.0f;
	Vector3 addRotation;

	Vector3 finishCameraPlayerPos;
	Vector3 finishCameraPlayerTarget;

	Vector3 finishCameraEnemyPos;
	Vector3 finishCameraEnemyTarget;

	float transObjAlpha = 0.0f;
	float addAlpha = 0.018f;
#pragma endregion

#pragma region 音関連

	// 音関係まとめ

	TTEngine::Audio* audio = nullptr;

	// 音を止める関数
	IXAudio2SourceVoice* pSourceVoice[10] = {0};

	int soundCheckFlag = 0;
#pragma endregion

std::list<std::unique_ptr<PlayerBullet>> bullets_;

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

	/// <summary>
	/// スタート演出
	/// </summary>
	void StartSign(Input* input,GamePad* gamepad);


	//playSceneでのみ使う各種パラメーターのリセット
	void ResetParam();

	void gameOverAnimetion();

	void gameClearAnimetion();

	void finishPlayerCamera();

	void finishEnemyCamera();

	//void Pause(Input* input);
};

