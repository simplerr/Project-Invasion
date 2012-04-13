#pragma once

#include "d3dUtil.h"

struct AABB
{
	AABB() {
		min = D3DXVECTOR3(999999, 999999, 999999);
		max = D3DXVECTOR3(-999999, -999999, -999999);
	}

	Dimensions getDimensions() {
		Dimensions dimensions(max.x - min.x, max.y - min.y, max.z - min.z);
		return dimensions;
	}

	D3DXVECTOR3 min;
	D3DXVECTOR3 max;
};