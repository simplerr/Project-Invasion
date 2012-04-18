#pragma once
#include <vector>
#include "d3dUtil.h"
#include "Heightmap.h"
#include "Material.h"
using namespace std;

class Mesh;
class Graphics;

class Terrain
{
public:
	Terrain(D3DXVECTOR3 center, int rows, int colums, float dx, float dz, float heightScale = 1.0f);
	~Terrain();

	void build(D3DXVECTOR3 center, int rows, int colums, float dx, float dz, vector<D3DXVECTOR3>& verts, vector<DWORD>& indices);
	void draw();

	void smothOut(int x, int z, int radius);

	float					getHeight(float x, float z);
	Mesh*					getMesh();
	D3DXVECTOR3				getPosition();
	Material				getMaterial();
	int getVertices();
	int getTriangles();
	int getColums();
	int getRows();
private:
	Mesh*					mTerrainMesh;
	Heightmap*				mHeightmap;
	D3DXVECTOR3				mPosition;
	Material				mMaterial;

	int mColums, mRows;

	float mWidth;
	float mDepth;
	float mDX;
	float mDZ;

	int mNumVertices, mNumTriangles;
};