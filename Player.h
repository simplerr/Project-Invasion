#pragma once

#include "SkinnedMesh.h"

class WeaponHandler;
class Weapon;

class Player : public SkinnedMesh
{
public:
	Player(D3DXVECTOR3 position);
	~Player();

	void update(float dt);
	void draw();
	void init();
	void pollInput();
	void drawDebug();

	float getHeightOffset();
private:
	WeaponHandler* mWeaponHandler;
	Weapon* mWeapon;
	float	mElapsed;
	bool	mJumping;
	bool	mBoost;
	float	mWalkAccel;
	float	mMaxSpeed;
	float	mHeightOffset;
};