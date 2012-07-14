#pragma once

#include "SkinnedMesh.h"

class WeaponHandler;
class Weapon;
class Skill;
class SkillHandler;

class Player : public SkinnedMesh
{
public:
	Player(D3DXVECTOR3 position);
	~Player();

	void update(float dt);
	void draw();
	void init();
	void reset();
	void attacked(float damage);
	void pollInput();
	void drawDebug();
	int getAmmo();
	int getHealth();
	int getEnergy();

	void toggleWeapon();
	void useEnergy(int usedEnergy);

	void setArmor(float armor);
	void setEnergy(float energy);
private:
	Weapon* mWeapon;
	IDirect3DTexture9* mRedTexture;
	float	mElapsed;
	bool	mJumping;
	bool	mBoost;
	bool	mDrawModel;
	float	mWalkAccel;
	float	mMaxSpeed;
	float	mHealth;
	float	mArmor;
	float	mEnergy;
	float	mDeltaHitTime;
};