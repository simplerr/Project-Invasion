#include "EnergyPowerup.h"
#include "Player.h"
#include "PowerupSpawner.h"
#include "Graphics.h"

EnergyPowerup::EnergyPowerup(D3DXVECTOR3 position)
	: Powerup("data/models/castle/castle.x", position, ENERGY_POWERUP)
{
	mEnergy = 50;
	setMinimapTexture("data/energy_icon_minimap.png");
}
	
EnergyPowerup::~EnergyPowerup()
{

}

void EnergyPowerup::draw()
{
	gGraphics->drawBoundingBox(getMinimapTexture(), getPosition(), Dimensions(50), WHITE, 1.0f);
}

void EnergyPowerup::pickup(Player* player)
{
	player->setEnergy(player->getEnergy() + mEnergy);
	getSpawner()->powerupRemoved();
	kill();
}