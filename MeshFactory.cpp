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
	for(auto iter = mSkinnedMeshMap.begin(); iter != mSkinnedMeshMap.end(); iter++) {
		SkinnedMeshData data = iter->second;
		ReleaseCOM(data.animCtrl);
		D3DXFrameDestroy(data.rootFrame, mAllocMeshHierarchy);
		//delete data.rootFrame;
	}

	for(auto iter = mMeshMap.begin(); iter != mMeshMap.end(); iter++) {
		MeshData data = iter->second;
		data.materials.clear();
		ReleaseCOM(data.mesh);
	}

	delete mAllocMeshHierarchy;
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

	//rootFrame = mSkinnedMeshMap[filename].rootFrame;
	//animCtrl = mSkinnedMeshMap[filename].animCtrl;
	//rootFrame = new D3DXFRAME();
	//memcpy(rootFrame, mSkinnedMeshMap[filename].rootFrame, sizeof(D3DXFRAME));
	rootFrame = mSkinnedMeshMap[filename].rootFrame;

	// Clone the animation controll.
	LPD3DXANIMATIONCONTROLLER ctrl = mSkinnedMeshMap[filename].animCtrl;
	ctrl->CloneAnimationController(ctrl->GetMaxNumAnimationOutputs(), ctrl->GetMaxNumAnimationSets(),
			ctrl->GetMaxNumTracks(), ctrl->GetMaxNumEvents(), &animCtrl);

	//D3DXFrameRegisterNamedMatrices(rootFrame, animCtrl);
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
	mesh->AddRef();
}