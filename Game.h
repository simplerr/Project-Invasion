#pragma once
#include "Runnable.h"
#include "Camera.h"
#include "Graphics.h"
#include "GfxStats.h"
#include <vector>
using namespace std;

class World;
class Light;
class Mesh;
class Player;
class MeshFactory;
class ParticleSystem;
class EnemyHandler;
class GameState;

class Game : public Runnable
{
public:
	Game(HINSTANCE hInstance, string caption, int width, int height, D3DDEVTYPE devType, DWORD requestedVP);
	~Game();

	void update(float dt);
	void draw();
	void changeState(GameState* state);

	void onLostDevice();
	void onResetDevice();
	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	GameState*	mCurrentState;
	GfxStats*	mGfxStats;
	int			mState;
};