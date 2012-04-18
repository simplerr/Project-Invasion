#include "Mesh.h"
#include "Graphics.h"
#include "d3dUtil.h"
#include "Vertex.h"
#include "AABB.h"
#include "MeshFactory.h"

Mesh::Mesh(string filename, D3DXVECTOR3 position, float scale)
	: Object3D(position, scale)
{
	loadFromXFile(filename);
}
	
Mesh::Mesh(D3DXVECTOR3 position)
	: Object3D(position, Dimensions())
{

}

Mesh::~Mesh()
{
	ReleaseCOM(mMesh);
}

void Mesh::loadFromXFile(string filename)
{
	// d3dUtil.h handles loading of .X files.
	gMeshFactory->loadMesh(filename, mMesh, mMaterials, mTextures);

	// Compute the bounding box (AABB).
	VertexPNT* v;
	mMesh->LockVertexBuffer(0, (void**)&v);
	D3DXComputeBoundingBox(&v->pos, mMesh->GetNumVertices(), mMesh->GetNumBytesPerVertex(), &mAABB.min, &mAABB.max);
	mMesh->UnlockVertexBuffer();
}

void Mesh::draw()
{
	gGraphics->drawMesh(this);
}

bool Mesh::rayIntersectAABB(D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	D3DXVec3TransformNormal(&direction, &direction, &getInvWorldMatrix());
	D3DXVec3TransformCoord(&position, &position, &getInvWorldMatrix());

	return D3DXBoxBoundProbe(&mAABB.min, &mAABB.max, &position, &direction);
}
	
bool Mesh::rayIntersectMesh(D3DXVECTOR3 position, D3DXVECTOR3 direction, DWORD& faceIndex, float& distance)
{
	BOOL hit = false;
	DWORD hitCount;
	float u, v;
	LPD3DXBUFFER allHits;

	D3DXVec3TransformNormal(&direction, &direction, &getInvWorldMatrix());
	D3DXVec3TransformCoord(&position, &position, &getInvWorldMatrix());
	D3DXIntersect(getMesh(), &position, &direction, &hit, &faceIndex, &u, &v, &distance, &allHits, &hitCount);
	return hit;
}

ID3DXMesh* Mesh::getMesh()
{
	return mMesh;
}

AABB Mesh::getAABB()
{
	return mAABB;
}

const vector<Material>& Mesh::getMaterials()
{
	return mMaterials;
}