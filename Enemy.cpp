#include "Enemy.h"
#include "Graphics.h"

Enemy::Enemy(D3DXVECTOR3 position)
	: SkinnedMesh("data/smith.x", position)
{
	mHealth = 100.0f;
}

Enemy::~Enemy()
{

}

void Enemy::update(float dt)
{
	SkinnedMesh::update(dt);
}
	
void Enemy::draw()
{
	gGraphics->drawSkinnedMesh(this);
}