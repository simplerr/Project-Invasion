#pragma once
#include <string>
#include <map>
#include "d3dUtil.h"
#include "Material.h"
using namespace std;

class BufferFactory;
class Terrain;
class SkinnedMesh;
class Mesh;
class Object3D;
struct Material;
struct Dimensions;

class Graphics
{
public:
	Graphics();
	~Graphics();

	void initBuffers();
	void initEffect();
	void initFonts();

	IDirect3DTexture9* loadTexture(string filename, DWORD colorKey = 0);

	void setEffectParameters(D3DXMATRIX world); 
	void setEffectData(IDirect3DTexture9* texture, D3DXVECTOR3 position, Dimensions dimensions, Material material, float rotation = 0.0f, D3DXMATRIX m = D3DXMATRIX(), bool useM = false);

	void drawBoundingBox(Object3D* object, D3DXCOLOR color, float opacity = 0.5f);
	void drawBoundingBox(D3DXVECTOR3 position, Dimensions dimensions, D3DXCOLOR color = WHITE, float opacity = 0.5f);

	void drawTexturedCube(IDirect3DTexture9* texture, D3DXVECTOR3 position, Dimensions dimensions, Material material = Material(), float rotation = 0.0f, bool solid = true, D3DXMATRIX m = D3DXMATRIX());
	void drawSkinnedMesh(SkinnedMesh* skinnedMesh);
	void drawTerrain(Terrain* terrain);
	void drawMesh(Mesh* mesh);
	void drawRay(D3DXVECTOR3 start, D3DXVECTOR3 direction, float length, float width);

	void onLostDevice();
	void onResetDevice();

	// OLD AND UGLY
	void drawText(string text, int x, int y, DWORD color = 0xff000000);
	void drawTest(ID3DXMesh* mesh, IDirect3DTexture9* texture, D3DXVECTOR3 position, D3DXVECTOR3 rotation);
public:
	// Texture map
	map<string, IDirect3DTexture9*> mTextureMap;

	// Buffer factory
	BufferFactory* mBufferFactory;

	// Colored Cube buffers
	IDirect3DVertexBuffer9* mCubeVB;
	IDirect3DIndexBuffer9*	mCubeIB;

	// Textured Cube buffers
	IDirect3DVertexBuffer9* mTexturedCubeVB;
	IDirect3DIndexBuffer9* mTexturedCubeIB;

	// Textured cube effect handlers
	ID3DXEffect* mFX;
	D3DXHANDLE   mhTexTech;
	D3DXHANDLE   mhWVP;
	D3DXHANDLE   mhTex;
	D3DXHANDLE   mhWorldInverseTranspose;
	D3DXHANDLE   mhWorld;
	D3DXHANDLE	 mhEyePos;
	D3DXHANDLE	 mhObjectMaterial;
	D3DXHANDLE	 mhLights;
	D3DXHANDLE   mhNumLights;
	
	// Terrain effect handlers
	D3DXHANDLE   mhTerrainTech;
	D3DXHANDLE   mhTex0;
	D3DXHANDLE   mhTex1;
	D3DXHANDLE   mhTex2;
	D3DXHANDLE   mhBlendMap;

	// Skinned mesh effect handlers
	D3DXHANDLE   mhSkinnedMeshTech;
	D3DXHANDLE   mhFinalXForms;
	D3DXHANDLE	 mhBoneTransform;

	IDirect3DTexture9* mTex0;
	IDirect3DTexture9* mTex1;
	IDirect3DTexture9* mTex2;
	IDirect3DTexture9* mBlendMap;
	IDirect3DTexture9* mWhiteTexture;

	ID3DXFont* mFont;
	ID3DXMesh* mRayMesh;
};

extern Graphics* gGraphics;