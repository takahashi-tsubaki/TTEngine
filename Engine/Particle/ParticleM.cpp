#include "ParticleM.h"
#include "Particle.h"

ParticleM::ParticleM()
{
}

ParticleM::~ParticleM()
{
}

void ParticleM::LoadTexture(const std::string& fileName)
{
	particles1->LoadTexture(fileName);
}

void ParticleM::Init()
{

	particles1.reset(Particles::Create());
	//particles2->Initialize();

}

void ParticleM::Update()
{
	particles1->Update();
	//particles2->Update();
}

void ParticleM::Draw()
{
	particles1->Draw();
	//particles2->Draw();
}

void ParticleM::CallExp(const Vector3& pos)
{
	for ( int i = 0; i < 100; i++ )
	{
		const float md_vel = 1.0f;
		Vector3 vel{};
		//それぞれの移動先のベクトルをランダムに決定
		vel.x = ( float ) rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = ( float ) rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = ( float ) rand() / RAND_MAX * md_vel - md_vel / 2.0f;

		Vector3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -( float ) rand() / RAND_MAX * rnd_acc;

		particles1->Add(30,pos,vel,acc,2,0,0.0f);
	}
}

void ParticleM::CallSmallExp(const Vector3& pos)
{
	for ( int i = 0; i < 50; i++ )
	{
		//const float md_vel = 1.0f;
		Vector3 vel{};
		vel.z/* = ( float ) rand() / RAND_MAX * md_vel - md_vel / 2.0f*/;

		Vector3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -( float ) rand() / RAND_MAX * rnd_acc;

		particles1->Add(30,pos,vel,acc, 5, 0 ,0.0f);
	}
}

void ParticleM::SetDrawBlendMode(int SetBlendMode)
{
	particles1->SetDrawBlendMode(SetBlendMode);

}

void ParticleM::Charge(const Vector3& pos)
{
	for (int i = 0; i < 100; i++)
	{
		const float md_vel = 1.0f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;

		Vector3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		particles1->Add(30, pos, vel, acc, 2, 0, 0.0f);
	}
}

