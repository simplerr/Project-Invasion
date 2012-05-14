#include "Skill.h"
#include "Player.h"
#include "World.h"

Skill::Skill(string name, World* world, Player* player, SkillHandler* skillHandler)
{
	mData = skillHandler->getData(name);
	mWorld = world;
	mPlayer = player;
	mCooldownDelta = 0.0f;
}
	
Skill::~Skill()
{

}

void Skill::increment(float dt)
{
	mCooldownDelta += dt;
	update(dt);
}
	
void Skill::ability()
{
	if(mCooldownDelta >= mData.cooldown) {
		useAbility();
		mCooldownDelta = 0.0f;
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
	return mData.cooldown - mCooldownDelta;
}