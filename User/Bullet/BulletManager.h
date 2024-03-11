#pragma once

#include "Bullet.h"

#pragma warning(push)
#pragma warning(disable : 4819)
#pragma warning(disable : 4828)
#pragma warning(disable : 4820)
#include <forward_list>
// #include <vector>
#include <memory>

#pragma warning(pop)


class BulletManager {
public:
	BulletManager() = default;
	~BulletManager() { ClearBullet(); }

	BulletManager(const BulletManager&) = delete;
	BulletManager& operator=(const BulletManager&) = delete;

public:
	static BulletManager* GetInstance();

public:

	inline void AddBullet(std::unique_ptr<Bullet> bullet_)
	{
		bullets_.push_back(std::move(bullet_));
	}

	void ClearBullet() { bullets_.clear(); }


	void Update();

	void Draw();

private:

	// プレイヤーの弾モデル関連

	std::list<std::unique_ptr<Bullet>> bullets_;

};
