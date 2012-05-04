#include "Wave.h"
#include "World.h"
#include "EnemyHandler.h"
#include "Enemy.h"
#include "Player.h"
#include "Spawner.h"

Wave::Wave(World* world, Player* player)
{
	setWorld(world);
	setPlayer(player);
}
	
Wave::~Wave()
{

}

void Wave::init()
{
	for(int i = 0; i < mSpawnList.size(); i++) {
		mSpawnList[i]->setPlayer(mPlayer);
		mWorld->addObject(mSpawnList[i]);
		mSpawnList[i]->setAdjusts(mSpeedAdjust, mDamageAdjust, mHealthAdjust);
	}

	// Spawn the initial enemies.
	spawnEnemies(mInitialEnemies);
}

void Wave::update(float dt)
{
	mSpawnDelta += dt;

	// Spawn new enemy.
	if(mSpawnDelta >= mSpawnRate && mSpawnedEnemies < mTotalEnemies) {
		spawnEnemies(1);
		mSpawnDelta = 0.0f;
	}
}

void Wave::spawnEnemies(int num)
{
	for(int i = 0; i < num; i++)
	{
		// Increment spawn counter.
		mSpawnedEnemies++;

		// Randomize spawn position.
		int spawn = rand() % mSpawnList.size();
		mSpawnList[spawn]->spawnObject();
	}
}

void Wave::enemyKilled()
{
	mEnemiesLeft -= 1;
}

void Wave::setData(vector<Spawner*> spawnList, int enemies, int initialEnemies, float spawnRate, float speedAdjust, float damageAdjust, float healthAdjust)
{
	mTotalEnemies = enemies;
	mEnemiesLeft = mTotalEnemies;
	mInitialEnemies = initialEnemies;
	mSpawnedEnemies = 0;
	mSpawnRate = spawnRate;
	mSpawnDelta = 0.0f;

	mSpawnList = spawnList;
	mSpeedAdjust = speedAdjust;
	mDamageAdjust = damageAdjust;
	mHealthAdjust = healthAdjust;
}

void Wave::setWorld(World* world)
{
	mWorld = world;
}
	
void Wave::setPlayer(Player* player)
{
	mPlayer = player;
}

int Wave::getEnemiesLeft()
{
	return mEnemiesLeft;
}

int Wave::getInitialEnemies()
{
	return mInitialEnemies;
}