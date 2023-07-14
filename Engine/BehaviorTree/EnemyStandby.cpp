#include "EnemyStandby.h"

void EnemyStandby::Update(EnemyState* action, Player* player, Enemy* enemy)
{
	playerPos = player->GetPosition();
	enemyPos = enemy->GetPosition();

	if (action != nullptr)
	{
		delete action_;
	}
	action_ = action;

	SetPostion(enemyPos);
}
