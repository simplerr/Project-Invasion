#include "IronArmor.h"
#include "Player.h"
#include "Camera.h"

IronArmor::IronArmor(World* world, Player* player)
	: Skill("IronArmor", world, player)
{
	mDelta = -1;
}
	
IronArmor::~IronArmor()
{

}

void IronArmor::update(float dt)
{
	increment(dt);

	if(mDelta != -1) {
		mDelta += dt;

		if(mDelta >= mData.cooldown) {
			mPlayer->setArmor(0.0f);
			mDelta = -1;
		}
	}
}

void IronArmor::useAbility()
{
	mPlayer->setArmor(100.0f);
	mDelta = 0.0f;
}