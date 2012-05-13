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

SelectLevel SelectLevel::mSelectLevel;

void SelectLevel::init(Game* game)
{
	// Important to set the game for changeState() to work!
	setGame(game);

	mMenu = new Menu("SelectLevelMenu", NavigationType::MOUSE, HOR); 
	mMenu->setSize(600, 400, 256, 512);

	mLevelHandler = new LevelHandler();

	mLevelHandler->loadLevels();

	for(int i = 0; i < mLevelHandler->getNumLevels(); i++) {
		string name = mLevelHandler->getLevel(i)->getName();
		LevelItem* item = new LevelItem(name, "data/level.png", "data/level_hoover.png");
		item->waves = mLevelHandler->getLevel(i)->getNumWaves();
		item->completedWaves = mLevelHandler->getLevel(i)->getCompletedWaves();
		mMenu->addMenuItem(item);
	}

	mMenu->buildMenu(256, 256);
	mMenu->connect(&SelectLevel::menuMessage, this);

	ShowCursor(true);
}
	
void SelectLevel::cleanup()
{
	delete mMenu;
	delete mLevelHandler;
}

void SelectLevel::update(double dt)
{
	mMenu->update(gInput->mousePosition());
}
	
void SelectLevel::draw()
{
	mMenu->draw();

	char buffer[256];
	sprintf(buffer, "x: %f, y: %f", gInput->mousePosition().x, gInput->mousePosition().y);
	gGraphics->drawText(buffer, 40, 300);
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
	changeState(PlayState::Instance());
	PlayState::Instance()->setLevel(message);

	return false;
}

void SelectLevel::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{

}

LevelItem::LevelItem(string name, string standardTextureSource, string onSelectTextureSource)
	: MenuItem(name, standardTextureSource, onSelectTextureSource)
{
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
	sprintf(buffer, "completed: %i/%i", completedWaves, waves);
	gGraphics->drawText(buffer, rect.left + 120, rect.top + 200);
}
