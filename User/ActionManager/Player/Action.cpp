#include "Action.h"

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
