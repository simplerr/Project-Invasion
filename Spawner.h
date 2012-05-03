#pragma once
#include "Mesh.h"
#include "EnemyHandler.h"

class Player;

class Spawner : public Mesh
{
public:
	Spawner(D3DXVECTOR3 position);
	~Spawner();

	void init();
	void update(float dt);
	void draw();
	void spawnObject();

	void setAdjusts(float speed, float damage, float health);

	void setPlayer(Player* player);
private:
	Player* mPlayer;
	EnemyData mToroData;
	EnemyData mMonsterData;
};