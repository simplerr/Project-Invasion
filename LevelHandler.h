#pragma once

#include <vector>
#include "Level.h"
using namespace std;

class TiXmlElement;
class World;
class Player;

class LevelHandler
{
public:
	LevelHandler();
	~LevelHandler();

	void loadLevels();
	vector<Wave*> loadWaves(TiXmlElement* level);

	void setWorld(World* world);
	void setPlayer(Player* player);

	Level* getLevel(int num);
private:
	vector<Level*> mLevelList;
	World*		mWorld;
	Player*		mPlayer;
};