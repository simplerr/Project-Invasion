#pragma once
#include "SkinnedMesh.h"

class Enemy : public SkinnedMesh
{
public:
	Enemy(string filename, D3DXVECTOR3 position);
	~Enemy();

	void update(float dt);
	void draw();

	void setTarget(Object3D* target);
private:
	Object3D* mTarget;
	bool mChasing;
	float mHealth;
	float mVisionRange;
};