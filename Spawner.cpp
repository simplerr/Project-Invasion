#include "Spawner.h"
#include "World.h"
#include "Enemy.h"
#include "EnemyHandler.h"
#include "Player.h"

Spawner::Spawner(D3DXVECTOR3 position)
	: Mesh("data/models/castle/castle.x", position, SPAWNER)
{

}
	
Spawner::~Spawner()
{
	int a = 1;
}

void Spawner::init()
{
	mToroData = getWorld()->getEnemyData("toro");
	mMonsterData = getWorld()->getEnemyData("monster");
}

void Spawner::update(float dt)
{

}
	
void Spawner::draw()
{
	
}

void Spawner::spawnObject()
{
	D3DXVECTOR3 spawnPos = getPosition();

	// Spread out around the spawn point.
	spawnPos.x += (rand() % 10) * 50;
	spawnPos.z += (rand() % 10) * 50;

	Enemy* enemy = NULL;
	if(rand() % 2 != 0)
		enemy = new Enemy(mMonsterData, spawnPos);
	else 
		enemy = new Enemy(mToroData, spawnPos);

	enemy->setTarget(mPlayer);
	mWorld->addObject(enemy);
}

void Spawner::setPlayer(Player* player)
{
	mPlayer = player;
}

void Spawner::setAdjusts(float speed, float health, float damage)
{
	// Add multipliers.
	mMonsterData.speed *= speed;
	mMonsterData.damage *= damage;
	mMonsterData.health *= health;

	mToroData.speed *= speed;
	mToroData.damage *= damage;
	mToroData.health *= health;
}