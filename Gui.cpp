#include "Gui.h"
#include "Bar.h"
#include "SkillBar.h"
#include "Player.h"
#include "SkillHandler.h"
#include "Leap.h"
#include "IronArmor.h"

Gui::Gui(Player* player)
{
	mPlayer = player;
	mSkillHandler = new SkillHandler();

	// Skill bar.
	mSkillBar = new SkillBar(player, player->getWorld(), mSkillHandler);
	mSkillBar->addSkill(new Leap(player->getWorld(), player, mSkillHandler));
	mSkillBar->addSkill(new IronArmor(player->getWorld(), player, mSkillHandler));

	// Health bar.
	mHealthBar = new Bar("data\\bar.bmp", "data\\heart_icon.png", 100);
	mHealthBar->setPosition(1075, 680);

	// Energy bar.
	mEneryBar = new Bar("data\\bar.bmp", "data\\energy_icon.png", 100);
	mEneryBar->setPosition(1075, 720);

	// Ammo bar.
	mAmmoBar = new Bar("data\\bar.bmp", "data\\ammo_icon.png", 100);
	mAmmoBar->setPosition(1075, 760);
}
	
Gui::~Gui()
{
	delete mSkillBar;
	delete mEneryBar;
	delete mSkillHandler;
}
	
void Gui::update(float dt)
{
	mSkillBar->update(dt);
	mHealthBar->setCurrent(mPlayer->getHealth());
	mAmmoBar->setCurrent(mPlayer->getAmmo());
	mEneryBar->setCurrent(mPlayer->getEnergy());
}
	
void Gui::draw()
{
	mSkillBar->draw();
	mHealthBar->draw();
	mAmmoBar->draw();
	mEneryBar->draw();
}

void Gui::setPlayer(Player* player)
{
	mPlayer = player;
}