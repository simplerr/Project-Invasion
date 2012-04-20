#include <vector>
#include <string>
#include "Material.h"
#include "d3dUtil.h"
#include "Vertex.h"
#include "Graphics.h"
#include "Vector.h"

bool equals(D3DXVECTOR3 a, D3DXVECTOR3 b, float epsilion)
{
	if(equal(a.x,b.x, epsilion) && equal(a.y,b.y, epsilion) && equal(a.z,b.z, epsilion)) 
			return true;
		else 
			return false;
}

void LoadXFile(
	const std::string& filename, 
	ID3DXMesh** meshOut,
	std::vector<Material>& mtrls, 
	std::vector<IDirect3DTexture9*>& texs)
{
	// Step 1: Load the .x file from file into a system memory mesh.
	ID3DXMesh* meshSys      = 0;
	ID3DXBuffer* adjBuffer  = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD numMtrls          = 0;

	HR(D3DXLoadMeshFromX(filename.c_str(), D3DXMESH_SYSTEMMEM, gd3dDevice,
		&adjBuffer,	&mtrlBuffer, 0, &numMtrls, &meshSys));

	// Step 2: Find out if the mesh already has normal info?
	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	HR(meshSys->GetDeclaration(elems));
	
	bool hasNormals = false;
	D3DVERTEXELEMENT9 term = D3DDECL_END();
	for(int i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		// Did we reach D3DDECL_END() {0xFF,0,D3DDECLTYPE_UNUSED, 0,0,0}?
		if(elems[i].Stream == 0xff )
			break;

		if( elems[i].Type == D3DDECLTYPE_FLOAT3 &&
			elems[i].Usage == D3DDECLUSAGE_NORMAL &&
			elems[i].UsageIndex == 0 )
		{
			hasNormals = true;
			break;
		}
	}

	// Step 3: Change vertex format to VertexPNT.
	D3DVERTEXELEMENT9 elements[64];
	UINT numElements = 0;
	VertexPNT::Decl->GetDeclaration(elements, &numElements);

	ID3DXMesh* temp = 0;
	HR(meshSys->CloneMesh(D3DXMESH_SYSTEMMEM, 
		elements, gd3dDevice, &temp));
	ReleaseCOM(meshSys);
	meshSys = temp;

	// Step 4: If the mesh did not have normals, generate them.
	if(hasNormals == false)
		HR(D3DXComputeNormals(meshSys, 0));

	// Step 5: Optimize the mesh.
	HR(meshSys->Optimize(D3DXMESH_MANAGED | D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, 
		(DWORD*)adjBuffer->GetBufferPointer(), 0, 0, 0, meshOut));
	ReleaseCOM(meshSys); // Done w/ system mesh.
	ReleaseCOM(adjBuffer); // Done with buffer.

	// Step 6: Extract the materials and load the textures.

	if(mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* d3dxmtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for(DWORD i = 0; i < numMtrls; ++i)
		{
			// Save the ith material.  Note that the MatD3D property does not have an ambient
			// value set when its loaded, so just set it to the diffuse value.
			Material m;
			m.ambient		= d3dxmtrls[i].MatD3D.Diffuse;
			m.diffuse		= d3dxmtrls[i].MatD3D.Diffuse;
			m.specular      = d3dxmtrls[i].MatD3D.Specular;
			m.specularPower = d3dxmtrls[i].MatD3D.Power;
			mtrls.push_back(m);

			// Check if the ith material has an associative texture
			if(d3dxmtrls[i].pTextureFilename != 0)
			{
				// Yes, load the texture for the ith subset
				IDirect3DTexture9* tex = 0;
				std::string source = "data/";
				source += d3dxmtrls[i].pTextureFilename;

				tex = gGraphics->loadTexture(source);

				//HR(D3DXCreateTextureFromFile(gd3dDevice, source.c_str(), &tex));

				// Save the loaded texture
				texs.push_back(tex);
			}
			else
			{
				// No texture for the ith subset
				texs.push_back(0);
			}
		}
	}
	ReleaseCOM(mtrlBuffer); // done w/ buffer
}