#include "Gui.h"
#include "Bar.h"
#include "SkillBar.h"
#include "Player.h"
#include "SkillHandler.h"
#include "Leap.h"
#include "IronArmor.h"
#include "Graphics.h"

Gui::Gui(Player* player)
{
	mPlayer = player;

	// Skill bar.
	mSkillBar = new SkillBar(player, player->getWorld());
	mSkillBar->addSkill(new Leap(player->getWorld(), player));
	mSkillBar->addSkill(new IronArmor(player->getWorld(), player));

	// Health bar.
	mHealthBar = new Bar("data\\bar.bmp", "data\\heart_icon.png", 100);
	mHealthBar->setPosition(1075, 670);

	// Energy bar.
	mEneryBar = new Bar("data\\bar.bmp", "data\\energy_icon.png", 100);
	mEneryBar->setPosition(1075, 710);

	// Ammo bar.
	mAmmoBar = new Bar("data\\bar.bmp", "data\\ammo_icon.png", 100);
	mAmmoBar->setPosition(1075, 750);

	mUiBorder = gGraphics->loadTexture("data/ui_border.png");
}
	
Gui::~Gui()
{
	delete mSkillBar;
	delete mEneryBar;
	delete mHealthBar;
	delete mAmmoBar;
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
	gGraphics->drawScreenTexture(mUiBorder, 1058, 710, 285, 180);

	mSkillBar->draw();
	mHealthBar->draw();
	mAmmoBar->draw();
	mEneryBar->draw();

	gGraphics->drawScreenTexture(mUiBorder, 128, 672, 256, 256);
}

void Gui::setPlayer(Player* player)
{
	mPlayer = player;
}