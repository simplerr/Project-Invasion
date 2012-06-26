#include "Graphics.h"
#include "SkillBar.h"
#include "Player.h"
#include "Input.h"
#include "Skill.h"
#include "Leap.h"
#include "World.h"
#include "SkillHandler.h"

SkillBar::SkillBar(Player* player, World* world, SkillHandler* skillHandler) : Container(400, 750, 600, 90)
{
	// Set the player
	mPlayer = player;

	// Add the slots.
	for(int i = 0; i < 6; i++) 
		addSlot(getPosition().x + i*50, getPosition().y, SKILL);

	// Set visible
	show();
}

SkillBar::~SkillBar()
{

}

void SkillBar::update(float dt)
{
	// Standard update
	Container::update(dt);

	// Check if a slot with a skill in it was pressed
	// Keys 1-6
	// TODO: Check for right press with the mouse
	// TODO: Check for right press with the mouse, e and q
	for(int i = 0; i < mSlotList.size(); i++) {
		if(gInput->keyPressed(49 + i) && mSlotList[i].taken)
			((Skill*)mSlotList[i].item)->ability();
	}
}

void SkillBar::draw()
{
	// Standard draw
	Container::draw();
}

void SkillBar::addSkill(Skill* skill)
{
	// Adds a new skill to the first free slot
	for(int i = 0; i < mSlotList.size(); i++) 
	{
		if(!mSlotList[i].taken) {
			mSlotList[i].item = skill;
			mSlotList[i].taken = true;
			break;
		}
	}
}