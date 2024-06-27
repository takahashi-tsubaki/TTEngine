#pragma once
#include "Input.h"
#include "GamePad.h"
#include "Audio.h"

#include "Sprite.h"
#include "PostEffect.h"

#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "Object3d.h"

#include "ParticleManager.h"

#include "Vector3.h"

#include "FbxObject3d.h"

#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

#include "LevelLoader.h"

#include "BulletManager.h"

enum SpriteNumber
{

	TITLE = 1,
	STAGESELECT,
	SCENETRANS,
	SPACE,

	FIGHT = 11,
	STARTSIGN,
	STARTTUTORIAL,
	TUTORIALTEXT,
	BATTLETEXT,

	HPBAR = 21,
	DAMAGEBAR,
	ALART,
	PAUSE,
	OPERATION,
	SOUSA,
	FINALCHECK,

	WIN = 31,
	LOSE,
	FINISH,

};

class SceneManager;
/// <summary>
/// シーンオブジェクト
/// </summary>
class SceneObjects
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="controller"></param>
	void Initialize(SceneManager* controller,SceneObjects* sceneObj);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 削除
	/// </summary>
	void Delete();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

public:
	SceneObjects* sceneObj_ = nullptr;

	FbxModel* fbxModel = nullptr;
	FbxObject3d* fbxObject = nullptr;
	Sprite* sprite_ = nullptr;

	Sprite* selectSp_ = nullptr;

	Sprite* spaceButton_ = nullptr;



	//天球
	Object3d* skydomeO_ = nullptr;
	Model* skydomeM_ = nullptr;

	//天球
	Object3d* selectSkydomeO_ = nullptr;
	Model* selectSkydomeM_ = nullptr;

	Model* model = nullptr;
	Object3d* object = nullptr;

	Model* transitionM_ = nullptr;
	Object3d* transitionO_ = nullptr;

	Model* effectM_ = nullptr;
	Object3d* effectO_ = nullptr;

	Model* bulletM_ = nullptr;
	Model* homingBulletM_ = nullptr;

	Model* laserM_ = nullptr;

	//Character* Player_ = nullptr;
	//Character* Enemy_ = nullptr;

	SceneManager* controller_;

	LevelLoader* fieldRock_;
	PlayerCharacter* player = nullptr;
	EnemyCharacter* enemy = nullptr;

	std::unique_ptr<ParticleManager> particle_;


#pragma region 音関連

	// 音関係まとめ

	TTEngine::Audio* audio_ = nullptr;

	// 音を止める関数
	IXAudio2SourceVoice* pSourceVoice[10] = {0};

	int soundCheckFlag = 0;

#pragma endregion


};

