#pragma once
#include "Object3D.h"
#include "EnemyHandler.h"

class Player;

class Spawner : public Object3D
{
public:
	Spawner(D3DXVECTOR3 position);
	~Spawner();

	void init();
	void update(float dt);
	void draw();
	void spawnObject();

	void setAdjusts(float speed, float health, float damage);
	void setPlayer(Player* player);
private:
	Player* mPlayer;
	EnemyData mToroData;
	EnemyData mMonsterData;
};