#pragma once
#include "SkillHandler.h"

class World;
class Player;

class Skill
{
public:
	Skill(string name, World* world, Player* player, SkillHandler* skillHandler);
	virtual ~Skill();

	void increment(float dt);
	void ability();

	SkillData	getData();
	float		getCooldownLeft();

private:
	virtual void update(float dt);
	virtual void useAbility();

protected:
	SkillHandler* mSkillHandler;
	Player* mPlayer;
	World*	mWorld;
	SkillData mData;
	float mDelta;
};