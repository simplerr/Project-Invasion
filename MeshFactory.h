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
	ID3DXAnimationController* animCtrl;
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

	void loadSkinnedMesh(string filename, LPD3DXFRAME& rootFrame, LPD3DXANIMATIONCONTROLLER& animCtrl);
	void loadMesh(string filename, LPD3DXMESH& mesh, vector<Material>& materials, vector<IDirect3DTexture9*>& textures);
private:
	map<string, SkinnedMeshData>	mSkinnedMeshMap;
	map<string, MeshData>			mMeshMap;
	AllocMeshHierarchy*				mAllocMeshHierarchy;
};

extern MeshFactory* gMeshFactory;