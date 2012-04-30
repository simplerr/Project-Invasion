#include "Wave.h"
#include "World.h"
#include "EnemyHandler.h"
#include "Enemy.h"
#include "Player.h"
#include "Spawner.h"

Wave::Wave(World* world, Player* player, string name)
{
	mName = name;
	mDescription = "This is the first level! Good luck and try to not die!";
	mTotalEnemies = 30;
	mEnemiesLeft = mTotalEnemies;
	mInitialEnemies = 5;
	mSpawnedEnemies = 0;
	mSpawnRate = 1.0f;
	mSpawnDelta = 0.0f;

	// Add some spawn points.
	mSpawnList.push_back(new Spawner(D3DXVECTOR3(0, 2000, 0)));
	mSpawnList.push_back(new Spawner(D3DXVECTOR3(0, 2000, 1000)));
	mSpawnList.push_back(new Spawner(D3DXVECTOR3(0, 2000, -1000)));
	mSpawnList.push_back(new Spawner(D3DXVECTOR3(1000, 2000, 0)));

	setWorld(world);
	setPlayer(player);

	// Add them to the world.
	for(int i = 0; i < mSpawnList.size(); i++) {
		mWorld->addObject(mSpawnList[i]);
		mSpawnList[i]->setPlayer(mPlayer);
	}

	// Spawn the initial enemies.
	spawnEnemies(mInitialEnemies);
}
	
Wave::~Wave()
{

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