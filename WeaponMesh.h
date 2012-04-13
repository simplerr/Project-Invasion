#pragma once
#include "SkinnedMesh.h"

class WeaponMesh : public SkinnedMesh
{
public:
	WeaponMesh(string filename, D3DXVECTOR3 position, float scale = 1.0f);
	~WeaponMesh();

	void updateWorldMatrix();
private:
	// Damage etc..
};