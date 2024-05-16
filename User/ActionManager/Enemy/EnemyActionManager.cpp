#include "EnemyAction.h"
#include "EnemyIdle.h"
#include "EnemyMove.h"
#include "EnemyShot.h"
#include "EnemyHomingShot.h"
#include "EnemyActionManager.h"

#include "ImguiManager.h"
	

EnemyActionManager::EnemyActionManager() { action_.reset(new EnemyIdle(&*this)); }

EnemyActionManager::~EnemyActionManager() {}

void EnemyActionManager::ActionInitialize(FbxObject3d* object, PlayerCharacter* player) {
	object_ = object;
	player_ = player;
	action_.get()->Initialize(object_,player_);

}

void EnemyActionManager::ActionUpdate()
{
	//敵がアクションを起こすためのタイマー
	if (action_->GetIsNowShot() == false)
	{
		moveDirectTimer--;
	}
	

	//2点間の距離
	DistanceTwoPoints();

	if ( distance_.z <= crossRange )
	{
		rangepattern = RangePattern::CROSSRange;
	}
	else if ( distance_.z > crossRange || distance_.z <= middleRange )
	{
		rangepattern = RangePattern::MIDDLERange;
	}
	else if (distance_.z > middleRange)
	{
		rangepattern = RangePattern::LONGRange;
	}

	if ( rangepattern == RangePattern::CROSSRange )
	{
		crossRangePattern();
	}
	else if (rangepattern == RangePattern::MIDDLERange)
	{
		middleRangePattern();
	}
	else if (rangepattern == RangePattern::LONGRange)
	{
		longRangePattern();
	}

	action_.get()->Update();
#ifdef _DEBUG
	ImGui::Begin("action");

	ImGui::SetWindowPos({600, 200});
	ImGui::SetWindowSize({400, 200});
	ImGui::InputInt("actionNum_", &actionNum_);
	ImGui::InputFloat("moveDirectTimer", &moveDirectTimer);
	ImGui::End();
#endif
}

void EnemyActionManager::ActionDraw() { action_.get()->Draw(); }

void EnemyActionManager::ChangeAction(EnemyAction* Action) {
	action_.reset(Action);
	ActionInitialize(object_,player_);
	action_.get()->Initialize(object_, player_);
}

int EnemyActionManager::generateRandomAction(int min, int max)
{
	std::mt19937 gen(random());
	std::uniform_int_distribution<> distrib(min, max);
	return distrib(gen);
}

void EnemyActionManager::DistanceTwoPoints() {
	distance_;
	distance_ = player_->GetFbxObject3d()->GetPosition() - object_->GetPosition();
	distance_.z = sqrtf(pow(distance_.x, 2.0f) + pow(distance_.z, 2.0f));
}

void EnemyActionManager::crossRangePattern() {
	if (moveDirectTimer < 0)
	{
		moveDirectTimer = ActionCoolTimer::MoveDirect;

		actionNum_ = generateRandomAction(1, 8);

			
		if (actionNum_ == 1)
		{
			ChangeAction(new EnemyIdle(&*this));

		}else if (actionNum_ >= 2 && actionNum_ < 4)
		{
			if (action_->GetIsNowShot() == false) {
				ChangeAction(new EnemyMove(&*this, actionNum_));
			}
		}
		else
		{
			if (action_->GetIsNowShot() == false) {
				action_->SetIsNowShot(true);
			}
			ChangeAction(new EnemyShot(&*this, actionNum_));
		}
	};
}

void EnemyActionManager::middleRangePattern()
{
	if (moveDirectTimer < 0)
	{
		moveDirectTimer = ActionCoolTimer::MoveDirect;

		actionNum_ = generateRandomAction(1, 9);

		if (actionNum_ == 1) {
			ChangeAction(new EnemyIdle(&*this));
		} 
		else if (actionNum_ >= 2 && actionNum_ < 4)
		{
			if (action_->GetIsNowShot() == false) {
				ChangeAction(new EnemyMove(&*this, actionNum_));
			}
		}
		else if ( actionNum_ >= 5 && actionNum_ <= 8 )//後ろ以外の方向への移動
		{
			if ( action_->GetIsNowShot() == false )
			{
				action_->SetIsNowShot(true);
			}
			ChangeAction(new EnemyHomingShot(&*this,actionNum_));
		}
		else
		{
			if (action_->GetIsNowShot() == false) {
				action_->SetIsNowShot(true);
			}
			ChangeAction(new EnemyShot(&*this, actionNum_));
		}
	};
}

void EnemyActionManager::longRangePattern()
{
	if (moveDirectTimer < 0)
	{
		moveDirectTimer = ActionCoolTimer::MoveDirect;

		actionNum_ = generateRandomAction(1, 10);

		if (actionNum_ == 1) {
			ChangeAction(new EnemyIdle(&*this));
		}
		else if (actionNum_ == 3 || actionNum_ == 4)//後ろ以外の方向への移動
		{
			if ( action_->GetIsNowShot() == false )
			{
				ChangeAction(new EnemyMove(&*this, actionNum_));

			}
		}
		else if ( actionNum_ == 7 || actionNum_ == 8 )//後ろ以外の方向への移動
		{
			if ( action_->GetIsNowShot() == false )
			{
				action_->SetIsNowShot(true);
			}
			ChangeAction(new EnemyHomingShot(&*this,actionNum_));
		}
		else
		{
			if (action_->GetIsNowShot() == false)
			{
				action_->SetIsNowShot(true);
			}
			ChangeAction(new EnemyShot(&*this, actionNum_));
		}
	};
}
