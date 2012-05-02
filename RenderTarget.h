#pragma once

#include "d3dUtil.h"

class RenderTarget
{
public:
	RenderTarget(int width, int height);
	~RenderTarget();

	IDirect3DTexture9* getTexture();
	IDirect3DSurface9* getSurface();

	void onLostDevice();
	void onResetDevice();
private:
	IDirect3DTexture9*    mTexture;
	ID3DXRenderToSurface* mRenderToSurface;
	IDirect3DSurface9*    mTopSurface;

	int         mWidth;
	int         mHeight;
	int         mMipLevels;
};
