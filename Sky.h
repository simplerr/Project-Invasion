#pragma once

#include <d3dx9.h>
#include <string>

class Sky
{
public:
	Sky(const std::string& envmapFilename, float skyRadius);
	~Sky();

	IDirect3DCubeTexture9* getEnvMap();
	float getRadius();

	DWORD getNumTriangles();
	DWORD getNumVertices();

	void onLostDevice();
	void onResetDevice();

	void draw();

private:
	ID3DXMesh* mSphere;
	float mRadius;
	IDirect3DCubeTexture9* mEnvMap;
	ID3DXEffect* mFX;
	D3DXHANDLE mhTech;
	D3DXHANDLE mhEnvMap;
	D3DXHANDLE mhWVP;
};