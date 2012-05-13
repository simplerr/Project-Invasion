#pragma once

#include "GameState.h"

class Level;
class RenderTarget;
class LevelHandler;
class Wave;

class PlayState : public GameState
{
public:
	void init(Game* game);
	void cleanup();

	void pause();
	void resume();

	void update(double dt);
	void draw();
	void msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	void onLostDevice();
	void onResetDevice();

	void limitCursor();

	void setLevel(string name);
	Wave* getCurrentWave();

	static PlayState* Instance() {
		return &mPlayState;
	}
protected:
	PlayState() {};
private:
	static PlayState mPlayState;

	IDirect3DTexture9* mTexture;
	RenderTarget* mRenderTarget;
	LevelHandler* mLevelHandler;
	Level*		mActiveLevel;
	World*		mWorld;
	Light*		mLight;
	Player*		mPlayer;
	Mesh*		mCastle;
	bool		mRotate;
};	// Class