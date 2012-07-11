#include "Terrain.h"
#include "Graphics.h"
#include "Vertex.h"
#include "AABB.h"
#include "Input.h"
#include "Mesh.h"

struct SubGrid
{
	const static int NUM_ROWS  = 53;
	const static int NUM_COLS  = 53;
	const static int NUM_TRIS  = (NUM_ROWS-1)*(NUM_COLS-1)*2;
	const static int NUM_VERTS = NUM_ROWS*NUM_COLS;
};

Terrain::Terrain(string source, int rows, int colums, float dx, float dz, float heightScale)
{
	mTerrainMesh = new Mesh(D3DXVECTOR3(0, 0, 0), TERRAIN);
	mRows = rows;
	mColums = colums;
	mHeightmap = new Heightmap();
	mHeightmap->loadRAW(rows, colums, source, heightScale, 0.0f);
	mMaterial = Material(WHITE, WHITE, BLACK, 16);
	mPosition = D3DXVECTOR3(0, 0, 0);
	mDX = dx;
	mDZ = dz;
	mWidth = (rows-1)*mDX;
	mDepth = (colums-1)*mDZ;
	
	mNumVertices = rows*colums;
	mNumTriangles = (rows-1)*(colums-1)*2;

	ID3DXMesh* mesh = mTerrainMesh->getMesh();

	// Create the mesh.
	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	UINT numElems = 0;
	HR(VertexPNT::Decl->GetDeclaration(elems, &numElems));
	HR(D3DXCreateMesh(mNumTriangles, mNumVertices, D3DXMESH_SYSTEMMEM | D3DXMESH_32BIT, elems, gd3dDevice, &mTerrainMesh->mMesh));

	// Write the vertices.
	VertexPNT* v = 0;
	HR(mTerrainMesh->mMesh->LockVertexBuffer(0,(void**)&v));

	// Build the grid.
	vector<D3DXVECTOR3> verts;
	vector<DWORD> indices;
	build(D3DXVECTOR3(0, 0, 0), rows, colums, dx, dz, verts, indices);

	float w = mWidth;
	float d = mDepth;
	for(UINT i = 0; i < mTerrainMesh->mMesh->GetNumVertices(); ++i)
	{
		// We store the grid vertices in a linear array, but we can
		// convert the linear array index to an (r, c) matrix index.
		int r = i / mColums;
		int c = i % mColums;

		v[i].pos   = verts[i];
		v[i].pos.y = mHeightmap->operator()(r, c);

		v[i].tex0.x = (v[i].pos.x + (0.5f*w)) / w;
		v[i].tex0.y = (v[i].pos.z - (0.5f*d)) / -d;
	}

	// Write triangle data so we can compute normals.
	DWORD* indexBuffPtr = 0;
	HR(mTerrainMesh->mMesh->LockIndexBuffer(0, (void**)&indexBuffPtr));
	for(UINT i = 0; i < mTerrainMesh->getMesh()->GetNumFaces(); ++i)
	{
		indexBuffPtr[i*3+0] = indices[i*3+0];
		indexBuffPtr[i*3+1] = indices[i*3+1];
		indexBuffPtr[i*3+2] = indices[i*3+2];
	}

	// Unlock the index and attribute buffers.
	HR(mTerrainMesh->mMesh->UnlockIndexBuffer());

	// Generate normals and then optimize the mesh.
	HR(D3DXComputeNormals(mTerrainMesh->mMesh, 0));

	// Break it up to subgrids.
	int subGridRows = (mRows-1) / (SubGrid::NUM_ROWS-1);
	int subGridCols = (mColums-1) / (SubGrid::NUM_COLS-1);

	for(int r = 0; r < subGridRows; ++r)
	{
		for(int c = 0; c < subGridCols; ++c)
		{
			// Rectangle that indicates (via matrix indices ij) the
			// portion of global grid vertices to use for this subgrid.
			RECT R = 
			{
					c * (SubGrid::NUM_COLS-1),
					r * (SubGrid::NUM_ROWS-1),
				(c+1) * (SubGrid::NUM_COLS-1),
				(r+1) * (SubGrid::NUM_ROWS-1)
			};

			buildSubGridMesh(R, v); 
		}
	}

	// Unlock the vertex buffer.
	HR(mTerrainMesh->mMesh->UnlockVertexBuffer());
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
	int numVertices = rows*colums;
	int numCellRows = rows-1;
	int numCellCols = colums-1;

	int numTris = numCellRows*numCellCols*2;

	float width = (float)numCellCols * dx;
	float depth = (float)numCellRows * dz;

	//===========================================
	// Build vertices.

	// We first build the grid geometry centered about the origin and on
	// the xz-plane, row-by-row and in a top-down fashion.  We then translate
	// the grid vertices so that they are centered about the specified 
	// parameter 'center'.

	verts.resize( numVertices );

	// Offsets to translate grid from quadrant 4 to center of 
	// coordinate system.
	float xOffset = -width * 0.5f; 
	float zOffset =  depth * 0.5f;

	int k = 0;
	for(float i = 0; i < rows; ++i)
	{
		for(float j = 0; j < colums; ++j)
		{
			// Negate the depth coordinate to put in quadrant four.  
			// Then offset to center about coordinate system.
			verts[k].x =  j * dx + xOffset;
			verts[k].z = -i * dz + zOffset;
			verts[k].y =  0.0f;

			// Translate so that the center of the grid is at the
			// specified 'center' parameter.
			D3DXMATRIX T;
			D3DXMatrixTranslation(&T, center.x, center.y, center.z);
			D3DXVec3TransformCoord(&verts[k], &verts[k], &T);
			
			++k; // Next vertex
		}
	}

	//===========================================
	// Build indices.

	indices.resize(numTris * 3);
	 
	// Generate indices for each quad.
	k = 0;
	for(DWORD i = 0; i < (DWORD)numCellRows; ++i)
	{
		for(DWORD j = 0; j < (DWORD)numCellCols; ++j)
		{
			indices[k]     =   i   * colums + j;
			indices[k + 1] =   i   * colums + j + 1;
			indices[k + 2] = (i+1) * colums + j;
					
			indices[k + 3] = (i+1) * colums + j;
			indices[k + 4] =   i   * colums + j + 1;
			indices[k + 5] = (i+1) * colums + j + 1;

			// next quad
			k += 6;
		}
	}
}

void Terrain::buildSubGridMesh(RECT& R, VertexPNT* gridVerts)
{
	//===============================================================
	// Create the subgrid mesh.
	ID3DXMesh* subMesh = 0;
	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	UINT numElems = 0;
	HR(VertexPNT::Decl->GetDeclaration(elems, &numElems));
	HR(D3DXCreateMesh(SubGrid::NUM_TRIS, SubGrid::NUM_VERTS, 
		D3DXMESH_MANAGED, elems, gd3dDevice, &subMesh));


	//===============================================================
	// Build Vertex Buffer.  Copy rectangle of vertices from the
	// grid into the subgrid structure.
	VertexPNT* v = 0;
	HR(subMesh->LockVertexBuffer(0, (void**)&v));
	int k = 0;
	for(int i = R.top; i <= R.bottom; ++i)
	{
		for(int j = R.left; j <= R.right; ++j)
		{
			v[k++] = gridVerts[i*mColums+j];
		}
	}

	//===============================================================
	// Compute the bounding box before unlocking the vertex buffer.
	AABB bndBox;
	HR(D3DXComputeBoundingBox((D3DXVECTOR3*)v, subMesh->GetNumVertices(), 
		sizeof(VertexPNT), &bndBox.min, &bndBox.max));

	HR(subMesh->UnlockVertexBuffer());


	//===============================================================
	// Build Index and Attribute Buffer.
	// Get indices for subgrid (we don't use the verts here--the verts
	// are given by the parameter gridVerts).
	std::vector<D3DXVECTOR3> tempVerts;
	std::vector<DWORD> tempIndices;
	build(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SubGrid::NUM_ROWS, SubGrid::NUM_COLS, mDX, mDZ, 
		tempVerts, tempIndices);

	WORD* indices  = 0;
	DWORD* attBuff = 0;
	HR(subMesh->LockIndexBuffer(0, (void**)&indices));
	HR(subMesh->LockAttributeBuffer(0, &attBuff));
	for(int i = 0; i < SubGrid::NUM_TRIS; ++i)
	{
		indices[i*3+0] = (WORD)tempIndices[i*3+0];
		indices[i*3+1] = (WORD)tempIndices[i*3+1];
		indices[i*3+2] = (WORD)tempIndices[i*3+2];

		attBuff[i] = 0; // All in subset 0.
	}
	HR(subMesh->UnlockIndexBuffer());
	HR(subMesh->UnlockAttributeBuffer());


	//===============================================================
	// Optimize for the vertex cache and build attribute table.
	DWORD* adj = new DWORD[subMesh->GetNumFaces()*3];
	HR(subMesh->GenerateAdjacency(0.001f, adj));
	HR(subMesh->OptimizeInplace(D3DXMESHOPT_VERTEXCACHE|D3DXMESHOPT_ATTRSORT,
		adj, 0, 0, 0));
	delete[] adj;

	
	//===============================================================
	// Save the mesh and bounding box.
	mSubGridMeshes.push_back(subMesh);
	mSubGridBndBoxes.push_back(bndBox);
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