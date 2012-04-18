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

class Game : public Runnable
{
public:
	Game(HINSTANCE hInstance, string caption, int width, int height, D3DDEVTYPE devType, DWORD requestedVP);
	~Game();

	void update(float dt);
	void draw();

	void buildBillboard();

	World* getWorld();

	void onLostDevice();
	void onResetDevice();
	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	IDirect3DTexture9* mTexture;
	World*		mWorld;
	GfxStats*	mGfxStats;
	Light*		mLight;
	bool		mRotate;
	Mesh*		mCastleMesh;
	ID3DXMesh*	mBillboard;
	Player*		mPlayer;
};