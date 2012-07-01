#include "Powerup.h"
#include "Player.h"

Powerup::Powerup(D3DXVECTOR3 position, ObjectType type)
	: Object3D(position, type)
{

}

Powerup::~Powerup()
{

}

void Powerup::update(float dt)
{

}

void Powerup::draw()
{
	
}

void Powerup::pickup(Player* player)
{

}

void Powerup::setSpawner(PowerupSpawner* spawner)
{
	mSpawner = spawner;
}

PowerupSpawner* Powerup::getSpawner()
{
	return mSpawner;
}