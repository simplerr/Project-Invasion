#pragma once

#include <d3dx9.h>

// Implements the ID3DXAllocateHierarchy interface.  In order to create and destroy an animation 
// hierarchy using the D3DXLoadMeshHierarchyFromX and D3DXFrameDestroy functions, we must implement
// the ID3DXAllocateHierarchy interface, which defines how meshes and frames are created and 
// destroyed, thereby giving us some flexibility in the construction and destruction process.

class AllocMeshHierarchy : public ID3DXAllocateHierarchy 
{
public:
	HRESULT STDMETHODCALLTYPE CreateFrame(THIS_ PCSTR Name, D3DXFRAME** ppNewFrame);                     

	HRESULT STDMETHODCALLTYPE CreateMeshContainer(PCSTR Name, const D3DXMESHDATA* pMeshData,               
		const D3DXMATERIAL* pMaterials, const D3DXEFFECTINSTANCE* pEffectInstances, DWORD NumMaterials, 
		const DWORD *pAdjacency, ID3DXSkinInfo* pSkinInfo, D3DXMESHCONTAINER** ppNewMeshContainer);     

	HRESULT STDMETHODCALLTYPE DestroyFrame(THIS_ D3DXFRAME* pFrameToFree);              
	HRESULT STDMETHODCALLTYPE DestroyMeshContainer(THIS_ D3DXMESHCONTAINER* pMeshContainerBase);
};

//--------------------------------------------------------------------------------------
// Name: class CAllocateHierarchy
// Desc: Custom version of ID3DXAllocateHierarchy with custom methods to create
//       frames and meshcontainers.
//--------------------------------------------------------------------------------------
class CAllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
    STDMETHOD( CreateFrame )( THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame );
    STDMETHOD( CreateMeshContainer )( THIS_
        LPCSTR Name,
        CONST D3DXMESHDATA *pMeshData,
        CONST D3DXMATERIAL *pMaterials,
        CONST D3DXEFFECTINSTANCE *pEffectInstances,
        DWORD NumMaterials,
        CONST DWORD *pAdjacency,
        LPD3DXSKININFO pSkinInfo,
        LPD3DXMESHCONTAINER *ppNewMeshContainer );
    STDMETHOD( DestroyFrame )( THIS_ LPD3DXFRAME pFrameToFree );
    STDMETHOD( DestroyMeshContainer )( THIS_ LPD3DXMESHCONTAINER pMeshContainerBase );

    CAllocateHierarchy()
    {
    }
};
