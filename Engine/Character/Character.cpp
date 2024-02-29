#include "Character.h"

Character::Character()
{

}

void Character::moveAngle() {
	cameraAngle.y = atan2(
	    fbxObject_->GetCamera()->GetTarget().x - fbxObject_->GetCamera()->GetEye().x,
	    fbxObject_->GetCamera()->GetTarget().z - fbxObject_->GetCamera()->GetEye().z);
}

void Character::Distance(Vector3 start, Vector3 goal)
{
	distance = {goal.x - start.x, goal.y - start.y, goal.z - start.z};
}

void Character::DistanceTwoPoints(Vector3 start, Vector3 goal)
{
	Distance(start,goal);

	distance.z = sqrtf(pow(distance.x, 2.0f) + pow(distance.z, 2.0f));
}
