#pragma once

#include <string>
#include "d3dUtil.h"
#include "Material.h"
#include "AABB.h"
using namespace std;

//! Base class for all Object3Ds.
class Object3D
{
public:
	Object3D(D3DXVECTOR3 position, Dimensions scale);
	virtual ~Object3D();

	virtual void update(float dt);
	virtual void draw();
	virtual void updateWorldMatrix();

	virtual bool rayIntersectAABB(D3DXVECTOR3 position, D3DXVECTOR3 direction) {return false;}
	virtual bool rayIntersectMesh(D3DXVECTOR3 position, D3DXVECTOR3 direction, DWORD& faceIndex, float& distance) {return false;}

	void accelerate(float dx, float dy, float dz);
	void move(float dx, float dy, float dz);

	void setPosition(D3DXVECTOR3 position);
	void setVelocity(D3DXVECTOR3 velocity);
	void setRotation(D3DXVECTOR3 rotation);
	void setMaterial(Material material);
	void setScale(Dimensions dimensions);
	void setOnGround(bool onGround);
	void setHeightOffset(float heightOffset);

	Dimensions	getScale();
	D3DXVECTOR3 getPosition();
	D3DXVECTOR3 getVelocity();
	D3DXVECTOR3 getRotation();
	D3DXMATRIX	getWorldMatrix();
	D3DXMATRIX	getInvWorldMatrix();
	AABB		getAABB();
	bool		getOnGround();
	float		getHeightOffset();
	vector<IDirect3DTexture9*>* getTextures();
protected:
	vector<IDirect3DTexture9*>	mTextures;
	D3DXMATRIX			mWorldMatrix;
	D3DXMATRIX			mInvWorldMatrix;
//private:
	D3DXVECTOR3			mPosition;
	D3DXVECTOR3			mVelocity;
	D3DXVECTOR3			mRotation;

	Material			mMaterial;
	Dimensions			mScale;
	AABB				mAABB;
	bool				mOnGround;
	float				mHeightOffset;
};