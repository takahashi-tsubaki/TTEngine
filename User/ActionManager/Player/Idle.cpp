#include "Idle.h"

#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

Idle::Idle(PlayerActionManager* ActionManager)
{
	ActionManager_ = ActionManager;
}

Idle::~Idle() {}

void Idle::Initialize(FbxObject3d* object, EnemyCharacter* enemy, SceneObjects* sceneObj) {
	object_ = object;
	enemy_ = enemy;
	sceneObj_ = sceneObj;
}

void Idle::Update(Input* input, GamePad* gamePad) {
	
	object_->PlayAnimation(FBXAnimetion::IdeL, true);
	if (gamePad->StickInput(L_UP) || input->PushKey(DIK_W)) {

			/*velocity_ += {0, 0, moveSpeed};

			fbxVelocity_ += {0, 0, (moveSpeed / fbxScale_)};*/
	}
	
	else
	{

	}
}

void Idle::Draw() {}

void Idle::ParticleDraw(ID3D12GraphicsCommandList* cmdList) {

	if (cmdList) {
	}
}
