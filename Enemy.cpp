#include "Enemy.h"
#include "Graphics.h"
#include "Camera.h"

Enemy::Enemy(string filename, D3DXVECTOR3 position)
	: SkinnedMesh(filename, position)
{
	setTarget(NULL);
	setAnimation(2);
	mHealth = 100.0f;
	mVisionRange = 1000.0f;
	mChasing = false;
}

Enemy::~Enemy()
{

}

void Enemy::update(float dt)
{
	SkinnedMesh::update(dt);

	if(mChasing)
	{
		// Set running animation.
		D3DXVECTOR3 direction = mTarget->getPosition() - getPosition();
		D3DXVec3Normalize(&direction, &direction);

		D3DXVECTOR3 velocity = direction * 1.0f;
		velocity.y = getVelocity().y;
		setVelocity(velocity);
	}

	D3DXVECTOR3 direction = mTarget->getPosition() - getPosition();
	float distance = sqrt(direction.x * direction.x + direction.z * direction.z);
	if(distance > mVisionRange) {
		mChasing = false;
		setAnimation(2);
	}
	else {
		mChasing = true;
		setAnimation(0);
	}

	// Set facing direction to target.
	setRotation(D3DXVECTOR3(0.0f, atan2f(-gCamera->getDirection().x, -gCamera->getDirection().z), 0));
}
	
void Enemy::draw()
{
	SkinnedMesh::draw();
}

void Enemy::setTarget(Object3D* target)
{
	mTarget = target;
}