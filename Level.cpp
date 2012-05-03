#include "Graphics.h"
#include "Level.h"

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
}

void Level::update(float dt)
{
	// Update the current wave.
	mWaveList[mCurrentWave]->update(dt);
}

void Level::draw()
{
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