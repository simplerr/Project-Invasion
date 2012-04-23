#ifndef VERTEX_H
#define VERTEX_H

#include <d3dx9.h>

// Call in constructor and destructor, respectively, of derived application class.
void InitAllVertexDeclarations();
void DestroyAllVertexDeclarations();

//===============================================================
struct VertexCol
{
	VertexCol():pos(0.0f, 0.0f, 0.0f), col(0x00000000){}
	VertexCol(float x, float y, float z, D3DCOLOR c = 0xff000000):pos(x,y,z), col(c){}
	VertexCol(const D3DXVECTOR3& v):pos(v){}

	D3DXVECTOR3 pos;
	D3DCOLOR    col;
	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct VertexPNT
{
	VertexPNT()
		:pos(0.0f, 0.0f, 0.0f), normal(0, 0, 0), tex0(0.0f, 0.0f){}
	VertexPNT(float x, float y, float z, float nx, float ny, float nz, float u, float v)
		:pos(x,y,z), normal(nx, ny, nz), tex0(u,v){}
	VertexPNT(const D3DXVECTOR3& v, const D3DXVECTOR3& n, const D3DXVECTOR2& uv)
		:pos(v), normal(n), tex0(uv){}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 tex0;

	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct Particle
{
	D3DXVECTOR3 initialPos;
	D3DXVECTOR3 initialVelocity;
	float       initialSize; // In pixels.
	float       initialTime;
	float       lifeTime;
	float       mass;
	D3DCOLOR    initialColor;

	static IDirect3DVertexDeclaration9* Decl;
};

#endif // VERTEX_H