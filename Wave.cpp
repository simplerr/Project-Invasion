#include "Wave.h"
#include "World.h"
#include "EnemyHandler.h"
#include "Enemy.h"
#include "Player.h"
#include "Spawner.h"

Wave::Wave(int enemies, int initialEnemies, float spawnRate)
{
	mTotalEnemies = enemies;
	mEnemiesLeft = mTotalEnemies;
	mInitialEnemies = initialEnemies;
	mSpawnRate = spawnRate;
}
	
Wave::~Wave()
{

}

void Wave::enemyKilled()
{
	mEnemiesLeft -= 1;
}

int Wave::getEnemiesLeft()
{
	return mEnemiesLeft;
}

int Wave::getInitialEnemies()
{
	return mInitialEnemies;
}

int Wave::getTotalEnemies()
{
	return mTotalEnemies;
}

float Wave::getSpawnRate()
{
	return mSpawnRate;
}