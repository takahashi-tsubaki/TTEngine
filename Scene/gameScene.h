#pragma once
#include "DirectXCommon.h"
#include "Input.h"

#include "Sprite.h"

#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "Object3d.h"

#include "ParticleManager.h"

#include "Vector3.h"

#include <map>

struct LevelEditer;

class GameScene
{

public:

	GameScene();
	~GameScene();

	void Initalize();

	void Update();

	void Draw();

	

private:
	DirectXCommon* dxCommon_ = nullptr;

	Input* input_ = nullptr;


	Sprite* sprite_ = nullptr;
	Sprite* sprite2_ = nullptr;


	Camera* camera_ = nullptr;
	Light* light_ = nullptr;

	Model* modelSkydome_ = nullptr;
	Object3d* objSkydome_ = nullptr;

	Model* modelParticle_ = nullptr;
	Particle* particle_ = nullptr;


	Model* modelParticle2_ = nullptr;
	Particle* particle2_ = nullptr;

	Model* modelPlayer_ = nullptr;
	Object3d* objPlayer_ = nullptr;

	Model* modelBume_ = nullptr;
	Object3d* objBume_ = nullptr;

	bool isParticle = false;

	LevelEditer* levelEditer = nullptr;
	std::map<std::string, Model*> models;
	std::vector<Object3d*> objects;
};