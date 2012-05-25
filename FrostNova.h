#pragma once
#include <vector>
#include "Skill.h"
using namespace std;

class Enemy;

class FrostNova : public Skill
{
public:
	FrostNova(World* world, Player* player, SkillHandler* skillHandler);
	~FrostNova();

private:
	void update(float dt);
	void useAbility();

private:
	vector<Enemy*> mStunnedList;
};