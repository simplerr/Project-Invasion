#pragma once

#include "d3dUtil.h"
#include "Material.h"

enum LightType 
{
	AMBIENT,
	DIFFUSE_SPECULAR
};

class Light
{
public:
	Light(){};
	Light(D3DXVECTOR3 position, D3DXVECTOR3 direction, Material material = Material(BLACK, WHITE, WHITE), float spotPower = 0.0f, float length = 100.0f);
	~Light();

	void setMaterial(Material material);
	void setPosition(D3DXVECTOR3 position);
	void setDirection(D3DXVECTOR3 direction);
	void setSpotPower(float power);
	void setLength(float length);

	D3DXVECTOR3 getPosition();
	D3DXVECTOR3 getDirection();
	float		getSpotPower();
	float		getLength();

	Material	mMaterial;

	// Align the data in 16 byte packages
	__declspec(align(16)) D3DXVECTOR3 mPosition;
	__declspec(align(16)) D3DXVECTOR3 mDirection;
	__declspec(align(16)) float mSpotPower;
	__declspec(align(16)) float mLength;
};