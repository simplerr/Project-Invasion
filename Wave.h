#pragma once
#include <string>
#include <vector>
#include "d3dUtil.h"
using namespace std;

class Wave
{
public:
	Wave(int enemies, int initialEnemies, float spawnRate);
	~Wave();

	void enemyKilled();

	float getSpawnRate();
	int getEnemiesLeft();
	int getInitialEnemies();
	int	getTotalEnemies();
private:
	int		mTotalEnemies;
	int		mEnemiesLeft;
	int		mInitialEnemies;
	float	mSpawnRate;
};