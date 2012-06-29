#pragma once
#include "SkillHandler.h"
#include "SlotItem.h"

class World;
class Player;

class Skill : public SlotItem
{
public:
	Skill(string name, World* world, Player* player);
	virtual ~Skill();

	void draw(Rect rect);
	void increment(float dt);
	void ability();

	SkillData	getData();
	float		getCooldownLeft();

private:
	virtual void useAbility();

protected:
	IDirect3DTexture9* mDarkTexture;
	Player* mPlayer;
	World*	mWorld;
	SkillData mData;
	float mCooldownDelta;
};