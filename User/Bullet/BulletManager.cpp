#include "BulletManager.h"

BulletManager* BulletManager::GetInstance() {
	static BulletManager instance;
	return &instance;
}

void BulletManager::Update()
{
	bullets_.remove_if([](std::unique_ptr<Bullet>& bullet) { return bullet->GetIsDead(); });
	homingBullets_.remove_if([](std::unique_ptr<HomingBullet>& bullet) { return bullet->GetIsDead(); });

	for (std::unique_ptr<Bullet>& bullet : bullets_) {
		bullet->Update();
		//bullet->GetParticle()->Update();
	};

	for (std::unique_ptr<HomingBullet>& homingBullet : homingBullets_) {
		homingBullet->Update();
		// bullet->GetParticle()->Update();
	};

}

void BulletManager::Draw()
{
	for (std::unique_ptr<Bullet>& bullet : bullets_) {
		bullet->Draw();
	};

	for (std::unique_ptr<HomingBullet>& homingBullet : homingBullets_) {
		homingBullet->Draw();
		// bullet->GetParticle()->Update();
	};

}

void BulletManager::ParticleDraw(ID3D12GraphicsCommandList* cmdList) {
	//for (std::unique_ptr<Bullet>& bullet : bullets_) {
	//	bullet->GetParticle()->Draw(cmdList);
	//};
	if (cmdList) {
	}
}
