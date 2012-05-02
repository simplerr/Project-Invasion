#include "RenderTarget.h"
#include "Runnable.h"

RenderTarget::RenderTarget(int width, int height)
{
	mTexture = NULL;
	mRenderToSurface = NULL;
	mTopSurface = NULL;
	mWidth = width;
	mHeight = height;

	onResetDevice();
}

RenderTarget::~RenderTarget()
{
	onLostDevice();
}

void RenderTarget::onLostDevice()
{
	mRenderToSurface->OnLostDevice();
	ReleaseCOM(mTexture);
	ReleaseCOM(mRenderToSurface);
	ReleaseCOM(mTopSurface);
}

void RenderTarget::onResetDevice()
{
	UINT usage = D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP;

	D3DVIEWPORT9 vp = {0, 0, mWidth, mHeight, 0.0f, 1.0f};
	HR(D3DXCreateTexture(gd3dDevice, mWidth, mHeight, 0, usage, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &mTexture));
	HR(D3DXCreateRenderToSurface(gd3dDevice, mWidth, mHeight, D3DFMT_X8R8G8B8, true, D3DFMT_D24X8, &mRenderToSurface));
	HR(mTexture->GetSurfaceLevel(0, &mTopSurface));

	mRenderToSurface->OnResetDevice();
}

IDirect3DTexture9* RenderTarget::getTexture()
{
	return mTexture;
}

IDirect3DSurface9* RenderTarget::getSurface()
{
	return mTopSurface;
}