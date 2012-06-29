#pragma once
#include "SkinnedMesh.h"
#include "WeaponHandler.h"

class Weapon : public SkinnedMesh
{
public:
	Weapon(WeaponData data, D3DXVECTOR3 position);
	virtual ~Weapon();

	void update(float dt);
	void draw();
	void reload();
	
	int getClipSize();
	int getAmmo();

	virtual void triggerDown();
	virtual void triggerPressed();
	virtual void triggerReleased();

	void setData(WeaponData data);
	void setAmmo(int ammo);
private:
	void updateWorldMatrix();
private:
	WeaponData mData;
	float mDeltaTime;
	float mReloadTime;
	float mReloadElapsed;
	int	mAmmo;
	bool mRealoading;
};