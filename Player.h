#pragma once

#include "SkinnedMesh.h"

class WeaponMesh;

class Player : public SkinnedMesh
{
public:
	Player(D3DXVECTOR3 position);
	~Player();

	void update(float dt);
	void draw();
	void pollMovement();
	void drawDebug();

	float getHeightOffset();
private:
	WeaponMesh* mKnifeMesh;
	ID3DXMesh* mesh;
	float	mElapsed;
	bool	mJumping;
	bool	mBoost;
	float	mWalkAccel;
	float	mMaxSpeed;
	float	mHeightOffset;
};