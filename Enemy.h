#pragma once
#include "SkinnedMesh.h"

class Enemy : public SkinnedMesh
{
public:
	Enemy(string filename, D3DXVECTOR3 position);
	~Enemy();

	void update(float dt);
	void draw();
	void attacked();

	void setTarget(Object3D* target);
private:
	D3DXVECTOR3 calculateChasingDirection();
	D3DXVECTOR3 calculateIdlePosition();

	Object3D* mTarget;
	D3DXVECTOR3 mTargetPosition;
	bool mChasing;
	bool mIdling;
	float mHealth;
	float mVisionRange;
	float mTargetOffset;
	float mDeathTimer;
};