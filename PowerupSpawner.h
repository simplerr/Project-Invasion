#pragma once

#include "d3dUtil.h"
class World;

class PowerupSpawner
{
public:
	PowerupSpawner(float x, float z, float size);
	~PowerupSpawner();

	void update(float dt);
	void draw();
	void clear();
	void spawnPowerup();
	void powerupRemoved();

	void setWorld(World* world);
private:
	World* mWorld;
	D3DXVECTOR3 mCenter;
	float mDelta;
	int mSize;
	int mSpawnInterval;
	int mMaxPowerups;
	int mNumSpawned;
};