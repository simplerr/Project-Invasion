#pragma once

#include "Object3D.h"
class PowerupSpawner;
class Player;

class Powerup : public Object3D
{
public:
	Powerup(D3DXVECTOR3 position, ObjectType type);
	~Powerup();

	void update(float dt);
	virtual void draw();
	virtual void pickup(Player* player);

	void setSpawner(PowerupSpawner* spawner);
	PowerupSpawner* getSpawner();
private:
	PowerupSpawner* mSpawner;
};