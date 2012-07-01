#pragma once
#include <vector>
#include "tinyxml\tinystr.h";
#include "tinyxml\tinyxml.h"
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
	void saveProgress();
	void deleteSpawners();

	Level* getLevel(int num);
	Level* getLevel(string name);
	int getNumLevels();
private:
	vector<Level*> mLevelList;
	TiXmlDocument mDocument;
};

extern LevelHandler* gLevelHandler;