#pragma once

class Bar;
class Player;

class Gui
{
public:
	Gui(Player* player);
	~Gui();

	void update(float dt);
	void draw();

	void setPlayer(Player* player);
private:
	Player* mPlayer;
	Bar* mHealthBar;
	Bar* mAmmoBar;
};