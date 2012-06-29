#pragma once
#include <vector>
#include "Skill.h"
using namespace std;

class Enemy;

class FrostNova : public Skill
{
public:
	FrostNova(World* world, Player* player);
	~FrostNova();

private:
	void update(float dt);
	void useAbility();

private:
	vector<Enemy*> mStunnedList;
};