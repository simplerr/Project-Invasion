#include "Weapon.h"
#include "Camera.h"
#include "Input.h"
#include "World.h"
#include "Graphics.h"

Weapon::Weapon(WeaponData data, D3DXVECTOR3 position)
	: SkinnedMesh(data.filename, position, WEAPON)
{
	setData(data);
	mReloadTime = 1.0f;
	mReloadElapsed = 0.0f;
	mRealoading = false;
	mDeltaTime = 0.0f;
	counter = 0;
}

Weapon::~Weapon()
{

}

void Weapon::update(float dt)
{
	// Update the skinned mesh.
	SkinnedMesh::update(dt);
	mDeltaTime += dt;
	mReloadElapsed += dt;

	if(mReloadElapsed >= mReloadTime && mRealoading) {
		setAnimation(5);
		mRealoading = false;
		mAmmo = mData.clipSize;
		setSpeedAdjust(1/mData.rateOfFire);
	}
}
	
void Weapon::draw()
{
	SkinnedMesh::draw();

	char buffer[256];
	sprintf(buffer, "counter: %i", counter);
	gGraphics->drawText(buffer, 40, 400, GREEN);
}

void Weapon::triggerDown()
{
	// Ready to fire?
	if(mDeltaTime >= mData.rateOfFire && getAmmo() > 0 && mReloadElapsed >= mReloadTime)
	{
		// Set shooting animation.
		setAnimation(0);

		// Find out if there was an object that got hit.
		DWORD faceIndex;
		float dist;
		Object3D* intersected = getWorld()->getIntersectedObject(gCamera->getPosition(), gCamera->getDirection(), faceIndex, dist);

		// [TODO] Check if enemy.
		if(intersected != NULL) {
			intersected->attacked(mData.damage);
			counter++;
		}

		mDeltaTime = 0.0f;
		mAmmo--;
	}
	else if(getAmmo() <= 0) {
		setAnimation(5);
		// [TODO] Play no ammo sound.
	}
}
	
void Weapon::triggerPressed()
{

}
	
void Weapon::triggerReleased()
{
	setAnimation(5);
}

void Weapon::reload()
{
	if(mReloadElapsed >= mReloadTime) {
		setSpeedAdjust(2.0f);
		setAnimation(3, 0.0f);
		mReloadElapsed = 0.0f;
		mRealoading = true;
	}
}

void Weapon::updateWorldMatrix()
{
	D3DXMATRIX T, S, R1, R2;

	D3DXMatrixTranslation(&T, mPosition.x, mPosition.y, mPosition.z);

	D3DXVECTOR3 dir = getRotation();
	float rotY = atan2f(dir.x, dir.z);
	float rotXZ = dir.y * D3DX_PI/2 * 0.8;

	// -rigth for some reason :)
	D3DXMatrixRotationY(&R1, rotY);
	D3DXMatrixRotationAxis(&R2, &-gCamera->getRight(), rotXZ);
	D3DXMatrixScaling(&S, mScale.width, mScale.height, mScale.depth);

	mWorldMatrix = S*R1*R2*T;
	D3DXMatrixInverse(&mInvWorldMatrix, 0, &mWorldMatrix);
}

int Weapon::getClipSize()
{
	return mData.clipSize;;
}
	
int Weapon::getAmmo()
{
	return mAmmo;
}

void Weapon::setData(WeaponData data)
{
	mData = data;
	mAmmo = mData.clipSize;
	setSpeedAdjust((float)1/mData.rateOfFire);
}