#include "Powerup.h"
#include "Player.h"

Powerup::Powerup(string source, D3DXVECTOR3 position, ObjectType type)
	: Mesh(source, position, type)
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
	Mesh::draw();
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