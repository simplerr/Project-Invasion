#pragma once

#include <vector>
#include <string>
#include "Wave.h"
#include "d3dUtil.h"
using namespace std;

class World;

enum LevelState
{
	LEVEL_STARTED,
	WAVE_COMPLETED,
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

	void setCurrentWave(int currentWave);
	void setWaves(vector<Wave*> waveList);
	bool completedWave();
	Wave* getCurrentWave();

	void setWorld(World* world);
private:
	vector<Wave*> mWaveList;
	string mName;
	string mDescription;
	D3DXVECTOR3 mPosition;
	LevelState mState;
	World* mWorld;
	float mTimeElapsed;
	int mCurrentWave;
};
