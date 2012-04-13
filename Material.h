#pragma once

#include "d3dUtil.h"

const D3DXCOLOR BLACK	= 0xff000000;
const D3DXCOLOR WHITE	= 0xffffffff;
const D3DXCOLOR RED		= 0xffff0000;
const D3DXCOLOR GREEN	= 0xff00ff00;
const D3DXCOLOR BLUE	= 0xff0000ff;

struct Material
{
	Material(D3DXCOLOR color, float power)
		: ambient(color), diffuse(color), specular(color), specularPower(8.0f){}
	Material(D3DXCOLOR a = WHITE, D3DXCOLOR d = WHITE, D3DXCOLOR s = WHITE, float power = 8.0f)
		: ambient(a), diffuse(d), specular(s), specularPower(power){}
	~Material(){};

	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR specular;
	float specularPower;
};