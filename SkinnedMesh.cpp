#include "SkinnedMesh.h"
#include "AllocMeshHierarchy.h"
#include "Vertex.h"
#include "Input.h"
#include "Graphics.h"
#include "Camera.h"

SkinnedMesh::SkinnedMesh(std::string filename, D3DXVECTOR3 position, float scale)
	: Object3D(position, scale) 
{
	char buffer[256];
	GetCurrentDirectory(256, buffer);
	//SetCurrentDirectory("F:/Users/Axel/Documents/Visual Studio 11/Projects/Stride3D/Stride3D/data");

	// Load the mesh hierarchy.
	AllocMeshHierarchy* allocMeshHierarchy = new AllocMeshHierarchy();
	HR(D3DXLoadMeshHierarchyFromX(filename.c_str(), D3DXMESH_MANAGED, gd3dDevice, allocMeshHierarchy, NULL, &mRoot, &mAnimCtrl));

	SetCurrentDirectory(buffer);

	// Starting values.
	mCurrentTrack =  mCurrentAnimationSet = mCurrentTime = mNumBones = mNumTriangles = mNumVertices = 0;
	mVelocityAdjust = 1.0f;

	// Setup the bone matrices, build the toRoot and the skinned mesh.
	setupBoneMatrices((FrameEx*)mRoot);

	// Load textures the mesh uses.
	loadTextures(mRoot);

	setRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

SkinnedMesh::~SkinnedMesh()
{
	// TODO: Clean up all mesh containers.
	if(mRoot)
	{
		AllocMeshHierarchy allocMeshHierarchy;
		HR(D3DXFrameDestroy(mRoot, &allocMeshHierarchy));
		mRoot = 0;
	}

	ReleaseCOM(mAnimCtrl);
}

void SkinnedMesh::cleanupFrame(D3DXFRAME* frame)
{
	LPD3DXMESHCONTAINER meshContainer = frame->pMeshContainer;
	if(meshContainer) {
		
	}

	if(!frame->pFrameFirstChild && !frame->pFrameSibling)
		return;

	if(frame->pFrameSibling)
		cleanupFrame(frame->pFrameSibling);
	if(frame->pFrameFirstChild)
		cleanupFrame(frame->pFrameFirstChild);
}

UINT SkinnedMesh::numVertices()
{
	return mNumVertices;
}

UINT SkinnedMesh::numTriangles()
{
	return mNumTriangles;
}

UINT SkinnedMesh::numBones()
{
	return mNumBones;
}

void SkinnedMesh::update(float dt)
{
	// Animate the mesh.  The AnimationController has pointers to the  hierarchy frame
	// transform matrices.  The AnimationController updates these matrices to reflect 
	// the given pose at the current time by interpolating between animation keyframes.
	HR(mAnimCtrl->AdvanceTime(dt*mVelocityAdjust, 0));
	
	mCurrentTime += dt;

	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);

	// Recurse down the tree and generate a frame's toRoot transform from the updated pose.
	updateToRootXForms((FrameEx*)mRoot, identity);
	updateSkinnedMesh((FrameEx*)mRoot);
}

void SkinnedMesh::updateSkinnedMesh(FrameEx* frame)
{
	MeshContainerEx* pMesh = (MeshContainerEx*)frame->pMeshContainer;
	if(pMesh && pMesh->pSkinInfo)
	{
		int numBones = pMesh->pSkinInfo->GetNumBones();
		for(int i = 0; i < numBones; i++)
			D3DXMatrixMultiply(&pMesh->exFinalXForms[i], pMesh->pSkinInfo->GetBoneOffsetMatrix(i), pMesh->exToRootXFormPtrs[i]);

		// Software skinning.
		void *srcPtr=0;
		pMesh->MeshData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&srcPtr);

		void *destPtr=0;
		pMesh->exSkinnedMesh->LockVertexBuffer(0, (void**)&destPtr);

		// Update the skinned mesh.
		pMesh->pSkinInfo->UpdateSkinnedMesh(&pMesh->exFinalXForms[0], NULL, srcPtr, destPtr);

		// Compute the AABB.
		D3DXComputeBoundingBox(&((VertexPNT*)destPtr)->pos, pMesh->exSkinnedMesh->GetNumVertices(), pMesh->exSkinnedMesh->GetNumBytesPerVertex(), &mAABB.min, &mAABB.max);

		// Unlock the meshes vertex buffers.
		pMesh->exSkinnedMesh->UnlockVertexBuffer();
		pMesh->MeshData.pMesh->UnlockVertexBuffer();		
	}

	// Pass on to sibling.
	if (frame->pFrameSibling != NULL)
		updateSkinnedMesh((FrameEx*)frame->pFrameSibling);

	// Pass on to child.
	if (frame->pFrameFirstChild != NULL)
		updateSkinnedMesh((FrameEx*)frame->pFrameFirstChild);
}

void SkinnedMesh::updateWorldMatrix()
{
	Object3D::updateWorldMatrix();
}

void SkinnedMesh::draw()
{
	// Draw all frames.
	drawFrame(mRoot);
}

void SkinnedMesh::drawFrameHierarchy()
{
	drawFrame(mRoot);
}

void SkinnedMesh::drawFrame(D3DXFRAME* frame)
{
	LPD3DXMESHCONTAINER meshContainer = frame->pMeshContainer;
	if(meshContainer) 
		drawMeshContainer(frame, frame->TransformationMatrix);

	if(!frame->pFrameFirstChild && !frame->pFrameSibling)
		return;

	if(frame->pFrameSibling)
		drawFrame(frame->pFrameSibling);
	if(frame->pFrameFirstChild)
		drawFrame(frame->pFrameFirstChild);
}	

void SkinnedMesh::drawMeshContainer(D3DXFRAME* frame, D3DXMATRIX transformation)
{
	MeshContainerEx* meshContainer = static_cast<MeshContainerEx*>(frame->pMeshContainer);
	for(int i = 0; i < meshContainer->NumMaterials; i++)
	{
		// Used for hardware skinning!
		//gGraphics->mFX->SetMatrixArray(gGraphics->mhFinalXForms, &meshContainer->exFinalXForms[0], meshContainer->pSkinInfo->GetNumBones());
		
		// Set the texture and commit the changes.
		gGraphics->mFX->SetTexture(gGraphics->mhTex, mTextures[i]);
		gGraphics->mFX->CommitChanges();

		// Draw the skinned mesh.
		meshContainer->exSkinnedMesh->DrawSubset(i);
	}
}

bool SkinnedMesh::rayIntersectAABB(D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	D3DXVec3TransformNormal(&direction, &direction, &getInvWorldMatrix());
	D3DXVec3TransformCoord(&position, &position, &getInvWorldMatrix());

	return D3DXBoxBoundProbe(&mAABB.min, &mAABB.max, &position, &direction);
}
	
bool SkinnedMesh::rayIntersectMesh(D3DXVECTOR3 position, D3DXVECTOR3 direction, DWORD& faceIndex, float& distance)
{
	BOOL hit = false;
	DWORD hitCount;
	float u, v;
	LPD3DXBUFFER allHits;

	D3DXVec3TransformNormal(&direction, &direction, &getInvWorldMatrix());
	D3DXVec3TransformCoord(&position, &position, &getInvWorldMatrix());
	D3DXIntersect(mBaseMesh, &position, &direction, &hit, &faceIndex, &u, &v, &distance, &allHits, &hitCount);
	return hit;
}

void SkinnedMesh::setupBoneMatrices(FrameEx* frame)
{
	// Cast to extended structure.
	MeshContainerEx* pMesh = (MeshContainerEx*)frame->pMeshContainer;

	if(pMesh && pMesh->pSkinInfo)
	{
		// Increment the counters.
		mNumBones += pMesh->pSkinInfo->GetNumBones();
		mNumVertices += pMesh->MeshData.pMesh->GetNumVertices();
		mNumTriangles += pMesh->MeshData.pMesh->GetNumFaces();

		// Build the skinned mesh [HARDWARE SKINNING]
		//pMesh->exSkinnedMesh = buildSkinnedMesh(pMesh->MeshData.pMesh, pMesh);

		// Clone and add the right declaration.
		D3DVERTEXELEMENT9 elements[64];
		UINT numElements = 0;
		VertexPNT::Decl->GetDeclaration(elements, &numElements);
		HR(pMesh->MeshData.pMesh->CloneMesh(D3DXMESH_SYSTEMMEM, elements, gd3dDevice, &pMesh->exSkinnedMesh));

		// Setup the toRoot list (make it point to the right toRoot data).
		for(int i = 0; i < pMesh->pSkinInfo->GetNumBones(); i++)
		{
			FrameEx* pTempFrame = (FrameEx*)D3DXFrameFind(mRoot, pMesh->pSkinInfo->GetBoneName(i));
			pMesh->exToRootXFormPtrs[i] = &pTempFrame->toRoot;
		}

		// Set the base mesh. NOTE: Hack!
		mBaseMesh = pMesh->exSkinnedMesh;
	}

	// Pass on to sibblings.
	if(frame->pFrameSibling)
		setupBoneMatrices((FrameEx*)frame->pFrameSibling);

	// Pass on to children.
	if(frame->pFrameFirstChild)
		setupBoneMatrices((FrameEx*)frame->pFrameFirstChild);
}

bool SkinnedMesh::hasNormals(ID3DXMesh* mesh)
{
	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	HR(mesh->GetDeclaration(elems));
	
	bool hasNormals = false;
	for(int i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		// Did we reach D3DDECL_END() {0xFF,0,D3DDECLTYPE_UNUSED, 0,0,0}?
		if(elems[i].Stream == 0xff)
			break;

		if( elems[i].Type == D3DDECLTYPE_FLOAT3 &&
			elems[i].Usage == D3DDECLUSAGE_NORMAL &&
			elems[i].UsageIndex == 0 )
		{
			hasNormals = true;
			break;
		}
	}
	return hasNormals;
}

ID3DXMesh* SkinnedMesh::buildSkinnedMesh(ID3DXMesh* mesh, D3DXMESHCONTAINER* container)
{
	//====================================================================
	// First add a normal component and 2D texture coordinates component.

	D3DVERTEXELEMENT9 elements[64];
	UINT numElements = 0;
	VertexPNT::Decl->GetDeclaration(elements, &numElements);

	ID3DXMesh* tempMesh = 0;
	HR(mesh->CloneMesh(D3DXMESH_SYSTEMMEM, elements, gd3dDevice, &tempMesh));
	 
	if(!hasNormals(tempMesh))
		HR(D3DXComputeNormals(tempMesh, 0));

	//====================================================================
	// Optimize the mesh; in particular, the vertex cache.
	DWORD* adj = new DWORD[tempMesh->GetNumFaces()*3];
	ID3DXBuffer* remap = 0;
	HR(tempMesh->GenerateAdjacency(EPSILON, adj));
	ID3DXMesh* optimizedTempMesh = 0;
	HR(tempMesh->Optimize(D3DXMESH_SYSTEMMEM | D3DXMESHOPT_VERTEXCACHE | 
		D3DXMESHOPT_ATTRSORT, adj, 0, 0, &remap, &optimizedTempMesh));

	ReleaseCOM(tempMesh); // Done w/ this mesh.
	delete[] adj;         // Done with buffer.

	// Update the index buffer.
	HR(container->pSkinInfo->Remap(optimizedTempMesh->GetNumVertices(), 
		(DWORD*)remap->GetBufferPointer()));
	ReleaseCOM(remap); // Done with remap info.

	// Add vertex weight and boneIndexes affecting it.
	ID3DXMesh*	 newMesh = 0;
	DWORD        numBoneComboEntries = 0;
	DWORD		 maxVertInfluences	 = 0;
	ID3DXBuffer* boneComboTable      = 0;
	HR(container->pSkinInfo->ConvertToIndexedBlendedMesh(optimizedTempMesh, D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,  
		MAX_NUM_BONES_SUPPORTED, container->pAdjacency, 0, 0, 0, &maxVertInfluences,
		&numBoneComboEntries, &boneComboTable, &newMesh));

	ReleaseCOM(optimizedTempMesh);	// Done with tempMesh.
	ReleaseCOM(boneComboTable);		// Don't need bone table.

	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	HR(newMesh->GetDeclaration(elems));
	container->pSkinInfo->SetDeclaration(elems);

	return newMesh;
}

void SkinnedMesh::updateToRootXForms(FrameEx* frame, D3DXMATRIX& parentsToRoot) 
{
	// Update the toRoot matrix, ie the tranform matrix fromt the bones local coordinate system to the root bones coordinate system (mRoot)
    frame->toRoot = frame->TransformationMatrix * parentsToRoot;

	// Recurse down siblings.
	if(frame->pFrameSibling)
		updateToRootXForms((FrameEx*)frame->pFrameSibling, parentsToRoot);

	// Recurse to first child.
	if(frame->pFrameFirstChild)
		updateToRootXForms((FrameEx*)frame->pFrameFirstChild, frame->toRoot);
}

void SkinnedMesh::loadTextures(D3DXFRAME* frame)
{
	// Recursively load all textures
	LPD3DXMESHCONTAINER meshContainer = frame->pMeshContainer;
	if(meshContainer && meshContainer->pMaterials) {
		for(int i = 0; i < meshContainer->NumMaterials; i++)
			mTextures.push_back(gGraphics->loadTexture(meshContainer->pMaterials[i].pTextureFilename));
	}

	if(!frame->pFrameFirstChild && !frame->pFrameSibling)
		return;

	if(frame->pFrameSibling)
		loadTextures(frame->pFrameSibling);
	if(frame->pFrameFirstChild)
		loadTextures(frame->pFrameFirstChild);
}

void SkinnedMesh::setAnimation(UINT index, float transisitionTime)
{
	if(index == mCurrentAnimationSet)
		return;

	if(index >= mAnimCtrl->GetNumAnimationSets())
		index = 0;

	LPD3DXANIMATIONSET set;

	// Set the current animation set.
	mCurrentAnimationSet = index;

	//LPD3DXANIMATIONSET set;
	mAnimCtrl->GetAnimationSet(mCurrentAnimationSet, &set);

	DWORD newTrack = mCurrentTrack == 0 ? 1 : 0;

	// Set the new animation to the second track.
	mAnimCtrl->SetTrackAnimationSet(newTrack, set);
	ReleaseCOM(set);

	// Remove event keys from the tracks, as we will fill them out with new.
	mAnimCtrl->UnkeyAllTrackEvents(mCurrentTrack);
	mAnimCtrl->UnkeyAllTrackEvents(newTrack);

	/*
		During the same time the old animation weight goes from 1 to 0, the new one goes from 0 to 1.
		That creates the fade in effect wanted. 
	*/

	// Add event key to turn off the current track, after transisition time.
	mAnimCtrl->KeyTrackEnable(mCurrentTrack, FALSE, mCurrentTime + transisitionTime);
	mAnimCtrl->KeyTrackSpeed(mCurrentTrack, 0.0f, mCurrentTime, transisitionTime, D3DXTRANSITION_LINEAR);
	mAnimCtrl->KeyTrackWeight(mCurrentTrack, 0.0f, mCurrentTime, transisitionTime, D3DXTRANSITION_LINEAR);


	// Enable the new track with a fade in effect.
	mAnimCtrl->SetTrackEnable(newTrack, TRUE);
	mAnimCtrl->KeyTrackSpeed(newTrack, 1.0f, mCurrentTime, transisitionTime, D3DXTRANSITION_LINEAR);
	mAnimCtrl->KeyTrackWeight(newTrack, 1.0f, mCurrentTime, transisitionTime, D3DXTRANSITION_LINEAR);

	mCurrentTrack = newTrack;
}

void SkinnedMesh::setSpeedAdjust(float speedAdjust)
{
	mVelocityAdjust = speedAdjust;
}