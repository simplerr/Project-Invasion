#include "Terrain.h"
#include "Graphics.h"
#include "Vertex.h"
#include "AABB.h"
#include "Input.h"
#include "Mesh.h"

Terrain::Terrain(string source, int rows, int colums, float dx, float dz, float heightScale)
{
	mTerrainMesh = new Mesh(D3DXVECTOR3(0, 0, 0), TERRAIN);
	mRows = rows;
	mColums = colums;
	mHeightmap = new Heightmap();
	mHeightmap->loadRAW(rows, colums, source, heightScale, 0.0f);
	mMaterial = Material(WHITE, WHITE, BLACK, 16);
	mPosition = D3DXVECTOR3(0, 0, 0);

	vector<D3DXVECTOR3> verts;
	vector<DWORD> indices;
	
	mNumVertices = rows*colums;
	mNumTriangles = (rows-1)*(colums-1)*2;
	build(D3DXVECTOR3(0, 0, 0), rows, colums, dx, dz, verts, indices);

	// Create the mesh.
	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	UINT numElems = 0;
	HR(VertexPNT::Decl->GetDeclaration(elems, &numElems));
	HR(D3DXCreateMesh(mNumTriangles, mNumVertices, D3DXMESH_SYSTEMMEM | D3DXMESH_32BIT, elems, gd3dDevice, &mTerrainMesh->mMesh));

	// Write the vertices.
	VertexPNT* v = 0;
	HR(mTerrainMesh->getMesh()->LockVertexBuffer(0,(void**)&v));

	float w = (colums-1) * dx;
	float d = (rows-1) * dz;
	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < colums; ++j)
		{
			DWORD index = i * rows + j;
			v[index].pos	= verts[index];
			v[index].pos.y	= mHeightmap->operator()(i, j);
			v[index].tex0.x = (v[index].pos.x + (0.5f*w)) / w;
			v[index].tex0.y = (v[index].pos.z - (0.5f*d)) / -d;
		}
	}

	// Write the indices.
	DWORD* k = 0;
	HR(mTerrainMesh->getMesh()->LockIndexBuffer(0, (void**)&k));
	// Compute the indices for each triangle.
	for(int i = 0; i < mNumTriangles; ++i)
	{
		k[i*3+0] = (DWORD)indices[i*3+0];
		k[i*3+1] = (DWORD)indices[i*3+1];
		k[i*3+2] = (DWORD)indices[i*3+2];
	}

	// Unlock the index and attribute buffers.
	HR(mTerrainMesh->getMesh()->UnlockIndexBuffer());

	// Generate normals and then optimize the mesh.
	HR(D3DXComputeNormals(mTerrainMesh->getMesh(), 0));

	// Unlock the vertex buffer.
	HR(mTerrainMesh->getMesh()->UnlockVertexBuffer());

	DWORD* adj = new DWORD[mTerrainMesh->getMesh()->GetNumFaces()*3];
	HR(mTerrainMesh->getMesh()->GenerateAdjacency(EPSILON, adj));
	//HR(mTerrainMesh->OptimizeInplace(D3DXMESHOPT_VERTEXCACHE|D3DXMESHOPT_ATTRSORT, adj, 0, 0, 0));
	delete[] adj;

	mDX = dx;
	mDZ = dz;

	mWidth = (rows-1)*mDX;
	mDepth = (colums-1)*mDZ;
}
	
Terrain::~Terrain()
{
	delete mHeightmap;
	delete mTerrainMesh;
}
	
void Terrain::draw()
{
	gGraphics->drawTerrain(this);

	// Testing...
	/*if(gInput->keyDown(VK_LBUTTON))
	{
		VertexPNT* v;
		mTerrainMesh->LockVertexBuffer(0, (void**)&v);

		for(int x = 0; x < 100; x++)
			v[x].pos.y = 500.0f;

		mTerrainMesh->UnlockVertexBuffer();
	}*/


	/*void* v = 0;
	BoundingBox boundingBox;
	mTerrainMesh->LockVertexBuffer(0, (void**)&v);
	D3DXComputeBoundingBox((D3DXVECTOR3*)v, getVertices(), sizeof(VertexPNT), &boundingBox.min, &boundingBox.max); 
	gGraphics->drawBoundingBox(getPosition(), boundingBox.getDimensions(), RED, 0.3f);

	mTerrainMesh->UnlockVertexBuffer();*/
}
	
float Terrain::getHeight(float x, float z)
{
	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f*mWidth) /  mDX;
	float d = (z - 0.5f*mDepth) /  -mDZ;

	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = mHeightmap->operator()(row, col);
	float B = mHeightmap->operator()(row, col+1);
	float C = mHeightmap->operator()(row+1, col);
	float D = mHeightmap->operator()(row+1, col+1);

	// Where we are relative to the cell.
	float s = c - (float)col;
	float t = d - (float)row;

	// If upper triangle ABC.
	if(t < 1.0f - s)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s*uy + t*vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f-s)*uy + (1.0f-t)*vy;
	}
}

void Terrain::build(D3DXVECTOR3 center, int rows, int colums, float dx, float dz, vector<D3DXVECTOR3>& verts, vector<DWORD>& indices)
{
	int	cellRows = rows - 1;
	int cellCols = colums - 1;
	float width = (float)cellCols * dx;
	float depth = (float)cellRows * dz;

	verts.resize(rows*colums);
	indices.resize(cellRows*cellCols*2*3);

	//================================================================
	// Build vertics.

	int k = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < colums; j++)
		{
			verts[k].x = j * dx - width/2;
			verts[k].z = -i * dz + depth/2;
			verts[k].y = 0.0f;

			D3DXMATRIX T;
			D3DXMatrixTranslation(&T, center.x, center.y, center.z);
			D3DXVec3TransformCoord(&verts[k], &verts[k], &T);
			++k;
		}
	}

	//================================================================
	// Build indices.

	k = 0;
	for(DWORD i = 0; i < (DWORD)cellRows; i++)
	{
		for(DWORD j = 0; j < (DWORD)cellCols; j++)
		{
			indices[k]     =   i   * colums + j;
			indices[k + 1] =   i   * colums + j + 1;
			indices[k + 2] = (i+1) * colums + j;
					
			indices[k + 3] = (i+1) * colums + j;
			indices[k + 4] =   i   * colums + j + 1;
			indices[k + 5] = (i+1) * colums + j + 1;

			// Next quad.
			k += 6;
		}
	}
}

void Terrain::smothOut(int x, int z, int radius)
{
	VertexPNT* v;
	mTerrainMesh->getMesh()->LockVertexBuffer(0, (void**)&v);

	vector<float> avgs;

	// Loop through each vertex we want to smoth out.
	for(int i = z - radius/2; i < z + radius/2; i++)
	{
		for(int j = x - radius/2; j < x + radius/2; j++)
		{
			// Loop through all neighbouring vertices.
			int num = 0;
			float avg = 0.0f;

			for(int a = i - radius/2; a < i + radius/2; a++)
			{
				for(int b = j - radius/2; b < j + radius/2; b++)
				{
					avg += v[a*mColums + b].pos.y;
					num++;
				}
			}

			avg /= (float)num;
			avgs.push_back(avg);
		}
	}

	int a = 0;
	for(int i = z - radius/2; i < z + radius/2; i++)
		for(int j = x - radius/2; j < x + radius/2; j++, a++)
			v[i*mColums + j].pos.y = avgs[a];

	mTerrainMesh->getMesh()->UnlockVertexBuffer();
}
	
D3DXVECTOR3 Terrain::getPosition()
{
	return mPosition;
}
	
int Terrain::getVertices()
{
	return mNumVertices;
}
	
int Terrain::getTriangles()
{
	return mNumTriangles;
}

Material Terrain::getMaterial()
{
	return mMaterial;
}

int Terrain::getColums()
{
	return mColums;
}
	
int Terrain::getRows()
{
	return mRows;
}

Mesh* Terrain::getMesh()
{
	return mTerrainMesh;
}