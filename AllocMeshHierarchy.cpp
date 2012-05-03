#include "AllocMeshHierarchy.h"
#include "SkinnedMesh.h"

void CopyString(const char* input, char** output)
{
	if( input )
	{
		UINT length = (UINT)::strlen(input) + 1; // add 1 for terminating null charater.
		*output = new char[length];
		::strcpy(*output, input);
	}
	else
	{
		*output = 0;
	}
}

char* DuplicateCharString(const char* charString)
{
    if (!charString)
		return 0;

	size_t len=strlen(charString) + 1;
	char *newString = new char[len];
	memcpy(newString, charString, len*sizeof(char));

	return newString;
}

HRESULT AllocMeshHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
{
	// Remark: CreateFrame must return a non-null value for ppNewFrame,
	// otherwise D3DXLoadMeshHierarchyFromX will interpret it as 
	// a failed operation.

	*ppNewFrame = 0;

	FrameEx* frameEx = new FrameEx();
	ZeroMemory(frameEx,sizeof(FrameEx));

	frameEx->pMeshContainer = 0;
	frameEx->pFrameSibling = 0;
	frameEx->pFrameFirstChild = 0;
	D3DXMatrixIdentity(&frameEx->TransformationMatrix);
	D3DXMatrixIdentity(&frameEx->toRoot);

	*ppNewFrame = frameEx;

	// The frame name (note: may be 0 or zero length)
	if (Name && strlen(Name))
	{
		frameEx->Name=DuplicateCharString(Name);	
	}


    return D3D_OK;
}

HRESULT AllocMeshHierarchy::CreateMeshContainer(PCTSTR Name, 
	const D3DXMESHDATA* pMeshData,
	const D3DXMATERIAL* pMaterials, 
	const D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials, 
	const DWORD *pAdjacency, 
	ID3DXSkinInfo* pSkinInfo, 
	D3DXMESHCONTAINER** ppNewMeshContainer)
{	
	// Remark: CreateMeshContainer should always return a non-null value
	// for ppNewMeshContainer, even if we are not interested in the mesh 
	// (i.e., it contains no skin info), otherwise D3DXLoadMeshHierarchyFromX
	// will interpret it as  a failed operation.


	//===============================================================
	// Allocate a new D3DXMESHCONTAINER, and set its name.  

	MeshContainerEx* meshContainer = new MeshContainerEx();
	ZeroMemory(meshContainer, sizeof(MeshContainerEx));	

	if(Name)
		CopyString(Name, &meshContainer->Name);
	else
		CopyString("<no name>", &meshContainer->Name);

	//===============================================================
	// Save our created mesh container now because we might return 
	// early, and we must _always_ return an allocated container.

	*ppNewMeshContainer = 0;//meshContainer;

	//===============================================================
	// Only interested in meshes with skin info and regular meshes.
	// If a given mesh does not satisfy these requirements we do
	// not return an error; rather we simply skip loading anything
	// for this mesh container.

	if( pSkinInfo == 0 || pMeshData->Type != D3DXMESHTYPE_MESH)
		return D3D_OK;
	
	meshContainer->MeshData.Type  = D3DXMESHTYPE_MESH;

	// Adjacency data - holds information about triangle adjacency, required by the ID3DMESH object
	DWORD dwFaces = pMeshData->pMesh->GetNumFaces();
	meshContainer->pAdjacency = new DWORD[dwFaces*3];
	if (pAdjacency)
	{
		memcpy(meshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * dwFaces*3);
	}
	else
	{
		// Added 24/08/10: previously did not detect null adjacency
		if(FAILED(meshContainer->MeshData.pMesh->GenerateAdjacency( 1e-6f, (DWORD*)(void*)meshContainer->pAdjacency ) ) )
            FillMemory((void*)meshContainer->pAdjacency, dwFaces*3, 0xFF );
	}

	LPDIRECT3DDEVICE9 pd3dDevice = 0;
	pMeshData->pMesh->GetDevice(&pd3dDevice);

	meshContainer->MeshData.pMesh = pMeshData->pMesh; 
	meshContainer->MeshData.pMesh->AddRef();

	//===============================================================
	// Copy material data, and allocate memory for texture file names.
	meshContainer->NumMaterials = max(NumMaterials, 1);
	meshContainer->pMaterials   = new D3DXMATERIAL[NumMaterials];
	D3DXMATERIAL* mtrls = meshContainer->pMaterials;
	for(DWORD i = 0; i < NumMaterials; ++i)
	{
		mtrls[i].MatD3D = pMaterials[i].MatD3D;
		mtrls[i].MatD3D.Ambient = pMaterials[i].MatD3D.Diffuse;

		CopyString(pMaterials[i].pTextureFilename, 
			&mtrls[i].pTextureFilename);
	}

	//===============================================================
	// Ignore effect instances and adjacency info for this demo.
	meshContainer->pEffects   = 0;

	//===============================================================
	// Save mesh and skininfo.
	meshContainer->pSkinInfo = pSkinInfo;
	pSkinInfo->AddRef();

	int numBones = meshContainer->pSkinInfo->GetNumBones();
	meshContainer->exFinalXForms.resize(numBones);
	meshContainer->exToRootXFormPtrs.resize(numBones);

	pd3dDevice->Release();

	*ppNewMeshContainer = meshContainer;

    return D3D_OK;
}

HRESULT AllocMeshHierarchy::DestroyFrame(D3DXFRAME* pFrameToFree) 
{
	delete[] pFrameToFree->Name;
	delete pFrameToFree;
	pFrameToFree = NULL;
 
    return D3D_OK; 
}

HRESULT AllocMeshHierarchy::DestroyMeshContainer(D3DXMESHCONTAINER* pMeshContainerBase)
{
	delete[] pMeshContainerBase->Name;
	delete[] pMeshContainerBase->pAdjacency;
	delete[] pMeshContainerBase->pEffects;
 
	for(DWORD i = 0; i < pMeshContainerBase->NumMaterials; ++i)
		delete[] pMeshContainerBase->pMaterials[i].pTextureFilename;

	delete[] pMeshContainerBase->pMaterials;

	ReleaseCOM(pMeshContainerBase->MeshData.pMesh);
	ReleaseCOM(pMeshContainerBase->pSkinInfo);
	
	MeshContainerEx* container = (MeshContainerEx*) pMeshContainerBase;
	ReleaseCOM(container->exSkinnedMesh);
 
	delete pMeshContainerBase;
 
    return D3D_OK;
}