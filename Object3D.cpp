#include "Object3D.h"
#include "Graphics.h"

Object3D::Object3D(D3DXVECTOR3 position, ObjectType type)
{
	setPosition(position);
	setScale(1.0f);
	setMaterial(Material());	
	setRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	setVelocity(D3DXVECTOR3(0, 0, 0));
	setOnGround(false);
	setHeightOffset(30.0f);
	setType(type);
	setFriction(1.0f);
	setMinimapTexture("data/white.bmp");
	mAlive = true;
	mMaterial.specularPower = 128;
}
	
Object3D::~Object3D()
{
	
}

void Object3D::init()
{

}

void Object3D::update(float dt)
{
	
}

void Object3D::draw()
{
	
}

void Object3D::updateWorldMatrix()
{
	D3DXMATRIX T, RX, RY, RZ, S;

	D3DXMatrixTranslation(&T, mPosition.x, mPosition.y, mPosition.z);
	D3DXMatrixScaling(&S, mScale.width, mScale.height, mScale.depth);
	D3DXMatrixRotationX(&RX, mRotation.x);
	D3DXMatrixRotationY(&RY, mRotation.y);
	D3DXMatrixRotationZ(&RZ, mRotation.z);

	mWorldMatrix = S*RX*RY*RZ*T;
	D3DXMatrixInverse(&mInvWorldMatrix, 0, &mWorldMatrix);
}

void Object3D::move(float dx, float dy, float dz)
{
	setPosition(getPosition() + D3DXVECTOR3(dx, dy, dz));
	updateWorldMatrix();
}

void Object3D::accelerate(float dx, float dy, float dz)
{
	mVelocity += D3DXVECTOR3(dx, dy, dz);
}

void Object3D::rotate(float dx, float dy, float dz)
{
	mRotation += D3DXVECTOR3(dx, dy, dz);
}

void Object3D::setPosition(D3DXVECTOR3 position)
{
	mPosition = position;
	updateWorldMatrix();
}

void Object3D::setVelocity(D3DXVECTOR3 velocity)
{
	mVelocity = velocity;
}
	
void Object3D::setMaterial(Material material)
{
	mMaterial = material;
}

void Object3D::setScale(Dimensions scale)
{
	mScale = scale;
	updateWorldMatrix();
}

void Object3D::setRotation(D3DXVECTOR3 rotation)
{
	mRotation = rotation;
	updateWorldMatrix();
}

void Object3D::setOnGround(bool onGround)
{
	mOnGround = onGround;
}

void Object3D::setFriction(float friction)
{
	mFriction = friction;
}

void Object3D::setHeightOffset(float heightOffset)
{
	mHeightOffset = heightOffset;
}

void Object3D::setWorld(World* world)
{
	mWorld = world;
}

void Object3D::setType(ObjectType type)
{
	mType = type;
}

void Object3D::setId(int id)
{
	mId = id;
}

D3DXVECTOR3 Object3D::getPosition()
{
	return mPosition;
}

D3DXVECTOR3 Object3D::getVelocity()
{
	return mVelocity;
}

Dimensions Object3D::getScale()
{
	return mScale;
}

D3DXMATRIX Object3D::getWorldMatrix()
{
	return mWorldMatrix;
}

D3DXMATRIX Object3D::getInvWorldMatrix()
{
	return mInvWorldMatrix;
}

D3DXVECTOR3 Object3D::getRotation()
{
	return mRotation;
}

vector<IDirect3DTexture9*>* Object3D::getTextures()
{
	return &mTextures;
}

AABB Object3D::getAABB()
{
	return mAABB;
}

bool Object3D::getOnGround()
{
	return mOnGround;
}

float Object3D::getHeightOffset()
{
	return mHeightOffset;
}

void Object3D::kill()
{
	mAlive = false;
}

bool Object3D::getAlive()
{
	return mAlive;
}

World* Object3D::getWorld()
{
	return mWorld;
}

ObjectType Object3D::getType()
{
	return mType;
}

int Object3D::getId()
{
	return mId;
}

float Object3D::getFriction()
{
	return mFriction;
}

void Object3D::setMinimapTexture(string source)
{
	mMinimapIcon = gGraphics->loadTexture(source);
}
	
IDirect3DTexture9* Object3D::getMinimapTexture()
{
	return mMinimapIcon;
}