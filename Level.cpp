#include "Graphics.h"
#include "Level.h"
#include "World.h"
#include "Spawner.h"
#include "Player.h"

Level::Level(string name, string description, D3DXVECTOR3 playerSpawn, vector<Spawner*> spawnList)
{
	setCurrentWave(-1);
	mName = name;
	mDescription = description;
	mPlayerSpawn = playerSpawn;
	mSpawnList = spawnList;
	mSpawnDelta = mTimer = 0.0f;
	mSpawnedEnemies = 0;
	mStatusText = new StatusText("nothing", 400, 400, 0.0f);
}
	
Level::~Level()
{

}

void Level::init(World* world, Player* player)
{
	// Setup the spawns.
	for(int i = 0; i < mSpawnList.size(); i++) {
		mSpawnList[i]->setPlayer(player);
		world->addObject(mSpawnList[i]);
		mSpawnList[i]->init();
	}

	// 5 seconds untill the first wave.
	setState(WAVE_COMPLETED, 5.0f);
	mStatusText->setText(mDescription, GREEN, 3.0f);

	mWorld = world;
}

void Level::update(float dt)
{
	mSpawnDelta += dt;
	mTimer -= dt;
	mStatusText->update(dt);

	// Handle the state.
	if(mState == WAVE_COMPLETED)
	{
		if(mTimer <= 0.0f) 
			launchNextWave();
	}
	else if(mState == PLAYING)
	{
		Wave* currentWave = mWaveList[mCurrentWave];

		// Spawn enemies.
		if(mSpawnDelta >= currentWave->getSpawnRate() && mSpawnedEnemies < currentWave->getTotalEnemies()) 
			spawnEnemies(1);

		// Wave completed.
		if(currentWave->getEnemiesLeft() <= 0)
		{
			// More waves left.
			if(mCurrentWave <= mWaveList.size() - 2) {
				setState(WAVE_COMPLETED, 4.0f);
				mStatusText->setText("Wave completed! Prepare for the next one!", GREEN, 2.0f);
			}
			// Last wave.
			else if(mCurrentWave > mWaveList.size() - 2) {
				setState(LEVEL_COMPLETED, 2.0f);
				mStatusText->setText("Level completed!", GREEN, 2.0f);
			}
		}
	}
}

void Level::draw()
{
	mStatusText->draw();

	static char buffer[256];
	if(mState == PLAYING) {
		sprintf(buffer, "Enemies left: %i", mWaveList[mCurrentWave]->getEnemiesLeft());
		gGraphics->drawText(buffer, 800, 200, RED);
	}
	else if(mState == WAVE_COMPLETED) {
		sprintf(buffer, "Time untill next wave: %.2f", mTimer);
		gGraphics->drawText(buffer, 800, 200, RED);
	}
}

void Level::launchNextWave()
{
	mStatusText->setText("Wave incoming!", RED, 2.0f);
	mState = PLAYING;
	mCurrentWave++;
	mSpawnedEnemies = mSpawnDelta = 0;

	// Set the spawn difficulty adjusts.
	for(int i = 0; i < mSpawnList.size(); i++) {
		mSpawnList[i]->setAdjusts(1.0f + mSpeedAdjust * mCurrentWave, 1.0f + mHealthAdjust * mCurrentWave, 1.0f + mDamageAdjust * mCurrentWave);
	}

	// Spawn the initial enemies in the wave.
	spawnEnemies(mWaveList[mCurrentWave]->getInitialEnemies());
}

void Level::spawnEnemies(int enemies)
{
	for(int i = 0; i < enemies; i++)
	{
		// Increment spawn counter.
		mSpawnedEnemies++;

		// Randomize spawn position.
		int spawn = rand() % mSpawnList.size();
		mSpawnList[spawn]->spawnObject();
	}

	// Reset the spawn delta.
	mSpawnDelta = 0.0f;
}

void Level::setState(LevelState state, float time)
{
	mState = state;
	mTimer = time;
}

void Level::setWaves(vector<Wave*> waveList)
{
	mWaveList = waveList;
}

void Level::setCurrentWave(int currentWave)
{
	mCurrentWave = currentWave;
}

void Level::setDifficultyAdjusts(float speed, float health, float damage)
{
	mSpeedAdjust = speed;
	mHealthAdjust = health;
	mDamageAdjust = damage;
}

bool Level::completedWave()
{
	if(mCurrentWave != -1 && mWaveList[mCurrentWave]->getEnemiesLeft() <= 0 && mCurrentWave > mWaveList.size() - 2)
		return true;
	else
		return false;
}

Wave* Level::getCurrentWave()
{
	return mWaveList[mCurrentWave];
}

LevelState Level::getState()
{
	return mState;
}