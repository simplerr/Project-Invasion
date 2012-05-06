#pragma once

#include <vector>
#include <string>
#include "Wave.h"
#include "d3dUtil.h"
#include "StatusText.h"
using namespace std;

class World;

enum LevelState
{
	LEVEL_STARTED,
	WAVE_COMPLETED,
	LEVEL_COMPLETED,
	PLAYING
};

class Level
{
public:
	Level(string name, string description, D3DXVECTOR3 position);
	~Level();

	void update(float dt);
	void draw();
	void init();

	void launchNextWave();
	void setCurrentWave(int currentWave);
	void setWaves(vector<Wave*> waveList);
	void setState(LevelState state, float time = 0.0f);
	bool completedWave();
	Wave* getCurrentWave();

	LevelState getState();

	void setWorld(World* world);
private:
	vector<Wave*> mWaveList;
	string mName;
	string mDescription;
	D3DXVECTOR3 mPosition;
	LevelState mState;
	World* mWorld;
	StatusText* mStatusText;
	float mTimer;
	int mCurrentWave;
};
