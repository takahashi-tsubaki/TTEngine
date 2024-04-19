#include "Action.h"

std::unique_ptr<ParticleManager> Action::paritcle2_;

Action::Action()
{
	LoadSound();
}

Action::~Action() { delete audio_; }

void Action::LoadSound()
{
	//// 音声データの初期化と読み取り
	//audio_ = new TTEngine::Audio();
	//audio_->Initialize();

	//audio_->LoadWave("bullet.wav");
}

void Action::LoadTexture() {}

void Action::LoadParticle() {
	particle_ = std::make_unique<ParticleManager>();
	particle_->SetDrawBlendMode(1);
	particle_->Initialize();
	particle_->LoadTexture("sprite/particle.png");
	particle_->Update();
}
