#pragma once

#include "Powerup.h"

class EnergyPowerup : public Powerup
{
public:
	EnergyPowerup(D3DXVECTOR3 position);
	~EnergyPowerup();

	void pickup(Player* player);
private:
	float mEnergy;
};