#pragma once

#include "d3dUtil.h"
#include <string>
using namespace std;

class Bar;
class Player;
class SkillBar;
class SkillHandler;
class Menu;

class Gui
{
public:
	Gui(Player* player);
	~Gui();

	void update(float dt);
	void draw();
	void toggleMenu();

	void setPlayer(Player* player);
	bool menuMessage(string message);
private:
	Player*		mPlayer;
	SkillBar*	mSkillBar;
	IDirect3DTexture9* mAmmoIcon;
	IDirect3DTexture9* mUiBorder;
	Bar*		mHealthBar;
	Bar*		mAmmoBar;
	Bar*		mEneryBar;
	Menu*		mMenu;
};