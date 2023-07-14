#include "EnemyAttack.h"

void EnemyAttack::Update(EnemyState* action,Player* player,Enemy* enemy)
{
	enemyPos = enemy->GetPosition();
	playerPos = player->GetPosition();

	delete action;
	SetPostion(enemyPos);
}
