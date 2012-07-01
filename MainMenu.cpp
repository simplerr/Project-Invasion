#include <time.h>
#include "MainMenu.h"
#include "World.h"
#include "Light.h"
#include "Player.h"
#include "SelectLevel.h"
#include "PlayState.h"

MainMenu MainMenu::mMainMenu;

void MainMenu::init(Game* game)
{
	// Important to set the game for changeState() to work!
	setGame(game);

	// Create the menu.
	mMenu = new Menu("MainMenu", NavigationType::MOUSE, HOR); 
	mMenu->setSize(1000, 700, 256, 512);
	mMenu->addMenuItem(new MenuItem("SelectLevel", "data/menu_standard.png", "data/menu_glow.png"));
	mMenu->addMenuItem(new MenuItem("Credits", "data/restart_standard.png", "data/restart_glow.png"));
	mMenu->buildMenu(128, 80);
	mMenu->connect(&MainMenu::menuMessage, this);

	// Create the world.
	mWorld = new World();
	mLight = new Light(D3DXVECTOR3(0, 25, 0), D3DXVECTOR3(0, -1, 0), Material(WHITE, 0), 32, 1600.0f);
	mWorld->addLight(mLight); 
	mWorld->addAmbientLight(D3DCOLOR_ARGB(200, 100, 100, 100));

	// Set the graphics light list.
	gGraphics->setLightList(mWorld->getLights());

	// Setup the camera.
	gCamera->setPosition(D3DXVECTOR3(0, 5000, 0));
	gCamera->setDirection(D3DXVECTOR3(0, -0.6, 0.4));

	ShowCursor(true);
}
	
void MainMenu::cleanup()
{
	delete mMenu;
	delete mWorld;
}

void MainMenu::update(double dt)
{
	mMenu->update(gInput->mousePosition());
	gCamera->rotate(0, 0.001);
	//gCamera->rotate();
	gCamera->updateView();

	// Spot light in the looking direction.
	mLight->setPosition(gCamera->getPosition() + D3DXVECTOR3(0, 5, 0));
	D3DXVECTOR3 t;
	mLight->setDirection(-*D3DXVec3Normalize(&t, &(gCamera->getPosition() - gCamera->getTarget())));
}
	
void MainMenu::draw()
{
	mWorld->draw();
	mMenu->draw();
}

void MainMenu::onLostDevice()
{

}

void MainMenu::onResetDevice()
{

}

void MainMenu::pause()
{

}

void MainMenu::resume()
{

}

bool MainMenu::menuMessage(string message)
{
	if(message == "SelectLevel") 
		changeState(SelectLevel::Instance());
	else if(message == "Credits") {
		changeState(PlayState::Instance());
		PlayState::Instance()->setLevel("1");
	}
	else
		return true;

	return false;
}

void MainMenu::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{

}