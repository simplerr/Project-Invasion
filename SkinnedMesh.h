#pragma once

#include <vector>
#include <string>
#include "Object3D.h"
#include "d3dUtil.h"

using namespace std;

// Extended frame (bone) structure
struct FrameEx : public D3DXFRAME
{
	D3DXMATRIX toRoot;
};

// Extended mesh container
struct MeshContainerEx : public D3DXMESHCONTAINER
{
	ID3DXMesh*					exSkinnedMesh;		// The skinned mesh (updated with software skinning).
	std::vector<D3DXMATRIX>		exFinalXForms;		// Final transforms for each frame.
	std::vector<D3DXMATRIX*>	exToRootXFormPtrs;	// To root transform for each frame.
};

class SkinnedMesh : public Object3D
{
public:
	SkinnedMesh(string filename, D3DXVECTOR3 position, ObjectType type);
	virtual ~SkinnedMesh();

	virtual void update(float dt);
	virtual void draw();

	bool rayIntersectAABB(D3DXVECTOR3 position, D3DXVECTOR3 direction);
	bool rayIntersectMesh(D3DXVECTOR3 position, D3DXVECTOR3 direction, DWORD& faceIndex, float& distance);

	void drawFrame(D3DXFRAME* frame);
	void drawFrameHierarchy();

	virtual void updateWorldMatrix();

	void setAnimation(UINT index, float transisitionTime = 0.50f);
	void setSpeedAdjust(float speedAdjust);

	UINT numVertices();
	UINT numTriangles();
	UINT numBones();
protected:
	ID3DXMesh* buildSkinnedMesh(ID3DXMesh* mesh, D3DXMESHCONTAINER* container);
	void setupBoneMatrices(FrameEx* frame);
	void drawMeshContainer(D3DXFRAME* frame, D3DXMATRIX transformation);
	void updateToRootXForms(FrameEx* frame, D3DXMATRIX& parentsToRoot);
	void updateSkinnedMesh(FrameEx* frame);
	bool hasNormals(ID3DXMesh* mesh);
	void loadTextures(D3DXFRAME* frame);
	void cleanupFrame(D3DXFRAME* frame);

protected:
	ID3DXAnimationController* mAnimCtrl;	// Controls the animation and keyframes.
	D3DXFRAME*			mRoot;				// The root frame (bone).
	DWORD				mNumBones;			// Number of bones.
	DWORD				mNumVertices;
	DWORD				mNumTriangles;

	UINT				mCurrentTrack;
	UINT				mCurrentAnimationSet;
	float				mCurrentTime;
	float				mSpeedAdjust;
	float				mDeltaTime;

	// [HACK] !
	ID3DXMesh*			mBaseMesh;

	// Maximum bones supported.
	static const int MAX_NUM_BONES_SUPPORTED = 50; 
};