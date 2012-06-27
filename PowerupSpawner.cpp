#include "PowerupSpawner.h"
#include "World.h"
#include "EnergyPowerup.h"

PowerupSpawner::PowerupSpawner(float x, float z, float size)
{
	mCenter = D3DXVECTOR3(x, 0, z);
	mSize = size;
	mSpawnInterval = 5.0f;
	mMaxPowerups = 2;
	mNumSpawned = 0;
	mDelta = 0.0f;
}

PowerupSpawner::~PowerupSpawner()
{

}

void PowerupSpawner::update(float dt)
{
	mDelta += dt;

	if(mDelta >= mSpawnInterval && mNumSpawned < mMaxPowerups) {
		spawnPowerup();
		mNumSpawned++;
		mDelta = 0;
	}
	else if(mNumSpawned >= mMaxPowerups && mDelta <= mSpawnInterval)
		mDelta = 0;
}
	
void PowerupSpawner::draw()
{

}

void PowerupSpawner::clear()
{
	mNumSpawned = 0;
}

void PowerupSpawner::spawnPowerup()
{
	float x = rand() % mSize;
	float z = rand() % mSize;
	EnergyPowerup* powerup = new EnergyPowerup(D3DXVECTOR3(x, 5000, z));
	powerup->setSpawner(this);
	mWorld->addObject(powerup);
}

void PowerupSpawner::powerupRemoved()
{
	mNumSpawned--;
}

void PowerupSpawner::setWorld(World* world)
{
	mWorld = world;
}