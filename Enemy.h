#pragma once
#include "SkinnedMesh.h"
#include "EnemyHandler.h"

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
	Enemy(EnemyData data, D3DXVECTOR3 position);
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
	EnemyData mData;
	float mTargetOffset;
	float mDeathTimer;
	float mAttackTimer;
	float mPatrolTimer;
};