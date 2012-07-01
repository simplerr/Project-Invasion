#pragma once

#include "GameState.h"
#include "Menu.h"

class World;
class Light;

class MainMenu : public GameState
{
public:
	void init(Game* game);
	void cleanup();

	void pause();
	void resume();

	void update(double dt);
	void draw();
	void msgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	bool menuMessage(string message);

	void onLostDevice();
	void onResetDevice();

	static MainMenu* Instance() {
		return &mMainMenu;
	}
protected:
	MainMenu() {};
private:
	static MainMenu mMainMenu;
	Menu* mMenu;
	World* mWorld;
	Light* mLight;
};	// Class