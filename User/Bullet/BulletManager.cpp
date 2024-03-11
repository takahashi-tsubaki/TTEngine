#include "BulletManager.h"

BulletManager* BulletManager::GetInstance() {
	static BulletManager instance;
	return &instance;
}

void BulletManager::Update()
{
	bullets_.remove_if([](std::unique_ptr<Bullet>& bullet) { return bullet->GetIsDead(); });
	for (std::unique_ptr<Bullet>& bullet : bullets_) {
		bullet->Update();
	};
}

void BulletManager::Draw()
{
	for (std::unique_ptr<Bullet>& bullet : bullets_) {
		bullet->Draw();
	};
}
