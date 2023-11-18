#pragma once
#include "StatePattern.h"
class StandBy : public StatePattern
{
protected:
	StateManager controller_;
	Input* input_ = nullptr;
};
