#include "Enemy.h"
#include "Graphics.h"
#include "Camera.h"
#include "World.h"
#include "BloodPSystem.h"

#define MAX_SPEED_ADJUST 1.5f

Enemy::Enemy(EnemyData data, D3DXVECTOR3 position)
	: SkinnedMesh(data.filename, position, ENEMY)
{
	setAnimation(4);
	mData = data;
	mTarget = NULL;
	mTargetOffset = 0.0f;
	mDeathTimer = -1;
	mActionState = AS_IDLING;
	mAttackTimer = 0.0f;
	mPatrolTimer = 0.0f;
	setSpeedAdjust(min(data.speed, MAX_SPEED_ADJUST));
	setMinimapTexture("data/enemy_icon.png");
}

Enemy::~Enemy()
{

}

void Enemy::update(float dt)
{
	// Update the skinned mesh.
	SkinnedMesh::update(dt);

	mAttackTimer += dt;
	mPatrolTimer += dt;

	// Remove fromt the world.
	if(mDeathTimer > 1.0f) 
		kill();

	// Update the death timer.
	if(mDeathTimer != -1)
	{
		mDeathTimer += dt;
		return;
	}

	// Will be used later.
	D3DXVECTOR3 direction = mTarget->getPosition() - getPosition();
	float distance = sqrt(direction.x * direction.x + direction.z * direction.z);

	// Chasing the target.
	if(mActionState == AS_CHASING)
	{
		if(distance <= mData.visionRange)
			mTargetPosition = mTarget->getPosition();
		else 
		{
			mTargetPosition = calculateIdlePosition();
			mActionState = AS_PATROLLING;
		}
	}
	// Not chasing, Randomize patrolling each 1 second.
	else if(mActionState == AS_IDLING && mPatrolTimer > 1.0f)
	{
		// Randomize patrolling.
		if(rand() % 3 == 1) {
			mTargetPosition = calculateIdlePosition();
			mActionState = AS_PATROLLING;
			setAnimation(2);
			setSpeedAdjust(min(mData.speed, MAX_SPEED_ADJUST));
		}
		mPatrolTimer = 0.0f;
	}
	else
	{
		// Generate new target offset.
		if(distance <= mData.visionRange)
			mTargetOffset = rand() % 200 - 100;
	}

	// Set the correct animation and mChasing.
	if(distance > mData.visionRange) 
	{
		D3DXVECTOR3 pos = getPosition();
		mTargetPosition.y = getPosition().y;

		// Reached the target position?
		if(equals(getPosition(), mTargetPosition, 1.0f))
		{
			mActionState = AS_IDLING;
			setAnimation(4);
			setVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			setSpeedAdjust(1.0f);
		}
	}
	else
	{
		setAnimation(2);
		setSpeedAdjust(min(mData.speed, MAX_SPEED_ADJUST));
		mActionState = AS_CHASING;
	}

	// In attack range?
	if(distance < mData.attackRange) 
	{
		setAnimation(0);
		setSpeedAdjust(1.0f);
		mActionState = AS_ATTACKING;

		// Ready to attack?
		if(mAttackTimer >= mData.attackRate && equals(getVelocity(), D3DXVECTOR3(0, getVelocity().y, 0)))
		{
			mTarget->attacked(mData.damage);
			mAttackTimer = 0.0f;
		}
	}

	// Set the new velocity.
	D3DXVECTOR3 dir = calculateChasingDirection() * (mActionState == AS_CHASING || mActionState == AS_PATROLLING);
	dir.y = getVelocity().y;

	setVelocity(dir*mData.speed);

	// Set facing direction in the velocitys direction.
	D3DXVECTOR3 velocity = getVelocity();
	D3DXVec3Normalize(&velocity, &velocity);

	// Doesn't rotate in velocity direction if really close to player.
	if(mActionState != AS_ATTACKING && distance > mData.attackRange*1.5)	
		setRotation(D3DXVECTOR3(0.0f, atan2f(velocity.x, velocity.z), 0));
	else 
		setRotation(D3DXVECTOR3(0.0f, atan2f(direction.x, direction.z), 0));
}
	
void Enemy::draw()
{
	SkinnedMesh::draw();
}

void Enemy::attacked(float damage)
{
	mData.health -= damage;

	// Dead?
	if(mData.health <= 0 && mDeathTimer == -1)
	{
		// Set death animation and start the death timer.
		setAnimation(1);
		mDeathTimer = 0;
	}

	// Add blood effect.
	BloodPSystem* bloodEffect = new BloodPSystem(getPosition(), "particle.fx", "ParticleTech", 
	"smoke.dds", D3DXVECTOR3(-3.0f, -9.8f, 0.0f), AABB(), 200, 0.02f);
	bloodEffect->setLifetime(0.30f);
	getWorld()->addObject(bloodEffect);
}

D3DXVECTOR3 Enemy::calculateChasingDirection()
{
	D3DXVECTOR3 direction = mTargetPosition - getPosition();
	D3DXVec3Normalize(&direction, &direction);
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &D3DXVECTOR3(0, 1, 0), &direction);
	D3DXVec3Normalize(&right, &right);

	direction = mTargetPosition + right * mTargetOffset - getPosition();
	D3DXVec3Normalize(&direction, &direction);
	return direction;
}
	
D3DXVECTOR3 Enemy::calculateIdlePosition()
{
	mTargetOffset = 0.0f;
	int radius = 2000;
	D3DXVECTOR3 pos;
	pos.x = getPosition().x + rand() % radius - radius/2;
	pos.z = getPosition().z + rand() % radius - radius/2;
	pos.y = getPosition().y;

	return pos;
}

void Enemy::setTarget(Object3D* target)
{
	mTarget = target;
	mTargetPosition = mTarget->getPosition();
} 