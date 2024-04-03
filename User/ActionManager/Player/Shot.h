#pragma once
#include "Action.h"
#include <list>
#include <memory>
enum ShotType
{
	Nasi = 0,
	ONE,
	RAPID
};

class PlayerCharacter;
class EnemyCharacter;

class Shot : public Action
{
protected:
	PlayerActionManager* ActionManager_;

public:
	Shot(PlayerActionManager* ActionManager);
	~Shot() override;

	void StaticInitialize();

	void Initialize(FbxObject3d* object, EnemyCharacter* enemy, SceneObjects* sceneObj) override;
	void Update(Input* input, GamePad* gamePad) override;
	void Draw() override;

	void Reset();

	//std::list<std::unique_ptr<PlayerBullet>> GetBulletList() { return bullets_; }

	//void Distance(Vector3 start, Vector3 goal);

	//// 2点間の距離
	//void DistanceTwoPoints(Vector3 start, Vector3 goal);

private:
	FbxObject3d* object_;

	SceneObjects* sceneObj_;

	Vector3 velocity_{0, 0, 0};
	PlayerCharacter* player_ = nullptr;
	EnemyCharacter* enemy_ = nullptr;
	Vector3 Distance_{0, 0, 0};
	Vector3 enemyPos;
	Vector3 playerPos;

	Model* bulletM_ = nullptr;

	int MaxBulletSize_;
	int bulletSize_;
	int shotType_;

	float speed_;
	float scale_ = 0.01f;
	float keyPressTimer_;
	float bulletSizeUpTimer_;
	float bulletCoolTimer_;
	float koutyokuTimer_ = 120.0f;

	bool isShot = false;

	bool isEffect = false;
	bool isSubColor = false;

	// 音を止める関数
	IXAudio2SourceVoice* pSourceVoice[10] = {0};

};
