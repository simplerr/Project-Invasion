#pragma once
#include "Skill.h"
#include "d3dUtil.h"

class Leap : public Skill
{
public:
	Leap(World* world, Player* player);
	~Leap();

private:
	void update(float dt);
	void useAbility();

private:
	float	mLeapedDist;
	D3DXVECTOR3 mStartPos;
};