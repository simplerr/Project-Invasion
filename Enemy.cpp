#include "Enemy.h"
#include "Graphics.h"
#include "Camera.h"

Enemy::Enemy(string filename, D3DXVECTOR3 position)
	: SkinnedMesh(filename, position)
{
	setAnimation(2);
	mTarget = NULL;
	mHealth = 100.0f;
	mVisionRange = 1000.0f;
	mChasing = false;
	mIdling = false;
	mTargetOffset = 0.0f;
}

Enemy::~Enemy()
{

}

void Enemy::update(float dt)
{
	// Update the skinned mesh.
	SkinnedMesh::update(dt);

	// Will be used later.
	D3DXVECTOR3 direction = mTarget->getPosition() - getPosition();
	float distance = sqrt(direction.x * direction.x + direction.z * direction.z);

	// Chasing the target.
	if(mChasing)
	{
		if(distance <= mVisionRange)
			mTargetPosition = mTarget->getPosition();
		else
			mTargetPosition = calculateIdlePosition();
	}
	// Not chasing.
	else if(!mChasing)
	{
		// Generate new target offset
		if(distance <= mVisionRange)
			mTargetOffset = rand() % 200 - 100;
	}

	// Set the correct animation and mChasing.
	if(distance > mVisionRange) {
		mChasing = false;
		D3DXVECTOR3 pos = getPosition();
		mTargetPosition.y = getPosition().y;
		if(equals(getPosition(), mTargetPosition, 1.0f)) {
			setAnimation(2);
			mIdling = true;
			setVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
	else {
		mIdling = false;
		mChasing = true;
		setAnimation(0);
	}

	// Set the new velocity.
	D3DXVECTOR3 dir = calculateChasingDirection() * 1.0f * !mIdling;
	dir.y = getVelocity().y;
	setVelocity(dir);

	// Set facing direction in the velocitys direction.
	D3DXVECTOR3 velocity = getVelocity();
	D3DXVec3Normalize(&velocity, &velocity);
	setRotation(D3DXVECTOR3(0.0f, atan2f(velocity.x, velocity.z), 0));
}
	
void Enemy::draw()
{
	SkinnedMesh::draw();
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