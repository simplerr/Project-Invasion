#include "PowerupSpawner.h"
#include "World.h"
#include "EnergyPowerup.h"

PowerupSpawner::PowerupSpawner(float x, float z, float size)
{
	mCenter = D3DXVECTOR3(x, 0, z);
	mSize = size;
	mSpawnInterval = 1.0f;
	mMaxPowerups = 10;
	mNumSpawned = 0;
}

PowerupSpawner::~PowerupSpawner()
{

}

void PowerupSpawner::update(float dt)
{
	static float delta = 0;
	delta += dt;

	if(delta >= mSpawnInterval && mNumSpawned < mMaxPowerups) {
		spawnPowerup();
		delta = 0;
		mNumSpawned++;
	}
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