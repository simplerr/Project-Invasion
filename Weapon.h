#pragma once
#include "SkinnedMesh.h"
#include "WeaponHandler.h"

class Weapon : public SkinnedMesh
{
public:
	Weapon(string filename, D3DXVECTOR3 position, float scale = 1.0f);
	Weapon(WeaponData data, D3DXVECTOR3 position, float scale = 1.0f);
	~Weapon();

	void update(float dt);
	void draw();
	void reload();
	
	int getClipSize();
	int getAmmo();

	virtual void triggerDown();
	virtual void triggerPressed();
	virtual void triggerReleased();

	void setData(WeaponData data);
private:
	void updateWorldMatrix();
private:
	WeaponData mData;
	float mDeltaTime;
	float mReloadTime;
	float mReloadElapsed;
	int	mAmmo;
	int counter;
	bool mRealoading;
};