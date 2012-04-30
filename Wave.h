#pragma once
#include <string>
#include <vector>
#include "d3dUtil.h"
using namespace std;

class World;
class Player;
class Spawner;

class Wave
{
public:
	Wave(World* world, Player* player, string name);
	~Wave();

	void update(float dt);
	void spawnEnemies(int num);
	void enemyKilled();

	void setWorld(World* world);
	void setPlayer(Player* player);
	int getEnemiesLeft();
private:
	vector<Spawner*> mSpawnList;
	World*	mWorld;
	Player* mPlayer;
	string	mName;
	string	mDescription;
	int		mTotalEnemies;
	int		mEnemiesLeft;
	int		mInitialEnemies;
	int		mSpawnedEnemies;
	float	mSpawnRate;
	float	mSpawnDelta;
};