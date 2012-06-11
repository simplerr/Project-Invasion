#include "Gui.h"
#include "Bar.h"
#include "Player.h"

Gui::Gui(Player* player)
{
	mPlayer = player;

	// Health bar.
	mHealthBar = new Bar("data\\bar.bmp", "data\\heart_icon.png", 100);
	mHealthBar->setPosition(1075, 650);

	// Ammo bar.
	mAmmoBar = new Bar("data\\bar.bmp", "data\\ammo_icon.png", 100);
	mAmmoBar->setPosition(1075, 700);
}
	
Gui::~Gui()
{

}
	
void Gui::update(float dt)
{
	mHealthBar->setCurrent(mPlayer->getHealth());
	mAmmoBar->setCurrent(mPlayer->getAmmo());
}
	
void Gui::draw()
{
	mHealthBar->draw();
	mAmmoBar->draw();
}

void Gui::setPlayer(Player* player)
{
	mPlayer = player;
}