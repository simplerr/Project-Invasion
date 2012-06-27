#pragma once

#include "d3dUtil.h"

class Bar;
class Player;
class SkillBar;
class SkillHandler;

class Gui
{
public:
	Gui(Player* player);
	~Gui();

	void update(float dt);
	void draw();

	void setPlayer(Player* player);
private:
	Player*		mPlayer;
	SkillHandler* mSkillHandler;
	SkillBar*	mSkillBar;
	IDirect3DTexture9* mAmmoIcon;
	Bar*		mHealthBar;
	Bar*		mAmmoBar;
	Bar*		mEneryBar;
};