#include <time.h>
#include "PlayState.h"
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
#include "Wave.h"
#include "RenderTarget.h"
#include "LevelHandler.h"
#include "Level.h"
#include "SelectLevel.h"
#include "Gui.h"

PlayState PlayState::mPlayState;

/*
if(mRotate) {
		mLight->setPosition(gCamera->getPosition() + D3DXVECTOR3(0, 5, 0));
		D3DXVECTOR3 t;
		mLight->setDirection(-*D3DXVec3Normalize(&t, &(gCamera->getPosition() - gCamera->getTarget())));
	}
*/

void PlayState::init(Game* game)
{
	// Create the world and add an ambient light to it.
	mWorld = new World();
	mLight = new Light(D3DXVECTOR3(0, 25, 0), D3DXVECTOR3(0, -1, 0), Material(WHITE, 8.0f), 256, 16.0f);
	mWorld->addLight(mLight); 
	mWorld->addAmbientLight(D3DCOLOR_ARGB(255, 110, 110, 110));

	// Create a player and add it to the level.
	mPlayer = new Player(D3DXVECTOR3(0, 2000, 5000));
	mWorld->addObject(mPlayer);

	// Load the crosshair texture.
	mTexture = gGraphics->loadTexture("data/aim.png");

	// Hide the cursor.
	ShowCursor(false);

	// Seed the rand() function.
	srand(time(0));

	// Important to set the game for changeState() to work!
	setGame(game);	

	// Set the graphics light list.
	gGraphics->setLightList(mWorld->getLights());

	mRenderTarget = new RenderTarget(256, 256);
	gLevelHandler->loadLevels();

	mGui = new Gui(mPlayer);

	mPaused = false;
}
	
void PlayState::cleanup()
{
	// Cleanup all the game objects.
	delete mWorld;
	delete mGui;
	delete mRenderTarget;
	gLevelHandler->deleteSpawners();
	gLevelHandler->saveProgress();
}

void PlayState::update(double dt)
{
	// Toggle the menu.
	if(gInput->keyPressed(VK_ESCAPE)) {
		mGui->toggleMenu();
		mPaused = !mPaused;

		if(mPaused) 
			ShowCursor(true);
		else
			ShowCursor(false);
	}

	// Update the Gui.
	mGui->update(dt);

	// Don't update anything if paused.
	if(mPaused)
		return;

	// Update the world and everything it contains.
	mWorld->update(dt);
	mActiveLevel->update(dt);

	if(mActiveLevel->getState() == LEVEL_COMPLETED) {
		if(gInput->keyPressed(VK_RETURN)) {
			string name = mActiveLevel->getName();
			int levelNum = atoi(name.c_str());
			if(levelNum < gLevelHandler->getNumLevels()) {
				char buffer[10];
				setLevel(itoa(levelNum+1, buffer, 10));
			}
			else {
				ShowCursor(true);
				changeState(SelectLevel::Instance());
			}
		}
	}

	// Spot light in the looking direction.
	mLight->setPosition(gCamera->getPosition() + D3DXVECTOR3(0, 5, 0));
	D3DXVECTOR3 t;
	mLight->setDirection(-*D3DXVec3Normalize(&t, &(gCamera->getPosition() - gCamera->getTarget())));

	// Toggle camera type. [TODO] Remove!
	if(gInput->keyPressed(VK_F2)) 
		gCamera->setType(FPS);
	else if(gInput->keyPressed(VK_F3)) 
		gCamera->setType(NOCLIP);

	// Limit the cursor movement (only inside the window).
	limitCursor();
}
	
void PlayState::draw()
{
	// Draw the world.
	mWorld->draw();
	mActiveLevel->draw();
	mWorld->drawToMinimap(mRenderTarget);

	// Draw the crosshair.
	gGraphics->drawScreenTexture(mTexture, gGame->getScreenWidth()/2, gGame->getScreenHeight()/2, 32, 32, false);

	// Draw the Gui.
	mGui->draw();

	// Minimap.
	gGraphics->drawScreenTexture(mRenderTarget->getTexture(), 128, 672, 230, 230);
}

void PlayState::onLostDevice()
{
	// Pass on to the world.
	mWorld->onLostDevice();
	mRenderTarget->onLostDevice();
}

void PlayState::onResetDevice()
{
	// Pass on to the world.
	mWorld->onLostDevice();
	mRenderTarget->onResetDevice();
}

void PlayState::limitCursor()
{
	// Limits the mouse position to within the window.
	if(GetFocus() == gGame->getMainWnd()) {
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		ScreenToClient(gGame->getMainWnd(), &cursorPos);

		if(cursorPos.x < 0) 
			cursorPos.x = 0;
		else if(cursorPos.x > gGame->getScreenWidth()-15)
			cursorPos.x = gGame->getScreenWidth()-15;
		if(cursorPos.y < 0)
			cursorPos.y = 0;
		else if(cursorPos.y > gGame->getScreenHeight()-40)
			cursorPos.y = gGame->getScreenHeight()-40;

		ClientToScreen(gGame->getMainWnd(), &cursorPos);
		SetCursorPos(cursorPos.x, cursorPos.y);
	}
}

void PlayState::setLevel(string name)
{
	// Get the new level, reset and init the new one.
	gLevelHandler->loadLevels();
	mActiveLevel = gLevelHandler->getLevel(name);
	mWorld->reset();
	mActiveLevel->init(mWorld, mPlayer);

	// Randomize player location.
	mPlayer->setPosition(D3DXVECTOR3((rand() % 2500) - 2500, 5000, (rand() % 2500) - 2500));
	mPlayer->reset();
}

void PlayState::restartLevel()
{
	setLevel(mActiveLevel->getName());
}

Wave* PlayState::getCurrentWave()
{
	return mActiveLevel->getCurrentWave();
}

void PlayState::setPaused(bool paused)
{
	mPaused = paused;
}

void PlayState::pause()
{

}

void PlayState::resume()
{

}

void PlayState::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{

}

