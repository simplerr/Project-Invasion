//=============================================================================
// d3dUtil.h by Frank Luna (C) 2005 All Rights Reserved.
//
// Contains various utility code for DiRectX applications, such as, clean up
// and debugging code.
//=============================================================================

#ifndef D3DUTIL_H
#define D3DUTIL_H

// Enable extra D3D debugging in debug builds if using the debug DiRectX runtime.  
// This makes D3D objects work well in the debugger watch window, but slows down 
// performance slightly.

#if defined(DEBUG) | defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif

#ifdef _DEBUG
#   pragma comment(lib, "d3dx9d.lib")
#else
#   pragma comment(lib, "d3dx9.lib")
#endif
#pragma comment(lib, "d3d9.lib")

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <string>
#include <sstream>
#include <vector>
//#include "Runnable.h"

class Material;

const float EPSILON  = 0.001f;

struct Dimensions
{
	Dimensions(float s)
		: width(s), height(s), depth(s) {}

	Dimensions()
		: width(1.0f), height(1.0f), depth(1.0f){}

	Dimensions(float w, float h, float d)
		: width(w), height(h), depth(d) {}

	const Dimensions operator+(const Dimensions &rhs) {
		this->width += rhs.width;
		this->height += rhs.height;
		this->depth += rhs.depth;
		return *this;
	}

	float width;
	float height;
	float depth;
};

//===============================================================
// .X Files

void LoadXFile(
	const std::string& filename, 
	ID3DXMesh** meshOut, 
	std::vector<Material>& mtrls, 
	std::vector<IDirect3DTexture9*>& texs);

//===============================================================
// Globals for convenient access.
extern IDirect3DDevice9*		gd3dDevice;

//===============================================================
// Clean up

#define ReleaseCOM(x) { if(x){ x->Release();x = 0; } }

//===============================================================
// Debug

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                      \
	{                                                  \
		HRESULT hr = x;                                \
		if(FAILED(hr))                                 \
		{											   \
			MessageBox(0, "HR Erro! (means trouble)", 0, 0);		   \
			DXTrace(__FILE__, __LINE__, hr, #x, TRUE); \
		}                                              \
	}
	#endif

#else
	#ifndef HR
	#define HR(x) x;
	#endif
#endif 

#endif // D3DUTIL_H