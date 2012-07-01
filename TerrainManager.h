#pragma once

#include <string>
#include <map>
using namespace std;
class Terrain;

class TerrainManager
{
public:
	TerrainManager();
	~TerrainManager();

	void loadTerrain(string source, int rows, int colums, float dx, float dz, float heightScale = 1.0f);
	Terrain* getTerrain(string source);
private:
	map<string, Terrain*> mTerrainMap;
};

extern TerrainManager* gTerrainManager;