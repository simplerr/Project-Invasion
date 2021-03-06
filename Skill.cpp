#include "Skill.h"
#include "Player.h"
#include "World.h"
#include "Graphics.h"
#include "Game.h"

Skill::Skill(string name, World* world, Player* player)
{
	mDarkTexture = gGraphics->loadTexture("data\\imgs\\dark.png");

	mData = gSkillHandler->getData(name);
	mWorld = world;
	mPlayer = player;
	mCooldownDelta = mData.cooldown;
	loadTexture(mData.icon);
}
	
Skill::~Skill()
{

}

void Skill::increment(float dt)
{
	mCooldownDelta += dt;
}
	
void Skill::ability()
{
	if(mCooldownDelta >= mData.cooldown && mPlayer->getEnergy() >= mData.energy) {
		useAbility();
		mCooldownDelta = 0.0f;
		mPlayer->useEnergy(mData.energy);
	}
}

void Skill::draw(Rect rect)
{
	float cd = mData.cooldown - mCooldownDelta;
	// Resize the rect.
	rect.top += 5;
	rect.bottom -= 5;
	rect.left += 5;
	rect.right -= 5;

	// Not enough energy to use skill.
	if(mPlayer->getEnergy() < mData.energy)
		gGraphics->drawScreenTexture(mDarkTexture, rect);
	else if(cd > 0) {
		// Dark shade.
		gGraphics->drawScreenTexture(mDarkTexture, rect);
		
		// Cooldown number.
		char buffer[256];
		sprintf(buffer, "%.2f", cd);
		float size = 16;
		if(gGame->widthRatio() != 1.0f)
			size = 22;
		gGraphics->drawText(buffer, rect.left * gGame->widthRatio() + 15, rect.top * gGame->heightRatio() + 25, WHITE, size);
	}
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