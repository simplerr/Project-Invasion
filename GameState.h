#pragma once
#include "Game.h"

class Game;

// Reference: http://gamedevgeek.com/tutorials/managing-game-states-in-c/

// Abstract baseclass
class GameState
{
public:
	virtual void init(Game* game) = 0;
	virtual void cleanup(void) = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void msgProc(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void update(double dt) = 0;
	virtual void draw() = 0;

	virtual void onLostDevice() = 0;
	virtual void onResetDevice() = 0;

	void setGame(Game* game) {
		mGame = game;
	}
	void changeState(GameState* state) {
		mGame->changeState(state);
	}
protected:
	GameState(){};
private:
	Game *mGame;
};	// Class

extern GameState* gGameState;