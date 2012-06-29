#pragma once
#include "Skill.h"
#include "d3dUtil.h"

class IronArmor : public Skill
{
public:
	IronArmor(World* world, Player* player);
	~IronArmor();

private:
	void update(float dt);
	void useAbility();

private:
	float mDelta;
};