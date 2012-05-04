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
	Wave(World* world, Player* player);
	~Wave();

	void update(float dt);
	void init();
	void spawnEnemies(int num);
	void enemyKilled();

	void setData(vector<Spawner*> spawnList, int enemies, int initialEnemies, float spawnRate, float speedAdjust, float damageAdjust, float healthAdjust);
	void setWorld(World* world);
	void setPlayer(Player* player);
	int getEnemiesLeft();
	int getInitialEnemies();
private:
	vector<Spawner*> mSpawnList;
	World*	mWorld;
	Player* mPlayer;
	int		mTotalEnemies;
	int		mEnemiesLeft;
	int		mInitialEnemies;
	int		mSpawnedEnemies;
	float	mSpawnRate;
	float	mSpawnDelta;
	float	mSpeedAdjust;
	float	mDamageAdjust;
	float	mHealthAdjust;
};