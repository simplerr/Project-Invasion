#include "Graphics.h"
#include "Level.h"
#include "World.h"
#include "Spawner.h"
#include "Player.h"
#include "PowerupSpawner.h"
#include "Input.h"
#include "LevelHandler.h"
#include "Game.h"
#include "Sound.h"

Level::Level(string name, string description, D3DXVECTOR3 playerSpawn, vector<Spawner*> spawnList)
{
	setCurrentWave(-1);
	mName = name;
	mDescription = description;
	mPlayerSpawn = playerSpawn;
	mSpawnList = spawnList;
	mSpawnDelta = mTimer = 0.0f;
	mSpawnedEnemies = 0;
	mStatusText = new StatusText("nothing", 0.3 * gGame->getScreenWidth(), 0.3 * gGame->getScreenHeight(), 0.0f);
	mPowerupSpawner = NULL;

	mSoundEffectDelta = 0.0f;
	mRandomEffectDelay = 1.0f;
}
	
Level::~Level()
{
	for(int i = 0; i < mWaveList.size(); i++)
		delete mWaveList[i];

	mWaveList.clear();

	if(mPowerupSpawner != NULL)
		delete mPowerupSpawner;

	delete mStatusText;
}

void Level::init(World* world, Player* player)
{
	mPlayer = player;

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

	// Setup the powerup spawner.
	mPowerupSpawner = new PowerupSpawner(0, 0, 1000);
	mPowerupSpawner->setWorld(mWorld);
}

void Level::update(float dt)
{
	if(mState == PLAYING)
	{
		// Sound code.
		mSoundEffectDelta += dt;
		if(mSoundEffectDelta >= mRandomEffectDelay) {
			gSound->setVolume(0.15);
			int num = rand() % 5;
			if(num == 0)
				gSound->playEffect("data/sound/zombie_pain1.wav");
			else if(num == 1)
				gSound->playEffect("data/sound/zombie_pain2.wav");
			else if(num == 2)
				gSound->playEffect("data/sound/nemesis_pain1.wav");
			else if(num == 3)
				gSound->playEffect("data/sound/nemesis_pain2.wav");
			else if(num == 4)
				gSound->playEffect("data/sound/nemesis_pain3.wav");

			mSoundEffectDelta = 0;
			mRandomEffectDelay = 0.5 + float((rand() % 101)) / 100.0f;
		}
	}

	mPowerupSpawner->update(dt);

	mSpawnDelta += dt;
	mTimer -= dt;
	mStatusText->update(dt);

	// Handle the state.
	if(mState == WAVE_COMPLETED || mState == LEVEL_COMPLETED)
	{
		if(mCurrentWave >= mCompletedWaves)
			mCompletedWaves = mState == LEVEL_COMPLETED ? mWaveList.size() : mCurrentWave;
		if(mTimer <= 0.0f && mState == WAVE_COMPLETED) {
			if(mCurrentWave == -1)
				mCurrentWave++;
			launchNextWave();
		}
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
				mCurrentWave++;
				mStatusText->setText("Wave completed! Prepare for the next one!", GREEN, 2.0f);
			}
			// Last wave.
			else if(mCurrentWave > mWaveList.size() - 2) {
				gLevelHandler->saveProgress();
				setState(LEVEL_COMPLETED, 2000.0f);

				if(atoi(getName().c_str()) < gLevelHandler->getNumLevels())
					mStatusText->setText("Level completed! Press <enter> to start the next level!", GREEN, 2000.0f);				
				else
					mStatusText->setText("You've completed all levels! Press <enter> to go the the menu!", GREEN, 2000.0f);				
			}
		}
	}
}

void Level::draw()
{
	mPowerupSpawner->draw();

	mStatusText->draw();

	static char buffer[256];
	if(mState == PLAYING) {
		sprintf(buffer, "Enemies left: %i", mWaveList[mCurrentWave]->getEnemiesLeft());
		gGraphics->drawText(buffer, gGame->getScreenWidth() * 0.85, 50, RED);
	}
	else if(mState == WAVE_COMPLETED) {
		sprintf(buffer, "Time untill next wave: %.2f", mTimer);
		gGraphics->drawText(buffer, gGame->getScreenWidth() * 0.85, 50, RED);
	}

	int completed = mState == LEVEL_COMPLETED ? mWaveList.size() : mCurrentWave;
	sprintf(buffer, "Waves completed: %i/%i", max(0, completed), mWaveList.size());
	gGraphics->drawText(buffer, gGame->getScreenWidth() * 0.85, 75, RED);
}

void Level::launchNextWave()
{
	mStatusText->setText("Wave incoming!", RED, 2.0f);
	mState = PLAYING;
	mSpawnedEnemies = mSpawnDelta = 0;

	// Set the spawn difficulty adjusts.
	for(int i = 0; i < mSpawnList.size(); i++) {
		mSpawnList[i]->setAdjusts(1.0f + mSpeedAdjust * mCurrentWave, 1.0f + mHealthAdjust * mCurrentWave, 1.0f + mDamageAdjust * mCurrentWave);
	}

	// Spawn the initial enemies in the wave.
	spawnEnemies(mWaveList[mCurrentWave]->getInitialEnemies());

	mPlayer->reset();
	gSound->playEffect("data/sound/prepare2.wav");
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

void Level::setCompletedWaves(int wavesCompleted)
{
	mCompletedWaves = wavesCompleted;
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

string Level::getName()
{
	return mName;
}

LevelState Level::getState()
{
	return mState;
}

int Level::getNumWaves()
{
	return mWaveList.size();
}

int Level::getCompletedWaves()
{
	return mCompletedWaves;
}

void Level::deleteSpawners()
{
	for(int i = 0; i < mSpawnList.size(); i++) {
		delete mSpawnList[i];
		mSpawnList[i] = NULL;
	}

	//mSpawnList.clear();
}