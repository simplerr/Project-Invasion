#include "TerrainManager.h"
#include "Terrain.h"

TerrainManager::TerrainManager()
{
	loadTerrain("data/heightmap17_257.raw", 257, 257, 100, 100, 16.0f);
	//loadTerrain("data/heightmap17_257.raw", 257, 257, 1, 1, 1);
}
	
TerrainManager::~TerrainManager()
{
	for(auto iter = mTerrainMap.begin(); iter != mTerrainMap.end(); iter++) 
	{
		delete (iter)->second;
	}
}

Terrain* TerrainManager::getTerrain(string source)
{
	if(mTerrainMap.find(source) != mTerrainMap.end())
		return mTerrainMap[source];
	else
		return NULL;
}

void TerrainManager::loadTerrain(string source, int rows, int colums, float dx, float dz, float heightScale)
{
	mTerrainMap[source] = new Terrain(source, rows, colums, dx, dz, heightScale);
}