#pragma once

#include "Mesh.h"
class PowerupSpawner;
class Player;

class Powerup : public Mesh
{
public:
	Powerup(string source, D3DXVECTOR3 position, ObjectType type);
	~Powerup();

	void update(float dt);
	void draw();
	virtual void pickup(Player* player);

	void setSpawner(PowerupSpawner* spawner);
	PowerupSpawner* getSpawner();
private:
	PowerupSpawner* mSpawner;
};