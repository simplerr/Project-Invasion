#include <time.h>
#include "MainMenu.h"
#include "World.h"
#include "Light.h"
#include "Player.h"
#include "SelectLevel.h"
#include "PlayState.h"
#include "Sound.h"

MainMenu MainMenu::mMainMenu;

void MainMenu::init(Game* game)
{
	// Important to set the game for changeState() to work!
	setGame(game);

	// Create the menu.
	mMenu = new Menu("MainMenu", NavigationType::MOUSE, HOR); 
	mMenu->setSize(880, 700, 256, 512);
	mMenu->addMenuItem(new MenuItem("SelectLevel", "data/buttons/play_standard.png", "data/buttons/play_glow.png"));
	mMenu->addMenuItem(new MenuItem("About", "data/buttons/about_standard.png", "data/buttons/about_glow.png"));
	mMenu->addMenuItem(new MenuItem("Quit", "data/buttons/quit_standard.png", "data/buttons/quit_glow.png"));
	mMenu->buildMenu(116*1.5, 50*1.5);
	mMenu->connect(&MainMenu::menuMessage, this);

	// Side menu.
	mAboutMenu = new Menu("SideMenu", NavigationType::MOUSE, HOR); 
	mAboutMenu->setSize(1000, 700, 256, 512);
	MenuItem* item = new MenuItem("Back", "data/buttons/back_standard.png", "data/buttons/back_glow.png");
	mAboutMenu->addMenuItem(item);
	mAboutMenu->buildMenu(116*1.5, 50*1.5);
	mAboutMenu->connect(&MainMenu::menuMessage, this);

	// Create the world.
	mWorld = new World();
	mLight = new Light(D3DXVECTOR3(0, 25, 0), D3DXVECTOR3(0, -1, 0), Material(WHITE, 0), 32, 1600.0f);
	mWorld->addLight(mLight); 
	mWorld->addAmbientLight(D3DCOLOR_ARGB(200, 100, 100, 100));

	mLogo = gGraphics->loadTexture("data/logo.png");
	mAboutTexture = gGraphics->loadTexture("data/about.png");

	// Set the graphics light list.
	gGraphics->setLightList(mWorld->getLights());

	// Setup the camera.
	gCamera->setPosition(D3DXVECTOR3(0, 5000, 0));
	gCamera->setDirection(D3DXVECTOR3(0, -0.6, 0.4));

	ShowCursor(true);

	gSound->muteMusic(false);
	gSound->playMusic("data/sound/menu_loop.wav", true, 0);
	gSound->setVolume(0.15f);

	mShowingAbout = false;
}
	
void MainMenu::cleanup()
{
	delete mMenu;
	delete mWorld;
}

void MainMenu::update(double dt)
{
	if(gInput->keyPressed(VK_ESCAPE))
		mShowingAbout = false;

	if(!mShowingAbout)
		mMenu->update(gInput->mousePosition());
	else
		mAboutMenu->update(gInput->mousePosition());

	gCamera->rotate(0, 0.001);
	gCamera->updateView();

	// Spot light in the looking direction.
	mLight->setPosition(gCamera->getPosition() + D3DXVECTOR3(0, 5, 0));
	D3DXVECTOR3 t;
	mLight->setDirection(-*D3DXVec3Normalize(&t, &(gCamera->getPosition() - gCamera->getTarget())));
}
	
void MainMenu::draw()
{
	mWorld->draw();
	gGraphics->drawScreenTexture(mLogo, 600, 125, 610*1.2, 93*1.2);

	if(!mShowingAbout)
		mMenu->draw();
	else {
		mAboutMenu->draw();
		gGraphics->drawScreenTexture(mAboutTexture, 600, 400, 356, 356);
	}
}

void MainMenu::onLostDevice()
{
	mWorld->onLostDevice();
}

void MainMenu::onResetDevice()
{
	mWorld->onResetDevice();
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
	else if(message == "Quit") 
		SendMessage(gGame->getMainWnd(), WM_CLOSE, 0, 0);
	else if(message == "About") 
		mShowingAbout = true;
	else if(message == "Back")
		mShowingAbout = false;
	else
		return true;

	return false;
}

void MainMenu::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{

}