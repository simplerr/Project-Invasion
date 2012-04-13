#include "Vertex.h"
#include "d3dUtil.h"

// Initialize static variables.
IDirect3DVertexDeclaration9* VertexCol::Decl = 0;
IDirect3DVertexDeclaration9* VertexPNT::Decl = 0;


void InitAllVertexDeclarations()
{
	//===============================================================
	// VertexPos
	D3DVERTEXELEMENT9 VertexColElements[] = 
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};	
	HR(gd3dDevice->CreateVertexDeclaration(VertexColElements, &VertexCol::Decl));

	//===============================================================
	// VertexPNT
	D3DVERTEXELEMENT9 VertexPNTElements[] = 
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},		// EXTREMELY IMPORTANT TO USE THE RIGHT OFFSET!!!!!
		D3DDECL_END()
	};	
	HR(gd3dDevice->CreateVertexDeclaration(VertexPNTElements, &VertexPNT::Decl));
}

void DestroyAllVertexDeclarations()
{
	ReleaseCOM(VertexCol::Decl);
	ReleaseCOM(VertexPNT::Decl);
}