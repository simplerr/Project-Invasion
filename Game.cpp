//#include <crtdbg.h>
#include <time.h>
#include <fstream>
#include <time.h>
#include <string>
#include "Game.h"
#include "Input.h"
#include "Vertex.h"
#include "World.h"
#include "Light.h"
#include "Object3D.h"
#include "Player.h"
#include "Mesh.h"
#include "Terrain.h"
#include "Enemy.h"

// Set the globals
Runnable*			gGame		= 0;
IDirect3DDevice9*	gd3dDevice	= 0;
Input*				gInput		= 0;
Graphics*			gGraphics	= 0;
Camera*				gCamera		= 0;

D3DXVECTOR3 normal;

// Use Windows 7 looking controls.
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//! The program starts here.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	Game game(hInstance, "DirectX_3D-1", 1200, 800, D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gGame = &game;

	gInput = new Input();
	gGame->init();

	// Run the game
	return gGame->run();
}

//! Constructor.
Game::Game(HINSTANCE hInstance, string caption, int width, int height, D3DDEVTYPE devType, DWORD requestedVP)
	:Runnable(hInstance, caption, width, height, devType, requestedVP)
{
	InitAllVertexDeclarations();
	
	gGraphics = new Graphics();
	gCamera = new Camera();
	mGfxStats = new GfxStats();
	
	mRotate = true;

	gCamera->setPosition(D3DXVECTOR3(0, 686, 1189));
	//gCamera->setPosition(D3DXVECTOR3(1238, 686, -1189));
	gCamera->setTarget(D3DXVECTOR3(0, 0, 0));
	gCamera->setHeightOffset(50.0f);

	mWorld = new World();
	mLight = new Light(D3DXVECTOR3(0, 25, 0), D3DXVECTOR3(0, -1, 0), Material(WHITE, 8.0f), 32, 50.0f);
	mWorld->addLight(mLight); 
	mWorld->addAmbientLight(D3DCOLOR_ARGB(255, 255, 255, 255));

	mPlayer = new Player(D3DXVECTOR3(400, 1200, 700));
	mWorld->addObject(mPlayer);

	mCastleMesh = new Mesh("data/castle.x", D3DXVECTOR3(900.0f, 100.0f, 0.0f), 5.0f);

	normal = D3DXVECTOR3 (0, 1, 0);
	mTexture = gGraphics->loadTexture("data/aim.png");

	// Hide the cursor.
	ShowCursor(false);

	srand(time(0));
	// Add test enemies.
	for(int i = 0; i < 20; i++)
	{
		D3DXVECTOR3 position(0.0f, 10000.0f, 0.0f);
		position.x = rand() % 24000 - 12000;
		position.z = rand() % 24000 - 12000;

		Enemy* enemy = new Enemy(position);
		mWorld->addObject(enemy);
	}
}

//! Destructor.
Game::~Game()
{
	/*delete mWorld;
	delete gInput;
	delete gCamera;
	delete gGraphics;
	delete mGfxStats;*/
}

//! Updates everything.
void Game::update(float dt)
{
	// Update the world and everything it contains.
	mWorld->update(dt);

	if(mRotate) {
		mLight->setPosition(gCamera->getPosition() + D3DXVECTOR3(0, 5, 0));
		D3DXVECTOR3 t;
		mLight->setDirection(-*D3DXVec3Normalize(&t, &(gCamera->getPosition() - gCamera->getTarget())));
	}

	if(gInput->keyPressed(VK_F1))
		mRotate = !mRotate;

	mGfxStats->update(dt);

	// Update the keystate.
	gInput->update(dt);

	if(gInput->keyPressed(VK_F2)) 
		gCamera->setType(FPS);
	else if(gInput->keyPressed(VK_F3)) 
		gCamera->setType(NOCLIP);

	// Limits the mouse position to within the window.
	if(GetFocus() == getMainWnd()) {
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		ScreenToClient(getMainWnd(), &cursorPos);

		if(cursorPos.x < 0) 
			cursorPos.x = 0;
		else if(cursorPos.x > 1200-15)
			cursorPos.x = 1200-15;
		if(cursorPos.y < 0)
			cursorPos.y = 0;
		else if(cursorPos.y > 800-40)
			cursorPos.y = 800-40;

		ClientToScreen(getMainWnd(), &cursorPos);
		SetCursorPos(cursorPos.x, cursorPos.y);
	}
}
	
//! Draws everything.
void Game::draw()
{
	// Draw the world.
	mWorld->draw();

	// Debug (very slow).
	//gCamera->drawDebug();
	mGfxStats->display();

	gGraphics->drawMesh(mCastleMesh);
	//gGraphics->drawBoundingBox(mCastleMesh, GREEN, 0.7f);

	//gGraphics->drawTest(mBillboard, mTexture, D3DXVECTOR3(500, 500, 500), D3DXVECTOR3(500, 500, 500) - gCamera->getPosition());

	//gGraphics->drawRay(gCamera->getPosition() + D3DXVECTOR3(0, -10, 0) - gCamera->getDirection() * 15, gCamera->getDirection(), 500.0f, 1.0f);

	gGraphics->drawScreenTexture(mTexture, 600, 400, 32, 32);
}

World* Game::getWorld()
{
	return mWorld;
}

//! The starting msgProc function.
LRESULT Game::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = Runnable::msgProc(msg, wParam, lParam);
	return result;
}

void Game::buildBillboard()
{
	mTexture = gGraphics->loadTexture("data/billboard.bmp");

	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	UINT numElems = 0;
	VertexPNT::Decl->GetDeclaration(elems, &numElems);
	D3DXCreateMesh(2, 4, D3DXMESH_MANAGED, elems, gd3dDevice, &mBillboard);

	VertexPNT* v;
	WORD* k;

	mBillboard->LockVertexBuffer(0, (void**)&v);
	mBillboard->LockIndexBuffer(0, (void**)&k);

	v[0].pos = D3DXVECTOR3(-0.5, -0.5, 0);
	v[1].pos = D3DXVECTOR3(-0.5, 0.5, 0);
	v[2].pos = D3DXVECTOR3(0.5, 0.5, 0);
	v[3].pos = D3DXVECTOR3(0.5, -0.5, 0);

	v[0].tex0 = D3DXVECTOR2(0, 0);
	v[1].tex0 = D3DXVECTOR2(0, 1);
	v[2].tex0 = D3DXVECTOR2(1, 1);
	v[3].tex0 = D3DXVECTOR2(1, 0);

	k[0] = 0;
	k[1] = 1;
	k[2] = 2;
	k[3] = 0;
	k[4] = 2;
	k[5] = 3;

	mBillboard->UnlockVertexBuffer();
	mBillboard->UnlockIndexBuffer();
}

void Game::onLostDevice()
{
	mGfxStats->onLostDevice();
	mWorld->onLostDevice();
	gGraphics->onLostDevice();
}
	
void Game::onResetDevice()
{
	mGfxStats->onResetDevice();
	mWorld->onLostDevice();
	gGraphics->onResetDevice();
}