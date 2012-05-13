#pragma once

#include <vector>
#include <string>
#include "Wave.h"
#include "d3dUtil.h"
#include "StatusText.h"
using namespace std;

class World;
class Spawner;
class Player;

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
	Level(string name, string description, D3DXVECTOR3 position, vector<Spawner*> spawnList);
	~Level();

	void update(float dt);
	void draw();
	void init(World* world, Player* player);

	void launchNextWave();
	void spawnEnemies(int enemies);
	void setCurrentWave(int currentWave);
	void setWaves(vector<Wave*> waveList);
	void setState(LevelState state, float time = 0.0f);
	void setDifficultyAdjusts(float speed, float health, float damage);
	void setCompletedWaves(int wavesCompleted);
	bool completedWave();

	Wave* getCurrentWave();
	LevelState getState();
	string getName();
	int getNumWaves();
	int getCompletedWaves();
private:
	vector<Wave*>	 mWaveList;
	vector<Spawner*> mSpawnList;
	World* mWorld;
	string mName;
	string mDescription;
	D3DXVECTOR3 mPlayerSpawn;
	LevelState mState;
	StatusText* mStatusText;
	float	mTimer;
	float	mSpawnDelta;
	float	mSpeedAdjust;
	float	mHealthAdjust;
	float	mDamageAdjust;
	int mCurrentWave;
	int mSpawnedEnemies;
	int mCompletedWaves;
};
