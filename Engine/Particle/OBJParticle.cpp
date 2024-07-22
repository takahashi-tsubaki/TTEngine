#include "OBJParticle.h"


OBJParticle* OBJParticle::Create(
	const Vector3& pos_,Model* model_,const Vector3& velocity_,float scale,Vector4 color,bool changeScale) {
	// インスタンス
	OBJParticle* instance = new OBJParticle();
	if ( instance == nullptr )
	{
		return nullptr;
	}

	// 初期化
	instance->Init(pos_,model_,velocity_,scale,color,changeScale);

	return instance;
}

void OBJParticle::Init(
	const Vector3& pos_,Model* model_,const Vector3& velocity_,float scale,Vector4 color,bool changeScale) {
	object3d.reset(object3d->Create());
	object3d->SetModel(model_);
	object3d->SetColor(color);
	object3d->worldTransform.translation_ = pos_;
	object3d->worldTransform.scale_ = { scale,scale,scale };

	velocity = velocity_;

	changeScale_ = changeScale;

	lifeTimer = 0;

}

void OBJParticle::Update() {
	if ( lifeTimer < MAXLIFETIME )
	{
		lifeTimer++;

		object3d->worldTransform.translation_ += velocity;

		object3d->worldTransform.rotation_ += { 30.0f,30.0f,30.0f };

		if ( changeScale_ == true )
		{
			if ( lifeTimer > MAXLIFETIME - MAXEASETIME )
			{
				if ( easeTimer < MAXEASETIME )
				{
					easeTimer++;
					object3d->worldTransform.scale_.x = ( float ) Ease::OutQuad(0.5f,0.0f,easeTimer,MAXEASETIME);
					object3d->worldTransform.scale_.y = ( float ) Ease::OutQuad(0.5f,0.0f,easeTimer,MAXEASETIME);
					object3d->worldTransform.scale_.z = ( float ) Ease::OutQuad(0.5f,0.0f,easeTimer,MAXEASETIME);
				}
			}
		}

	}
	else
	{
		isDead = true;
	}

	object3d->Update();
}

void OBJParticle::Draw() {
	object3d->Draw();
}

void ObjParticleManager::Init(Model* model) {
	model_ = model;

}

void ObjParticleManager::Update() {
	objParticles_.remove_if(
		[ ] (std::unique_ptr<OBJParticle>& objParticle)
 {
	 return objParticle->GetIsDead();
 });

	for ( std::unique_ptr<OBJParticle>& objParticle : objParticles_ )
	{
		objParticle->Update();
	}
}

void ObjParticleManager::Draw() {
	for ( std::unique_ptr<OBJParticle>& objParticle : objParticles_ )
	{
		objParticle->Draw();
	}
}

void ObjParticleManager::SetAnyExp(const Vector3& pos_,Vector2 velocityMinMax,size_t volume,float scale,Vector4 color,bool changeScale) {
	std::unique_ptr<OBJParticle> newParticle;
	float min,max;
	min = velocityMinMax.x;
	max = velocityMinMax.y;
	for ( size_t i = 0; i < volume; i++ )
	{
// 生成
		newParticle.reset(OBJParticle::Create(
			pos_,model_,{
				MathUtility::Randoms::GetRandFloat(min, max),
				MathUtility::Randoms::GetRandFloat(min, max),
				MathUtility::Randoms::GetRandFloat(min, max) },
				scale,color,changeScale
				));
		// 出力
		objParticles_.push_back(std::move(newParticle));
	}
}

void ObjParticleManager::SetAnyExp(ParticlePreset preset)
{
	std::unique_ptr<OBJParticle> newParticle;
	float min,max;
	min = preset.velocityMinMax.x;
	max = preset.velocityMinMax.y;
	for ( size_t i = 0; i < preset.volume; i++ )
	{
// 生成
		newParticle.reset(OBJParticle::Create(
			preset.pos_,model_,{
				MathUtility::Randoms::GetRandFloat(min, max),
				MathUtility::Randoms::GetRandFloat(min, max),
				MathUtility::Randoms::GetRandFloat(min, max) },
				preset.scale,preset.color
				));
		// 出力
		objParticles_.push_back(std::move(newParticle));
	}
}

void ObjParticleManager::SetCharge(const Vector3& pos,Vector2 velocityMinMax,size_t volume,float scale,Vector4 color,bool changeScale)
{
	std::unique_ptr<OBJParticle> newParticle;
	float min,max;
	min = velocityMinMax.x;
	max = velocityMinMax.y;
	for ( size_t i = 0; i < volume; i++ )
	{
		// 生成
		newParticle.reset(OBJParticle::Create(
			{
				pos.x + MathUtility::Randoms::GetRandFloat(min, max),
				pos.y + MathUtility::Randoms::GetRandFloat(min, max),
				pos.z + MathUtility::Randoms::GetRandFloat(min, max)
			},
			model_,
			pos,
			scale,
			color,
			changeScale));
		// 出力
		objParticles_.push_back(std::move(newParticle));

		//newParticle.reset(OBJParticle::Create(
		//	pos,model_,{
		//		MathUtility::Randoms::GetRandFloat(min, max),
		//		MathUtility::Randoms::GetRandFloat(min, max),
		//		MathUtility::Randoms::GetRandFloat(min, max) },
		//		scale,color
		//		));
		//// 出力
		//objParticles_.push_back(std::move(newParticle));
	}
}

void ObjParticleManager::SetCharge(ParticlePreset preset)
{
	std::unique_ptr<OBJParticle> newParticle;
	float min,max;
	min = preset.velocityMinMax.x;
	max = preset.velocityMinMax.y;
	for ( size_t i = 0; i < preset.volume; i++ )
	{
// 生成
		newParticle.reset(OBJParticle::Create(
			{
				MathUtility::Randoms::GetRandFloat(min, max),
				MathUtility::Randoms::GetRandFloat(min, max),
				MathUtility::Randoms::GetRandFloat(min, max)
			},
			model_,
			preset.pos_,
			preset.scale,
			preset.color
			));
		// 出力
		objParticles_.push_back(std::move(newParticle));
	}
}


ObjParticleManager* ObjParticleManager::GetInstance() {
	static ObjParticleManager instance;
	return &instance;
}
