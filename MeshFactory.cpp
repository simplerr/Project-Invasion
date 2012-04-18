#include "AllocMeshHierarchy.h"
#include "MeshFactory.h"
#include "Mesh.h"
#include "SkinnedMesh.h"

MeshFactory::MeshFactory()
{
	AllocMeshHierarchy* mAllocMeshHierarchy = new AllocMeshHierarchy();
}
	
MeshFactory::~MeshFactory()
{

}

SkinnedMesh* MeshFactory::loadSkinnedMesh(string filename, D3DXVECTOR3 position, float scale)
{
	// Already loaded?
	// - Yes
	if(mSkinnedMeshMap.find(filename) != mSkinnedMeshMap.end())
	{
		SkinnedMesh* mesh = new SkinnedMesh(position, mSkinnedMeshMap[filename].rootFrame, mSkinnedMeshMap[filename].animationController, scale);
		return mesh;
	}
	// - No
	else
	{
		SkinnedMeshData data;
		HR(D3DXLoadMeshHierarchyFromX(filename.c_str(), D3DXMESH_MANAGED, gd3dDevice, mAllocMeshHierarchy, NULL, &data.rootFrame, &data.animationController));
		mSkinnedMeshMap[filename] = data;
		SkinnedMesh* mesh = new SkinnedMesh(position, data.rootFrame, data.animationController, scale);
		return mesh;
	}
}
	
Mesh* MeshFactory::loadMesh(string filename, D3DXVECTOR3 position, float scale)
{
	return NULL;
}