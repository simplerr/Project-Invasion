#include "Skill.h"
#include "Player.h"
#include "World.h"

Skill::Skill(string name, World* world, Player* player, SkillHandler* skillHandler)
{
	mData = skillHandler->getData(name);
	mWorld = world;
	mPlayer = player;
	mDelta = 0.0f;
}
	
Skill::~Skill()
{

}

void Skill::increment(float dt)
{
	mDelta += dt;
	update(dt);
}
	
void Skill::ability()
{
	if(mDelta >= mData.cooldown) {
		useAbility();
		mDelta = 0.0f;
	}
}

void Skill::update(float dt)
{
	
}

void Skill::useAbility()
{

}

SkillData Skill::getData()
{
	return mData;
}

float Skill::getCooldownLeft()
{
	return mData.cooldown - mDelta;
}