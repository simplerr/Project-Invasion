#pragma once
#include "SkinnedMesh.h"

class Enemy : public SkinnedMesh
{
public:
	Enemy(D3DXVECTOR3 position);
	~Enemy();

	void update(float dt);
	void draw();
private:
	float mHealth;
};