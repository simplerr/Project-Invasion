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

SelectLevel SelectLevel::mSelectLevel;

void SelectLevel::init(Game* game)
{
	// Important to set the game for changeState() to work!
	setGame(game);

	mMenu = new Menu("SelectLevelMenu", NavigationType::MOUSE, HOR); 
	mMenu->setSize(600, 400, 256, 512);
	mMenu->addMenuItem(new LevelItem("level1", "data/level1.png", "data/level1_hoover.png"));
	mMenu->buildMenu(512, 512);
}
	
void SelectLevel::cleanup()
{
	delete mMenu;
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

void SelectLevel::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{

}

LevelItem::LevelItem(string name, string standardTextureSource, string onSelectTextureSource)
	: MenuItem(name, standardTextureSource, onSelectTextureSource)
{
	numLevel = 1;
	waves = 1;
	completedWaves = 1;
}
	
LevelItem::~LevelItem()
{

}

void LevelItem::draw()
{
	MenuItem::draw();

	char buffer[256];
	sprintf(buffer, "completed: %i/%i", completedWaves, waves);
	gGraphics->drawText(buffer, rect.left + 50, rect.top + 20);

	sprintf(buffer, "hej", numLevel);
	gGraphics->drawFont(buffer, rect.left + 100, rect.top + 100, 64, BLACK);
}
