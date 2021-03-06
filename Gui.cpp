#include "Gui.h"
#include "Bar.h"
#include "SkillBar.h"
#include "Player.h"
#include "SkillHandler.h"
#include "Leap.h"
#include "IronArmor.h"
#include "Graphics.h"
#include "Menu.h"
#include "PlayState.h"
#include "SelectLevel.h"
#include "MainMenu.h"

Gui::Gui(Player* player)
{
	mPlayer = player;

	// Skill bar.
	mSkillBar = new SkillBar(player, player->getWorld());
	mSkillBar->addSkill(new Leap(player->getWorld(), player));
	mSkillBar->addSkill(new IronArmor(player->getWorld(), player));

	// Health bar.
	mHealthBar = new Bar("data\\imgs\\bar.bmp", "data\\imgs\\heart_icon.png", 100);
	mHealthBar->setPosition(1075, 670);

	// Energy bar.
	mEneryBar = new Bar("data\\imgs\\bar.bmp", "data\\imgs\\energy_icon.png", 100);
	mEneryBar->setPosition(1075, 710);

	// Ammo bar.
	mAmmoBar = new Bar("data\\imgs\\bar.bmp", "data\\imgs\\ammo_icon.png", 100);
	mAmmoBar->setPosition(1075, 750);

	mUiBorder = gGraphics->loadTexture("data/imgs/ui_border.png");
	mGameOverTexture = gGraphics->loadTexture("data/imgs/game_over.png");
	mWhiteOverlay = gGraphics->loadTexture("data/imgs/white_transparent.png");
	mCrossHair = gGraphics->loadTexture("data/imgs/aim.png");

	// The pause menu.
	mMenu = new Menu("InGameMenu", NavigationType::MOUSE, VER); 
	mMenu->setSize(600, 400, 256, 512);
	mMenu->setVisible(false);
	//mMenu->setBkgdTexture("data/menu_border.png");

	MenuItem* menu = new MenuItem("MainMenu", "data/imgs/buttons/menu_standard.png", "data/imgs/buttons/menu_glow.png");
	MenuItem* restart = new MenuItem("Restart", "data/imgs/buttons/restart_standard.png", "data/imgs/buttons/restart_glow.png");

	mMenu->addMenuItem(menu);
	mMenu->addMenuItem(restart);

	mMenu->buildMenu(128, 80);
	mMenu->connect(&Gui::menuMessage, this);
}
	
Gui::~Gui()
{
	delete mSkillBar;
	delete mEneryBar;
	delete mHealthBar;
	delete mAmmoBar;
	delete mMenu;
}
	
void Gui::update(float dt)
{
	mSkillBar->update(dt);
	mHealthBar->setCurrent(mPlayer->getHealth());
	mAmmoBar->setCurrent(mPlayer->getAmmo());
	mEneryBar->setCurrent(mPlayer->getEnergy());

	mMenu->update(gInput->mousePosition());
}
	
void Gui::draw()
{
	// Right border.
	gGraphics->drawScreenTexture(mUiBorder, 1058, 710, 285, 180);
	
	// Bars.
	mSkillBar->draw();
	mHealthBar->draw();
	mAmmoBar->draw();
	mEneryBar->draw();

	// Map.
	gGraphics->drawScreenTexture(mUiBorder, 128, 672, 256, 256);
	gGraphics->drawScreenTexture(mMapTexture, 128, 672, 230, 230);

	// White overlay.
	if(mMenu->getVisible() && mPlayer->getHealth() > 0)
		gGraphics->drawScreenTexture(mWhiteOverlay, 600, 400, 1200, 800);
	else if(mPlayer->getHealth() > 0)
		gGraphics->drawScreenTexture(mCrossHair, 600, 400, 32, 32, false);

	// Don't draw UI when game over, add a red layer.
	if(PlayState::Instance()->getGameOver()) {
		gGraphics->drawScreenTexture(mGameOverTexture, 600, 400, 1200, 800);
		mMenu->draw();
		return;
	}

	mMenu->draw();
}

void Gui::setPlayer(Player* player)
{
	mPlayer = player;
}

bool Gui::menuMessage(string message)
{
	if(message == "MainMenu") {
		PlayState::Instance()->changeState(MainMenu::Instance());
		ShowCursor(false);
		return false;
	}
	else if(message == "Restart") {
		PlayState::Instance()->restartLevel();
		PlayState::Instance()->setPaused(false);
		mMenu->setVisible(false);
		ShowCursor(false);
	}

	return true;
}

void Gui::toggleMenu()
{
	mMenu->setVisible(!mMenu->getVisible());

	if(mMenu->getVisible()) 
		gCamera->setPosition(mPlayer->getPosition() + D3DXVECTOR3(0, 1000, 0));

	mPlayer->toggleWeapon();
}

bool Gui::isMenuVisible()
{
	return mMenu->getVisible();
}

void Gui::setMapTexture(IDirect3DTexture9* texture)
{
	mMapTexture = texture;
}