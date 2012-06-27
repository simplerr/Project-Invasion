#pragma once

#include <string>
#include "d3dUtil.h"
#include "Material.h"
#include "AABB.h"
using namespace std;

class World;

enum ObjectType
{
	PLAYER,
	ENEMY,
	PROP,
	WEAPON,
	TERRAIN,
	PARTICLE_SYSTEM,
	SPAWNER,
	POWERUP,
	ENERGY_POWERUP
};

//! Base class for all Object3Ds.
class Object3D
{
public:
	Object3D(D3DXVECTOR3 position, ObjectType type);
	virtual ~Object3D();

	virtual void update(float dt);
	virtual void draw();
	virtual void updateWorldMatrix();
	virtual void init();	// Gets called after the world is set.

	virtual void onLostDevice() {};
	virtual void onResetDevice() {};

	virtual void attacked(float damage) {};
	virtual bool rayIntersectAABB(D3DXVECTOR3 position, D3DXVECTOR3 direction) {return false;}
	virtual bool rayIntersectMesh(D3DXVECTOR3 position, D3DXVECTOR3 direction, DWORD& faceIndex, float& distance) {return false;}

	void accelerate(float dx, float dy, float dz);
	void move(float dx, float dy, float dz);
	void rotate(float dx, float dy, float dz);

	void setPosition(D3DXVECTOR3 position);
	void setVelocity(D3DXVECTOR3 velocity);
	void setRotation(D3DXVECTOR3 rotation);
	void setMaterial(Material material);
	void setScale(Dimensions dimensions);
	void setOnGround(bool onGround);
	void setFriction(float friction);
	void setHeightOffset(float heightOffset);
	void setWorld(World* world);
	void setType(ObjectType type);
	void setId(int id);
	void kill();

	Dimensions	getScale();
	D3DXVECTOR3 getPosition();
	D3DXVECTOR3 getVelocity();
	D3DXVECTOR3 getRotation();
	D3DXMATRIX	getWorldMatrix();
	D3DXMATRIX	getInvWorldMatrix();
	ObjectType  getType();
	AABB		getAABB();
	bool		getOnGround();
	bool		getAlive();
	float		getFriction();
	float		getHeightOffset();
	int			getId();
	vector<IDirect3DTexture9*>* getTextures();
	World*		getWorld();
protected:
	vector<IDirect3DTexture9*>	mTextures;
	World*				mWorld;
	D3DXMATRIX			mWorldMatrix;
	D3DXMATRIX			mInvWorldMatrix;
//private:
	D3DXVECTOR3			mPosition;
	D3DXVECTOR3			mVelocity;
	D3DXVECTOR3			mRotation;

	Material			mMaterial;
	Dimensions			mScale;
	AABB				mAABB;
	ObjectType			mType;
	bool				mOnGround;
	bool				mAlive;
	float				mHeightOffset;
	float				mFriction;
	int					mId;
};