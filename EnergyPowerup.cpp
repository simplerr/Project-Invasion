#include "EnergyPowerup.h"
#include "Player.h"
#include "PowerupSpawner.h"

EnergyPowerup::EnergyPowerup(D3DXVECTOR3 position)
	: Powerup("data/models/tree/tree0.x", position, ENERGY_POWERUP)
{
	mEnergy = 50;
}
	
EnergyPowerup::~EnergyPowerup()
{

}

void EnergyPowerup::pickup(Player* player)
{
	player->setEnergy(player->getEnergy() + mEnergy);
	getSpawner()->powerupRemoved();
	kill();
}