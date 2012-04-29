#pragma once

#include "GameState.h"

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

	static PlayState* Instance() {
		return &mPlayState;
	}
protected:
	PlayState() {};
private:
	static PlayState mPlayState;

	IDirect3DTexture9* mTexture;
	World*		mWorld;
	Light*		mLight;
	Player*		mPlayer;
	
	Mesh*		mCastle;
	bool		mRotate;
};	// Class