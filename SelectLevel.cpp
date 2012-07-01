#include <time.h>
#include "SelectLevel.h"
#include "d3dUtil.h"
#include "World.h"
#include "Player.h"
#include "Object3D.h"
#include "Mesh.h"
#include "SkinnedMesh.h"
#include "Light.h"
#include "EnemyHandler.h"
#include "Enemy.h"
#include "Input.h"
#include "Camera.h"
#include "PlayState.h"
#include "MainMenu.h"

SelectLevel SelectLevel::mSelectLevel;

void SelectLevel::init(Game* game)
{
	// Important to set the game for changeState() to work!
	setGame(game);

	mMenu = new Menu("SelectLevelMenu", NavigationType::MOUSE, HOR); 
	mMenu->setSize(600, 400, 256, 512);

	// [NOTE] It's actually World that should deletes the spawners, but since theres no World in the menu we have to do it manually.
	// Very poorly structured...
	gLevelHandler->deleteSpawners();
	gLevelHandler->loadLevels();

	for(int i = 0; i < gLevelHandler->getNumLevels(); i++) {
		string name = gLevelHandler->getLevel(i)->getName();
		LevelItem* item = new LevelItem(name, "data/level_standard_text.png", "data/level_glow_text.png", "data/level_invalid_text.png");
		item->waves = gLevelHandler->getLevel(i)->getNumWaves();
		item->completedWaves = gLevelHandler->getLevel(i)->getCompletedWaves();

		// Not the first level.
		if(i != 0) {
			Level* lastLevel = gLevelHandler->getLevel(i-1);
			if(lastLevel->getCompletedWaves() < lastLevel->getNumWaves())
				item->state = INACTIVE;
		}

		mMenu->addMenuItem(item);
	}

	mMenu->buildMenu(128*1.2, 70*1.2);
	mMenu->connect(&SelectLevel::menuMessage, this);

	// Side menu.
	mSideMenu = new Menu("SideMenu", NavigationType::MOUSE, HOR); 
	mSideMenu->setSize(1000, 700, 256, 512);
	MenuItem* item = new MenuItem("MainMenu", "data/menu_standard.png", "data/menu_glow.png");
	mSideMenu->addMenuItem(item);
	mSideMenu->buildMenu(128, 80);
	mSideMenu->connect(&SelectLevel::menuMessage, this);

	// Create the world.
	mWorld = new World();
	mLight = new Light(D3DXVECTOR3(0, 25, 0), D3DXVECTOR3(0, -1, 0), Material(WHITE, 0), 32, 1600.0f);
	mWorld->addLight(mLight); 
	mWorld->addAmbientLight(D3DCOLOR_ARGB(200, 100, 100, 100));

	// Set the graphics light list.
	gGraphics->setLightList(mWorld->getLights());

	mLogo = gGraphics->loadTexture("data/logo.png");

	// Setup the camera.
	gCamera->setPosition(D3DXVECTOR3(0, 5000, 0));
	gCamera->setDirection(D3DXVECTOR3(0, -0.6, 0.4));
}
	
void SelectLevel::cleanup()
{
	delete mMenu;
	delete mSideMenu;
	delete mWorld;
	gLevelHandler->deleteSpawners();
}

void SelectLevel::update(double dt)
{
	// Spot light in the looking direction.
	mLight->setPosition(gCamera->getPosition() + D3DXVECTOR3(0, 5, 0));
	D3DXVECTOR3 t;
	mLight->setDirection(-*D3DXVec3Normalize(&t, &(gCamera->getPosition() - gCamera->getTarget())));

	// Only update the other menu if the state havent changed.
	if(mMenu->update(gInput->mousePosition())) 
		mSideMenu->update(gInput->mousePosition());

	gCamera->rotate(0, 0.001);
	gCamera->updateView();
}
	
void SelectLevel::draw()
{
	mWorld->draw();
	gGraphics->drawScreenTexture(mLogo, 600, 125, 610*1.2, 93*1.2);
	mMenu->draw();
	mSideMenu->draw();
}

void SelectLevel::onLostDevice()
{

}

void SelectLevel::onResetDevice()
{

}

void SelectLevel::pause()
{

}

void SelectLevel::resume()
{

}

bool SelectLevel::menuMessage(string message)
{
	if(message == "MainMenu")
		changeState(MainMenu::Instance());
	else {
		changeState(PlayState::Instance());
		PlayState::Instance()->setLevel(message);
	}

	return false;
}

void SelectLevel::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{

}

LevelItem::LevelItem(string name, string standardTextureSource, string onSelectTextureSource, string inactiveTextureSource)
	: MenuItem(name, standardTextureSource, onSelectTextureSource, inactiveTextureSource)
{
	checkMark = gGraphics->loadTexture("data/checkmark.png");

	numLevel = 1;
	waves = 1;
	completedWaves = 0;
}
	
LevelItem::~LevelItem()
{

}

void LevelItem::draw()
{
	MenuItem::draw();

	char buffer[256];
	sprintf(buffer, "LEVEL %s", itemName.c_str());
	gGraphics->drawText(buffer, rect.left + 5, rect.top , WHITE, 30);

	if(this->state == INACTIVE)
		gGraphics->drawScreenTexture(inactiveTexture, rect);

	if(completedWaves == waves)
		gGraphics->drawScreenTexture(checkMark, rect.left + rect.getWidth()/2, rect.top + 60, 26, 26);
}
