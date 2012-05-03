#pragma once

#include <vector>
#include <string>
#include "Wave.h"
#include "d3dUtil.h"
using namespace std;

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
private:
	vector<Wave*> mWaveList;
	string mName;
	string mDescription;
	D3DXVECTOR3 mPosition;
	int mCurrentWave;
};
