#pragma once

#include <string>
#include "d3dUtil.h"
#include "Material.h"
using namespace std;

class Effect
{
public:
	Effect(string effect, string tech);
	~Effect();
	
	void setBaseData(IDirect3DTexture9* texture, D3DXMATRIX world, Material material = Material(WHITE, WHITE, WHITE)); 
private:
	ID3DXEffect* mFX;
	D3DXHANDLE mhTech;
	D3DXHANDLE mhWVP;
	D3DXHANDLE mhTex;
	D3DXHANDLE mhWorldInverseTranspose;
	D3DXHANDLE mhWorld;
	D3DXHANDLE mhEyePos;
	D3DXHANDLE mhObjectMaterial;
	D3DXHANDLE mhLights;
	D3DXHANDLE mhNumLights;
};