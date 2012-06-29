#pragma once

#include "d3dUtil.h"
#include "Container.h"
class Player;
class Skill;
class SkillHandler;
class World;

class SkillBar : public Container
{
public:
	SkillBar(Player* player, World* world);
	~SkillBar();

	void update(float dt);
	void draw();
	void addSkill(Skill* skill);
private:
	Player*				mPlayer;
};