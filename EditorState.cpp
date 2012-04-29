#include <time.h>
#include "EditorState.h"
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

EditorState EditorState::mEditorState;

/*
if(mRotate) {
		mLight->setPosition(gCamera->getPosition() + D3DXVECTOR3(0, 5, 0));
		D3DXVECTOR3 t;
		mLight->setDirection(-*D3DXVec3Normalize(&t, &(gCamera->getPosition() - gCamera->getTarget())));
	}
*/

void EditorState::init(Game* game)
{
	// Create the world and add an ambient light to it.
	mWorld = new World();
	mWorld->addAmbientLight(D3DCOLOR_ARGB(255, 255, 255, 255));

	// Important to set the game for changeState() to work!
	setGame(game);

	// Set the graphics light list.
	gGraphics->setLightList(mWorld->getLights());

	// Setup the camera.
	gCamera->setPosition(D3DXVECTOR3(0, 2000, 0));
	gCamera->setTarget(D3DXVECTOR3(0, 0, 0));
}
	
void EditorState::cleanup()
{
	// Cleanup all the game objects.
	delete mWorld;
}

void EditorState::update(double dt)
{
	// Update the world and everything it contains.
	mWorld->update(dt);

	// Toggle camera type. [TODO] Remove!
	if(gInput->keyPressed(VK_F2)) 
		gCamera->setType(FPS);
	else if(gInput->keyPressed(VK_F3)) 
		gCamera->setType(NOCLIP);
}
	
void EditorState::draw()
{
	// Draw the world.
	mWorld->draw();
}

void EditorState::onLostDevice()
{
	mWorld->onLostDevice();
}

void EditorState::onResetDevice()
{
	mWorld->onLostDevice();
}

void EditorState::pause()
{

}

void EditorState::resume()
{

}

void EditorState::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{

}
