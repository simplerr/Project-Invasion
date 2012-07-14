#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

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
#include "MeshFactory.h"
#include "MeshFactory.h"
#include "EffectManager.h"
#include "ParticleSystem.h"
#include "BloodPSystem.h"
#include "EnemyHandler.h"
#include "GameState.h"
#include "PlayState.h"
#include "EditorState.h"
#include "RenderTarget.h"
#include  "SelectLevel.h"
#include "LevelHandler.h"
#include "EnemyHandler.h"
#include "WeaponHandler.h"
#include "SkillHandler.h"
#include "MainMenu.h"
#include "TerrainManager.h"
#include "Sound.h"
#include "vld.h"

//#include "F:/Users/Axel/Documents/Visual Studio 11/Memory_and_Exception_Trace/Stackwalker.h"

// Set the globals
Runnable*			gGame			= 0;
IDirect3DDevice9*	gd3dDevice		= 0;
Input*				gInput			= 0;
Graphics*			gGraphics		= 0;
Camera*				gCamera			= 0;
MeshFactory*		gMeshFactory	= 0;
EffectManager*		gEffectManager	= 0;
LevelHandler*		gLevelHandler	= 0;
EnemyHandler*		gEnemyHandler	= 0;
WeaponHandler*		gWeaponHandler	= 0;
SkillHandler*		gSkillHandler	= 0;
TerrainManager*		gTerrainManager	= 0;
Sound*				gSound			= 0;


// Use Windows 7 looking controls.
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//! The program starts here.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	//InitAllocCheck(ACOutput_XML); // ACOutput_XML

	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	Game game(hInstance, "DirectX_3D-1", 1200, 800, D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gGame = &game;

	gInput = new Input();
	//gGame->init();

	// Run the game
	return gGame->run();
}

//! Constructor.
Game::Game(HINSTANCE hInstance, string caption, int width, int height, D3DDEVTYPE devType, DWORD requestedVP)
	:Runnable(hInstance, caption, width, height, devType, requestedVP)
{
	InitAllVertexDeclarations();
	
	mCurrentState	= NULL;
	gGraphics		= new Graphics();
	mLoadingTexture = gGraphics->loadTexture("data/imgs/loading.png");
	mInitState		= STARTUP;
}

void Game::init()
{
	gCamera			= new Camera();
	gMeshFactory	= new MeshFactory();
	gEffectManager	= new EffectManager();
	gLevelHandler	= new LevelHandler();
	gWeaponHandler	= new WeaponHandler();
	gSkillHandler	= new SkillHandler();
	gEnemyHandler	= new EnemyHandler();
	gTerrainManager = new TerrainManager();
	gSound			= new Sound();
	gLevelHandler->loadLevels();
	gGraphics->init();
	
	mGfxStats = new GfxStats();
	gSound->setVolume(0.3f);

	// Pre-load models.
	LPD3DXFRAME root;
	ID3DXAnimationController* animCtrl;
	gMeshFactory->loadSkinnedMesh("data/models/toro/toro.x", root, animCtrl);
	gMeshFactory->loadSkinnedMesh("data/models/monster/monster.x", root, animCtrl);
	gMeshFactory->loadSkinnedMesh("data/models/mp5/mp5.x", root, animCtrl);
	gMeshFactory->loadSkinnedMesh("data/models/smith/smith.x", root, animCtrl);

	ShowCursor(false);

	// Set the current state.
	mCurrentState = NULL;
	changeState(MainMenu::Instance());
	mInitState = RUNNING;

	gGame->switchScreenMode();
}

//! Destructor.
Game::~Game()
{
	// Clean up the current state.
	mCurrentState->cleanup();
	delete gMeshFactory;
	delete gInput;
	delete gCamera;
	delete gGraphics;
	delete gEffectManager;
	delete gLevelHandler;
	delete gEnemyHandler;
	delete gWeaponHandler;
	delete gSkillHandler;
	delete gTerrainManager;
	delete gSound;
	delete mGfxStats;
	ReleaseCOM(gd3dDevice);

	//DeInitAllocCheck();

	// Clean up all vertex declerations.
	DestroyAllVertexDeclarations();
}

void Game::changeState(GameState* state)
{
	// Cleanup the old state.
	if(mCurrentState != NULL)
		mCurrentState->cleanup();

	// Set and init the new state.
	mCurrentState = state;
	mCurrentState->init(this);
}

//! Updates everything.
void Game::update(float dt)
{
	if(mInitState == INIT)
		init();
	else if(mInitState == RUNNING)
	{
		// Update the keystate.
		gInput->update(dt);

		// Update the current state.
		mCurrentState->update(dt);
		//mGfxStats->update(dt);
	}
}
	
//! Draws everything.
void Game::draw()
{
	if(mInitState == STARTUP) {
		gGraphics->drawScreenTexture(mLoadingTexture, 600, 400, 1200, 800);
		mInitState = INIT;
	}
	if(mInitState == RUNNING)
	{
		// Draw the current state.
		mCurrentState->draw();

		// Debug (very slow).
		//mGfxStats->display();
		//gCamera->drawDebug();
	}

	/*char buffer[256];
	sprintf(buffer, "ratio: %f, %f", widthRatio(), heightRatio());
	gGraphics->drawText(buffer, 600 * widthRatio(), 400 * heightRatio());

	sprintf(buffer, "x: %f, y: %f", gInput->mousePosition().x, gInput->mousePosition().y);
	gGraphics->drawText(buffer, 100 * widthRatio(), 400 * heightRatio());*/
}

//! The starting msgProc function.
LRESULT Game::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Let the current state handle messages.
	if(mInitState == RUNNING)
		mCurrentState->msgProc(msg, wParam, lParam);

	gInput->msgProc(msg, wParam, lParam);
	LRESULT result = Runnable::msgProc(msg, wParam, lParam);
	return result;
}

void Game::onLostDevice()
{
	if(mCurrentState != NULL) {
	// Pass on to the current state.
	mCurrentState->onLostDevice();

	mGfxStats->onLostDevice();
	gGraphics->onLostDevice();
	gEffectManager->onLostDevice();
	}
}
	
void Game::onResetDevice()
{
	if(mCurrentState != NULL) {
	// Pass on to the current state.
	mCurrentState->onResetDevice();

	mGfxStats->onResetDevice();
	gGraphics->onResetDevice();
	gEffectManager->onResetDevice();
	}
}