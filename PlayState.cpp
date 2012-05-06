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
	mPlayer->init();

	// Temp test mesh.
	mCastle = new Mesh("data/models/castle/castle.x", D3DXVECTOR3(900.0f, 100.0f, 0.0f), PROP);
	mCastle->setScale(5.0f);
	mWorld->addObject(mCastle);

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
	mLevelHandler = new LevelHandler();
	mLevelHandler->loadLevels();

	mActiveLevel = mLevelHandler->getLevel(0);
	mActiveLevel->init(mWorld, mPlayer);
}
	
void PlayState::cleanup()
{
	// Cleanup all the game objects.
	delete mWorld;
}

void PlayState::update(double dt)
{
	// Update the world and everything it contains.
	mWorld->update(dt);
	mActiveLevel->update(dt);

	if(mActiveLevel->completedWave()) {
		// Change state etc...
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
	gGraphics->drawScreenTexture(mRenderTarget->getTexture(), 128, 672, 256, 256);
	gGraphics->drawScreenTexture(mTexture, gGame->getScreenWidth()/2, gGame->getScreenHeight()/2, 32, 32);
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

Wave* PlayState::getCurrentWave()
{
	return mActiveLevel->getCurrentWave();
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
