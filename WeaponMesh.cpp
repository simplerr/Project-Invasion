#include "WeaponMesh.h"
#include "Camera.h"

WeaponMesh::WeaponMesh(string filename, D3DXVECTOR3 position, float scale)
	: SkinnedMesh(filename, position, scale)
{

}
	
WeaponMesh::~WeaponMesh()
{

}

void WeaponMesh::updateWorldMatrix()
{
	D3DXMATRIX T, S, R1, R2;

	D3DXMatrixTranslation(&T, mPosition.x, mPosition.y, mPosition.z);

	D3DXVECTOR3 dir = getRotation();
	float rotY = atan2f(dir.x, dir.z);
	float rotXZ = dir.y * D3DX_PI/2 * 0.8;

	// -rigth for some reason :)
	D3DXMatrixRotationY(&R1, rotY);
	D3DXMatrixRotationAxis(&R2, &-gCamera->getRight(), rotXZ);
	D3DXMatrixScaling(&S, mScale.width, mScale.height, mScale.depth);

	mWorldMatrix = S*R1*R2*T;
	D3DXMatrixInverse(&mInvWorldMatrix, 0, &mWorldMatrix);
}