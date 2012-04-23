#include "BufferFactory.h"
#include "Vertex.h"

BufferFactory::BufferFactory()
{

}
	

BufferFactory::~BufferFactory()
{
	ReleaseCOM(mTexturedCubeIB);
	ReleaseCOM(mTexturedCubeVB);
	ReleaseCOM(mColoredCubeIB);
	ReleaseCOM(mColoredCubeVB);
}

IDirect3DVertexBuffer9* BufferFactory::coloredCubeBuffer()
{
	//===================================================================
	// Colored Cube vertex buffer 
	// Obtain a pointer to a new vertex buffer.
	HR(gd3dDevice->CreateVertexBuffer(8 * sizeof(VertexCol), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &mColoredCubeVB, 0));

	// Now lock it to obtain a pointer to its internal data, and write the
	// cube's vertex data.

	VertexCol* v = 0;
	HR(mColoredCubeVB->Lock(0, 0, (void**)&v, 0));

	v[0] = VertexCol(-1.0f, -1.0f, -1.0f);
	v[1] = VertexCol(-1.0f,  1.0f, -1.0f);
	v[2] = VertexCol( 1.0f,  1.0f, -1.0f);
	v[3] = VertexCol( 1.0f, -1.0f, -1.0f);
	v[4] = VertexCol(-1.0f, -1.0f,  1.0f);
	v[5] = VertexCol(-1.0f,  1.0f,  1.0f);
	v[6] = VertexCol( 1.0f,  1.0f,  1.0f);
	v[7] = VertexCol( 1.0f, -1.0f,  1.0f);

	HR(mColoredCubeVB->Unlock());

	return mColoredCubeVB;
}
	
IDirect3DVertexBuffer9* BufferFactory::textureCubeBuffer()
{
	HR(gd3dDevice->CreateVertexBuffer(24 * sizeof(VertexPNT), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &mTexturedCubeVB, 0));
	VertexPNT* v  = 0;

	HR(mTexturedCubeVB->Lock(0, 0, (void**)&v, 0));

	// Fill in the front face vertex data.
	v[0] = VertexPNT(-0.5, -0.5, -0.5, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = VertexPNT(-0.5,  0.5, -0.5, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = VertexPNT( 0.5,  0.5, -0.5, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[3] = VertexPNT( 0.5, -0.5, -0.5, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = VertexPNT(-0.5, -0.5, 0.5, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[5] = VertexPNT( 0.5, -0.5, 0.5, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[6] = VertexPNT( 0.5,  0.5, 0.5, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[7] = VertexPNT(-0.5,  0.5, 0.5, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8]  = VertexPNT(-0.5, 0.5, -0.5, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[9]  = VertexPNT(-0.5, 0.5,  0.5, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[10] = VertexPNT( 0.5, 0.5,  0.5, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[11] = VertexPNT( 0.5, 0.5, -0.5, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = VertexPNT(-0.5, -0.5, -0.5, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	v[13] = VertexPNT( 0.5, -0.5, -0.5, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	v[14] = VertexPNT( 0.5, -0.5,  0.5, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	v[15] = VertexPNT(-0.5, -0.5,  0.5, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = VertexPNT(-0.5, -0.5,  0.5, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[17] = VertexPNT(-0.5,  0.5,  0.5, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[18] = VertexPNT(-0.5,  0.5, -0.5, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[19] = VertexPNT(-0.5, -0.5, -0.5, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = VertexPNT( 0.5, -0.5, -0.5, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[21] = VertexPNT( 0.5,  0.5, -0.5, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[22] = VertexPNT( 0.5,  0.5,  0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[23] = VertexPNT( 0.5, -0.5,  0.5, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	HR(mTexturedCubeVB->Unlock());

	return mTexturedCubeVB;
}
	
IDirect3DIndexBuffer9* BufferFactory::coloredCubeIndices()
{
	HR(gd3dDevice->CreateIndexBuffer(36 * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &mColoredCubeIB, 0));

	// Now lock it to obtain a pointer to its internal data, and write the
	// cube's index data.

	WORD* k = 0;

	HR(mColoredCubeIB->Lock(0, 0, (void**)&k, 0));

	// Front face.
	k[0] = 0; k[1] = 1; k[2] = 2;
	k[3] = 0; k[4] = 2; k[5] = 3;

	// Back face.
	k[6] = 4; k[7]  = 6; k[8]  = 5;
	k[9] = 4; k[10] = 7; k[11] = 6;

	// Left face.
	k[12] = 4; k[13] = 5; k[14] = 1;
	k[15] = 4; k[16] = 1; k[17] = 0;

	// Right face.
	k[18] = 3; k[19] = 2; k[20] = 6;
	k[21] = 3; k[22] = 6; k[23] = 7;

	// Top face.
	k[24] = 1; k[25] = 5; k[26] = 6;
	k[27] = 1; k[28] = 6; k[29] = 2;

	// Bottom face.
	k[30] = 4; k[31] = 0; k[32] = 3;
	k[33] = 4; k[34] = 3; k[35] = 7;

	HR(mColoredCubeIB->Unlock());
	
	return mColoredCubeIB;
}

IDirect3DIndexBuffer9* BufferFactory::texturedCubeIndices()
{
	HR(gd3dDevice->CreateIndexBuffer(36 * sizeof(WORD),	D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,	D3DPOOL_MANAGED, &mTexturedCubeIB, 0));

	// Write box indices to the index buffer.
	WORD* i = 0;
	HR(mTexturedCubeIB->Lock(0, 0, (void**)&i, 0));

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7]  = 5; i[8]  = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] =  9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	HR(mTexturedCubeIB->Unlock());

	return mTexturedCubeIB;
}