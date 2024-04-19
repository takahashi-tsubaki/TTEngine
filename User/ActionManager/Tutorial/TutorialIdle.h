#pragma once
#include "TutorialAction.h"

class TutorialPlayer;
class TutorialEnemy;

class TutorialIdle :
    public TutorialAction
{
protected:
	TutorialManager* ActionManager_;

public:
	TutorialIdle(TutorialManager* ActionManager);
	~TutorialIdle() override;

	void Initialize(FbxObject3d* object,TutorialEnemy* enemy,SceneObjects* sceneObj) override;
	void Update(Input* input,GamePad* gamePad) override;
	void Draw() override;
	void ParticleDraw(ID3D12GraphicsCommandList* cmdList) override;

private:

	TutorialPlayer* player_ = nullptr;
	TutorialEnemy* enemy_ = nullptr;
	FbxObject3d* object_;
	SceneObjects* sceneObj_;
	Vector3 velocity_{ 0, 0, 0 };
	float speed = 0.5f;
};

