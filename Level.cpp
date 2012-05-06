#include "Graphics.h"
#include "Level.h"
#include "World.h"

Level::Level(string name, string description, D3DXVECTOR3 position)
{
	setCurrentWave(-1);
	mName = name;
	mDescription = description;
	mPosition = position;
	mStatusText = new StatusText("nothing", 400, 400, 0.0f);
}
	
Level::~Level()
{

}

void Level::init()
{
	// 5 seconds untill the first wave.
	setState(WAVE_COMPLETED, 5.0f);
	mStatusText->setText(mDescription, GREEN, 3.0f);
}

void Level::update(float dt)
{
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
		// Update the current wave.
		mWaveList[mCurrentWave]->update(dt);

		// Wave completed.
		if(mWaveList[mCurrentWave]->getEnemiesLeft() <= 0)
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
	mWorld->reset();
	mCurrentWave++;
	mWaveList[mCurrentWave]->init();
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

bool Level::completedWave()
{
	if(mCurrentWave != -1 && mWaveList[mCurrentWave]->getEnemiesLeft() <= 0 && mCurrentWave > mWaveList.size() - 2)
		return true;
	else
		return false;
}

void Level::setWorld(World* world)
{
	mWorld = world;
}

Wave* Level::getCurrentWave()
{
	return mWaveList[mCurrentWave];
}

LevelState Level::getState()
{
	return mState;
}