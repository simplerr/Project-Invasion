#include "Leap.h"
#include "Player.h"
#include "Camera.h"

Leap::Leap(World* world, Player* player, SkillHandler* skillHandler)
	: Skill("Leap", world, player, skillHandler)
{
	mLeapedDist = -1;
}
	
Leap::~Leap()
{

}

void Leap::update(float dt)
{
	if(mLeapedDist != -1)
	{
		D3DXVECTOR3 diff = mPlayer->getPosition() - mStartPos;
		mLeapedDist = sqrt(diff.x * diff.x + diff.z * diff.z);
		if(mLeapedDist >= mData.range) {
			mPlayer->setFriction(1.0f);
			mLeapedDist = -1;
		}
	}
}

void Leap::useAbility()
{
	D3DXVECTOR3 direction = gCamera->getDirection();
	D3DXVec3Normalize(&direction, &direction);
	direction.y = 0.0f;
	mPlayer->setVelocity(direction * 35.0f);
	mPlayer->setFriction(0.0f);
	mLeapedDist = 0.0f;
	mStartPos = mPlayer->getPosition();
}