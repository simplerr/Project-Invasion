#pragma once

#include "GameState.h"
#include "Menu.h"
#include "LevelHandler.h"

class LevelItem : public MenuItem
{
public:
	LevelItem(string name, string standardTextureSource, string onSelectTextureSource = "#none");
	~LevelItem();

	void draw();

	int numLevel;
	int waves;
	int completedWaves;
};

class SelectLevel : public GameState
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

	static SelectLevel* Instance() {
		return &mSelectLevel;
	}
protected:
	SelectLevel() {};
private:
	static SelectLevel mSelectLevel;
	LevelHandler* mLevelHandler;
	Menu* mMenu;
};	// Class