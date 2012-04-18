#pragma once
#include <string>
#include <map>
#include <vector>
#include "d3dUtil.h"
using namespace std;

class Material;
class SkinnedMesh;
class Mesh;
class AllocMeshHierarchy;

struct SkinnedMeshData
{
	D3DXFRAME* rootFrame;
	ID3DXAnimationController* animationController;
};

struct MeshData
{
	ID3DXMesh* mesh;
	vector<Material> materials;
	vector<IDirect3DTexture9*> textures;
};

class MeshFactory
{
public:
	MeshFactory();
	~MeshFactory();

	SkinnedMesh* loadSkinnedMesh(string filename, D3DXVECTOR3 position, float scale = 1.0f);
	Mesh* loadMesh(string filename, D3DXVECTOR3 position, float scale = 1.0f);
private:
	map<string, SkinnedMeshData>	mSkinnedMeshMap;
	map<string, MeshData>			mMeshMap;
	AllocMeshHierarchy*				mAllocMeshHierarchy;
};