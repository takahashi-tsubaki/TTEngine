#pragma once
class EnemyState
{
public:

	EnemyState();
	~EnemyState();
	virtual void Update(EnemyState* action) = 0;



private:
	EnemyState* action_ = nullptr ;


};

