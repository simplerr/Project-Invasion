#include "Light.h"

Light::Light(D3DXVECTOR3 position, D3DXVECTOR3 direction, Material material, float spotPower, float length)
{
	setPosition(position);
	setDirection(direction);
	setMaterial(material);
	mSpotPower = spotPower;
	mLength = length;
	mMaterial.ambient = D3DCOLOR_ARGB(0, 0, 0, 0);
	mMaterial.specular = D3DCOLOR_ARGB(100, 100, 100, 100);
}

Light::~Light()
{

}

void Light::setPosition(D3DXVECTOR3 position)
{
	mPosition = position;
}
	
void Light::setDirection(D3DXVECTOR3 direction)
{
	mDirection = direction;
}

void Light::setMaterial(Material material)
{
	mMaterial = material;
}

D3DXVECTOR3 Light::getPosition()
{
	return mPosition;
}

D3DXVECTOR3 Light::getDirection()
{
	return mDirection;
}
	
float Light::getSpotPower()
{
	return mSpotPower;
}

float Light::getLength()
{
	return mLength;
}

void Light::setLength(float length)
{
	mLength = length;
}

void Light::setSpotPower(float power)
{
	mSpotPower = power;
}