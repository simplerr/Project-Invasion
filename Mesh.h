#pragma once

#include <string>
#include <vector>
#include "Object3D.h"

using namespace std;

class Mesh : public Object3D
{
public:
	Mesh(string filename, D3DXVECTOR3 position, float scale = 1.0f);
	Mesh(D3DXVECTOR3 position);
	~Mesh();

	void draw();

	void loadFromXFile(string filename);

	bool rayIntersectAABB(D3DXVECTOR3 position, D3DXVECTOR3 direction);
	bool rayIntersectMesh(D3DXVECTOR3 position, D3DXVECTOR3 direction, DWORD& faceIndex, float& distance);

	const vector<Material>& getMaterials();

	ID3DXMesh*	getMesh();
	AABB		getAABB();
//private:
	ID3DXMesh*			mMesh;
	vector<Material>	mMaterials;
	bool				mFromFile;
};