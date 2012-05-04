#include "Graphics.h"
#include "Level.h"
#include "World.h"

Level::Level(string name, string description, D3DXVECTOR3 position)
{
	setCurrentWave(0);
	mName = name;
	mDescription = description;
	mPosition = position;
}
	
Level::~Level()
{

}

void Level::init()
{
	// Init the current wave.
	mWaveList[mCurrentWave]->init();
	mTimeElapsed = 0.0f;
	mState = LEVEL_STARTED;
}

void Level::update(float dt)
{
	mTimeElapsed += dt;

	// Update the current wave.
	mWaveList[mCurrentWave]->update(dt);

	// Wave completed.
	if(mWaveList[mCurrentWave]->getEnemiesLeft() <= 0) {
		mState = WAVE_COMPLETED;
		mCurrentWave++;
		mWaveList[mCurrentWave]->init();
		mTimeElapsed = 0.0f;
		mWorld->reset();
	}
}

void Level::draw()
{
	if(mTimeElapsed < 3.0f && mState == LEVEL_STARTED)
		gGraphics->drawText(mDescription, 400, 400, GREEN);
	else if(mState == LEVEL_STARTED){
		mTimeElapsed = 0.0f;
		mState = PLAYING;
	}

	if(mTimeElapsed < 3.0f && mState == WAVE_COMPLETED)
		gGraphics->drawText("Wave completed! Prepare for the next one!", 400, 400, GREEN);
	else if(mState == WAVE_COMPLETED) {
		mTimeElapsed = 0.0f;
		mState = PLAYING;
	}

	char buffer[256];
	sprintf(buffer, "Enemies left: %i", mWaveList[mCurrentWave]->getEnemiesLeft());
	gGraphics->drawText(buffer, 700, 300, GREEN);
}

void Level::setWaves(vector<Wave*> waveList)
{
	mWaveList = waveList;
}

void Level::setCurrentWave(int currentWave)
{
	mCurrentWave = currentWave;
}

bool Level::completedWave()
{
	if(mWaveList[mCurrentWave]->getEnemiesLeft() <= 0)
		return true;
	else
		return false;
}

Wave* Level::getCurrentWave()
{
	return mWaveList[mCurrentWave];
}

void Level::setWorld(World* world)
{
	mWorld = world;
}