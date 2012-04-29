#include "Player.h"
#include "Graphics.h"
#include "Input.h"
#include "Camera.h"
#include "Weapon.h"
#include "WeaponHandler.h"

Player::Player(D3DXVECTOR3 position) 
	: SkinnedMesh("data/smith.x", position, PLAYER)
{
	mWeaponHandler = new WeaponHandler();

	mElapsed	= 0.0f;
	mJumping	= false;
	mWalkAccel	= 0.1;
	mMaxSpeed	= 1.1f;
	mHealth = 60.0f;

	setAnimation(2, 0.1f);
	setSpeedAdjust(3.2f);
	setHeightOffset(100.0f);
	mBoost = false;
}

Player::~Player()
{
	delete mWeapon;
	delete mWeaponHandler;
}

void Player::init()
{
	// mWorld is set when here.
	WeaponData data = mWeaponHandler->getData("mp5");
	mWeapon = new Weapon(data, getPosition());
	mWeapon->setAnimation(5);
	mWeapon->setWorld(getWorld());
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
	pollInput();

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
	mWeapon->update(dt);
}
	
void Player::draw()
{
	//SkinnedMesh::draw();

	D3DXVECTOR3 knifePos = gCamera->getPosition();
	knifePos += gCamera->getDirection() * 15.0f;

	mWeapon->setPosition(knifePos);
	mWeapon->setRotation(gCamera->getDirection());
	mWeapon->draw();

	char buffer[256];
	sprintf(buffer, "Ammo: %i/%i", mWeapon->getAmmo(), mWeapon->getClipSize());
	gGraphics->drawText(buffer, 50, 200, GREEN);

	sprintf(buffer, "Health: %f/%f", mHealth, 100.0f);
	gGraphics->drawText(buffer, 50, 300, mHealth <= 0 ? RED: GREEN);
	//drawDebug();
}

void Player::attacked(float damage)
{
	mHealth -= damage;
}

void Player::pollInput()
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

	// Shooting?
	if(gInput->keyDown(VK_LBUTTON)) 
		mWeapon->triggerDown();
	else if(gInput->keyReleased(VK_LBUTTON)) 
		mWeapon->triggerReleased();

	if(gInput->keyPressed('R')) 
		mWeapon->reload();
}

void Player::drawDebug()
{
	static char buffer[256];
	sprintf(buffer, "speed.x: %f, speed.y: %f, speed.z: %f", getVelocity().x, getVelocity().z);
	//sprintf(buffer, "speed: %f", sqrt(getVelocity().x * getVelocity().x + getVelocity().z * getVelocity().z + getVelocity().y * getVelocity().y));
	gGraphics->drawText(buffer, 40, 600, WHITE);


	float rot = atan2f(gCamera->getDirection().x, gCamera->getDirection().z);
	sprintf(buffer, "rot.y: %f", rot);
	gGraphics->drawText(buffer, 40, 700);

	sprintf(buffer, "Pos.x: %f, Pos.y: %f, Pos.z: %f", getPosition().x , getPosition().y, getPosition().z);
	gGraphics->drawText(buffer, 40,  400, GREEN);

	if(getOnGround())
		gGraphics->drawText("On ground!", 500, 400, GREEN);
	else
		gGraphics->drawText("In air!", 500, 400, GREEN);
}