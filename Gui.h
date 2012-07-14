#pragma once

#include "d3dUtil.h"
#include <string>
using namespace std;

class Bar;
class Player;
class SkillBar;
class SkillHandler;
class Menu;

enum GuiMenu {GM_PAUSED, GM_DEAD, GM_NONE};

class Gui
{
public:
	Gui(Player* player);
	~Gui();

	void update(float dt);
	void draw();
	void toggleMenu();

	bool isMenuVisible();
	void setPlayer(Player* player);
	bool menuMessage(string message);
	void setMapTexture(IDirect3DTexture9* texture);
private:
	Player*		mPlayer;
	SkillBar*	mSkillBar;
	IDirect3DTexture9* mAmmoIcon;
	IDirect3DTexture9* mGameOverTexture;
	IDirect3DTexture9* mUiBorder;
	IDirect3DTexture9* mWhiteOverlay;
	IDirect3DTexture9* mMapTexture;
	IDirect3DTexture9* mCrossHair;
	Bar*		mHealthBar;
	Bar*		mAmmoBar;
	Bar*		mEneryBar;
	Menu*		mMenu;
	GuiMenu		mCurrentMenu;
};