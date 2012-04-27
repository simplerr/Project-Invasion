#pragma once
#include "SkinnedMesh.h"

enum ActionState
{
	AS_CHASING,
	AS_IDLING,
	AS_ATTACKING,
	AS_PATROLLING
};

class Enemy : public SkinnedMesh
{
public:
	Enemy(string filename, D3DXVECTOR3 position);
	~Enemy();

	void update(float dt);
	void draw();
	void attacked(float damage);

	void setTarget(Object3D* target);
private:
	D3DXVECTOR3 calculateChasingDirection();
	D3DXVECTOR3 calculateIdlePosition();

	Object3D* mTarget;
	D3DXVECTOR3 mTargetPosition;
	ActionState mActionState;
	float mHealth;
	float mVisionRange;
	float mTargetOffset;
	float mDeathTimer;
	float mAttackRange;
	float mAttackRate;
	float mAttackTimer;
	float mDamage;
};