#include "Player.h"
#include "Graphics.h"
#include "Input.h"
#include "Camera.h"
#include "Weapon.h"
#include "WeaponHandler.h"
#include "SkillHandler.h"
#include "Skill.h"
#include "Leap.h"
#include "IronArmor.h"
#include "Sound.h"

Player::Player(D3DXVECTOR3 position) 
	: SkinnedMesh("data/models/smith/smith.x", position, PLAYER)
{
	mElapsed	= 0.0f;
	mJumping	= false;
	mWalkAccel	= 0.5;
	mMaxSpeed	= 30.0f;
	mHealth = 100.0f;
	mEnergy = 100.0f;
	mArmor = 0.0f;

	setAnimation(2, 0.1f);
	setSpeedAdjust(3.2f);
	setHeightOffset(100.0f);
	mBoost = false;
	mDrawModel = false;

	setMinimapTexture("data/player_icon.png");
	mRedTexture = gGraphics->loadTexture("data/red_transparent.png");
	mDeltaHitTime = 100;
}

Player::~Player()
{
	delete mWeapon;
}

void Player::init()
{
	// mWorld is set when here.
	WeaponData data = gWeaponHandler->getData("mp5");
	mWeapon = new Weapon(data, getPosition());
	mWeapon->setAnimation(5);
	mWeapon->setWorld(getWorld());
}

void Player::update(float dt)
{
	mDeltaHitTime += dt;

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
	if(getVelocity().y < -34.0f)
		setVelocity(D3DXVECTOR3(getVelocity().x, -34.0f, getVelocity().z));

	// Set the camera position.
	gCamera->setPosition(getPosition());

	// Update the camera.
	gCamera->update(dt);

	setRotation(D3DXVECTOR3(0.0f, atan2f(gCamera->getDirection().x, gCamera->getDirection().z), 0));

	SkinnedMesh::update(dt);
	mWeapon->update(dt);
}
	
void Player::draw()
{
	// Update the view matrix of the camera.
	//gCamera->updateView();

	if(mDrawModel)
		SkinnedMesh::draw();

	D3DXVECTOR3 knifePos = gCamera->getPosition();
	knifePos += gCamera->getDirection() * 15.0f;

	mWeapon->setPosition(knifePos);
	mWeapon->setRotation(gCamera->getDirection());

	// Don't draw weapon if dead.
	if(mWeapon->getVisible())
		mWeapon->draw();

	if(mDeltaHitTime < 0.17f)
		gGraphics->drawScreenTexture(mRedTexture, 600, 400, 1200, 800);

	//gCamera->drawDebug();
	//drawDebug();
}

void Player::reset()
{
	mEnergy = 100.0f;
	mHealth = 100.0f;
	mWeapon->setAmmo(100);
	mWeapon->setVisible(true);
	mDeltaHitTime = 99;
	mDrawModel = false;
}

void Player::attacked(float damage)
{
	mHealth -= max(damage - mArmor, 0);
	mHealth = max(0, mHealth);

	int num = rand() % 3;
	if(num == 0)
		gSound->playEffect("data/sound/hit1.wav", 0.9f);
	if(num == 1)
		gSound->playEffect("data/sound/hit2.wav", 0.9f);
	if(num == 2)
		gSound->playEffect("data/sound/hit3.wav", 0.9f);

	// Dead.
	if(mHealth <= 0) {
		gSound->playEffect("data/sound/humiliation.wav");
		//gCamera->setPosition(getPosition() + D3DXVECTOR3(0, 2000, 0));
		//gCamera->setDirection(D3DXVECTOR3(0, -1, 0));
	}

	mDeltaHitTime = 0.0f;
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
		accelerate(0, 20.5f, 0);
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

void Player::setArmor(float armor)
{
	mArmor = armor;
}

void Player::setEnergy(float energy)
{
	mEnergy = energy;
	mEnergy = min(mEnergy, 100);
}

int Player::getAmmo()
{
	return mWeapon->getAmmo();
}

int Player::getHealth()
{
	return mHealth;
}

int Player::getEnergy()
{
	return mEnergy;
}

void Player::useEnergy(int usedEnergy)
{
	mEnergy -=  usedEnergy;
	mEnergy = max(mEnergy, 0);
}

void Player::toggleWeapon()
{
	mWeapon->setVisible(!mWeapon->getVisible());
	mDrawModel = !mDrawModel;
	setAnimation(2);
}