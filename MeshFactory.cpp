#include "AllocMeshHierarchy.h"
#include "MeshFactory.h"
#include "Mesh.h"
#include "SkinnedMesh.h"

MeshFactory::MeshFactory()
{
	mAllocMeshHierarchy = new AllocMeshHierarchy();
}
	
MeshFactory::~MeshFactory()
{

}

void MeshFactory::loadSkinnedMesh(string filename, LPD3DXFRAME& rootFrame, LPD3DXANIMATIONCONTROLLER& animCtrl)
{
	// Not loaded?
	if(mSkinnedMeshMap.find(filename) == mSkinnedMeshMap.end())
	{
		SkinnedMeshData data;
		HR(D3DXLoadMeshHierarchyFromX(filename.c_str(), D3DXMESH_MANAGED, gd3dDevice, mAllocMeshHierarchy, NULL, &data.rootFrame, &data.animCtrl));
		mSkinnedMeshMap[filename] = data;
	}

	rootFrame = mSkinnedMeshMap[filename].rootFrame;

	// Clone the animation controll.
	LPD3DXANIMATIONCONTROLLER ctrl = mSkinnedMeshMap[filename].animCtrl;
	ctrl->CloneAnimationController(ctrl->GetMaxNumAnimationOutputs(), ctrl->GetMaxNumAnimationSets(),
			ctrl->GetMaxNumTracks(), ctrl->GetMaxNumEvents(), &animCtrl);
}
	
void MeshFactory::loadMesh(string filename, LPD3DXMESH& mesh, vector<Material>& materials, vector<IDirect3DTexture9*>& textures)
{
	// Not loaded?
	if(mMeshMap.find(filename) == mMeshMap.end())
	{
		MeshData data;
		LoadXFile(filename, &data.mesh, data.materials, data.textures);	
		mMeshMap[filename] = data;
	}

	// Set value of the output arguments.
	mesh = mMeshMap[filename].mesh;
	materials = mMeshMap[filename].materials;
	textures = mMeshMap[filename].textures;
}