#pragma once
#include "d3dUtil.h"

class BufferFactory
{
public:
	BufferFactory();
	~BufferFactory();

	IDirect3DVertexBuffer9* coloredCubeBuffer();
	IDirect3DVertexBuffer9* textureCubeBuffer();
	IDirect3DIndexBuffer9* coloredCubeIndices();
	IDirect3DIndexBuffer9* texturedCubeIndices();
private:
	IDirect3DVertexBuffer9* mColoredCubeVB;
	IDirect3DVertexBuffer9* mTexturedCubeVB;
	IDirect3DIndexBuffer9* mColoredCubeIB;
	IDirect3DIndexBuffer9* mTexturedCubeIB;
};