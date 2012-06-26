#include "FrostNova.h"
#include "World.h"
#include "Player.h"

FrostNova::FrostNova(World* world, Player* player, SkillHandler* skillHandler)
	: Skill("FrostNova", world, player, skillHandler)
{

}
	
FrostNova::~FrostNova()
{

}

void FrostNova::update(float dt)
{
	increment(dt);
}
	
void FrostNova::useAbility()
{
	//mWorld->getObjectsInRadius(mStunnedList, mPlayer->getPosition(), 3000, ENEMY
}