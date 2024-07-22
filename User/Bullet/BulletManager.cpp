#include "BulletManager.h"

BulletManager* BulletManager::GetInstance() {
	static BulletManager instance;
	return &instance;
}

void BulletManager::Update()
{
	bombBullets_.remove_if([ ] (std::unique_ptr<BombBullet>& bombBullet){ return bombBullet->GetIsDead();});
	for ( std::unique_ptr<BombBullet>& bombBullet : bombBullets_ )
	{
		bombBullet->Update();
	}

	bullets_.remove_if([](std::unique_ptr<Bullet>& bullet) { return bullet->GetIsDead(); });
	for (std::unique_ptr<Bullet>& bullet : bullets_) {
		bullet->Update();
	};

	homingBullets_.remove_if([ ] (std::unique_ptr<HomingBullet>& bullet){return bullet->GetIsDead();});
	for (std::unique_ptr<HomingBullet>& homingBullet : homingBullets_) {
		homingBullet->Update();
	};

	lasers_.remove_if([ ] (std::unique_ptr<Laser>& laser){return laser->GetIsDead();});
	for ( std::unique_ptr<Laser>& laser : lasers_ )
	{
		laser->Update();
	};

}

void BulletManager::Draw()
{

	for ( std::unique_ptr<BombBullet>& bombBullet : bombBullets_ )
	{
		bombBullet->Draw();
	}

	for (std::unique_ptr<Bullet>& bullet : bullets_) {
		bullet->Draw();
	};

	for (std::unique_ptr<HomingBullet>& homingBullet : homingBullets_) {
		homingBullet->Draw();
	};

	for ( std::unique_ptr<Laser>& laser : lasers_ )
	{
		laser->Draw();
	};

}

void BulletManager::ParticleDraw(ID3D12GraphicsCommandList* cmdList) {
	//for (std::unique_ptr<Bullet>& bullet : bullets_) {
	//	bullet->GetParticle()->Draw(cmdList);
	//};
	if (cmdList) {
	}
}
