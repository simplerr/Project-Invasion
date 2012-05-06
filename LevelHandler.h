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

	Level* getLevel(int num);
private:
	vector<Level*> mLevelList;
};