#include "Player.h"
#include "Graphics.h"
#include "Input.h"
#include "Camera.h"
#include "WeaponMesh.h"

Player::Player(D3DXVECTOR3 position) 
	: SkinnedMesh("data/smith.x", position)
{
	mKnifeMesh = new WeaponMesh("data/reference.x", position);
	mKnifeMesh->setSpeedAdjust(1.20f);
	mElapsed	= 0.0f;
	mJumping	= false;
	mWalkAccel	= 0.1;
	mMaxSpeed	= 1.1f;
	mHeightOffset = 30.0f;

	setAnimation(2, 0.1f);
	setSpeedAdjust(3.2f);
	mBoost = false;
}

Player::~Player()
{

}

void Player::update(float dt)
{
	// Walking or running.
	if(getVelocity().x != 0 || getVelocity().y != 0 && getOnGround()) {
		float speed = sqrt(	getVelocity().x * getVelocity().x + getVelocity().z * getVelocity().z);
		setSpeedAdjust(1.7f);
	}
	else
		setSpeedAdjust(1.0f);

	if(mJumping) {
		mElapsed += dt;
		if(mElapsed > 0.4f) {
			setAnimation(2, 0.80f);
			mJumping = false;
		}
	}

	// Get keypresses from the user.
	pollMovement();

	// Standing still? Check if not jumping!
	if(abs(getVelocity().x) <= 0.0001 && abs(getVelocity().z) <= 0.0001 && !mJumping)
		setAnimation(2, 0.5f);

	// Limit the fall speed.
	if(getVelocity().y < -7.0f)
		setVelocity(D3DXVECTOR3(getVelocity().x, -7.0f, getVelocity().z));

	// Set the camera position.
	gCamera->setPosition(getPosition());// + D3DXVECTOR3(25, 100, 0) - gCamera->getDirection()*200);

	setRotation(D3DXVECTOR3(0.0f, atan2f(gCamera->getDirection().x, gCamera->getDirection().z), 0));

	SkinnedMesh::update(dt);
	mKnifeMesh->update(dt);
}
	
void Player::draw()
{
	//gGraphics->drawSkinnedMesh(this);

	D3DXVECTOR3 knifePos = gCamera->getPosition();
	knifePos += gCamera->getDirection() * 15.0f;

	mKnifeMesh->setPosition(knifePos);
	mKnifeMesh->setRotation(gCamera->getDirection());

	gGraphics->drawSkinnedMesh(mKnifeMesh);

	//drawDebug();
}

void Player::pollMovement()
{
	// Movement.
	D3DXVECTOR3 nv = mVelocity;
	D3DXVECTOR3 dir = gCamera->getDirection();
	dir.y = 0.0f;

	D3DXVec3Normalize(&dir, &dir);

	if(gInput->keyDown('W')) 
		nv += dir * mWalkAccel;
	if(gInput->keyDown('S')) 
		nv += -dir * mWalkAccel;
	if(gInput->keyDown('A')) 
		nv += -gCamera->getRight() * mWalkAccel;
	if(gInput->keyDown('D')) 
		nv += gCamera->getRight()  * mWalkAccel;
	if(gInput->keyDown('F')) {
		nv += dir * mWalkAccel * 10.0f;
		mBoost = true;
	}
	if(gInput->keyReleased('F'))
		mBoost = false;

	// Set walking animation.
	if(gInput->keyDown('W') || gInput->keyDown('A') || gInput->keyDown('S') || gInput->keyDown('D'))
		setAnimation(1, 0.5f);

	// Below the maximum speed?
	if(!mBoost) {
		if(sqrt(nv.x * nv.x + nv.z * nv.z) < mMaxSpeed)
			mVelocity = nv;
	}
	else {
		if(sqrt(nv.x * nv.x + nv.z * nv.z) < mMaxSpeed+10.0f)
			mVelocity = nv;
	}


	// Jumping.
	if(gInput->keyDown(VK_SPACE) && getOnGround())
	{
		setAnimation(0, 0.0f);
		accelerate(0, 2.5f, 0);
		mJumping = true;
		mElapsed = 0.0f;
	}
}

void Player::drawDebug()
{
	char buffer[256];
	sprintf(buffer, "speed.x: %f, speed.y: %f, speed.z: %f", getVelocity().x, getVelocity().z);
	//sprintf(buffer, "speed: %f", sqrt(getVelocity().x * getVelocity().x + getVelocity().z * getVelocity().z + getVelocity().y * getVelocity().y));
	gGraphics->drawText(buffer, 40, 600, WHITE);


	float rot = atan2f(gCamera->getDirection().x, gCamera->getDirection().z);
	sprintf(buffer, "rot.y: %f", rot);
	gGraphics->drawText(buffer, 40, 700);

	if(getOnGround())
		gGraphics->drawText("On ground!", 500, 400, GREEN);
	else
		gGraphics->drawText("In air!", 500, 400, GREEN);
}

float Player::getHeightOffset()
{
	return mHeightOffset;
}