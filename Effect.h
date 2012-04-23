#pragma once

#include <string>
#include "d3dUtil.h"
#include "EffectManager.h"
using namespace std;

class Effect
{
public:
	Effect(string filename, string technique, EffectId id);
	virtual ~Effect();

	virtual void init();


	void begin();
	void end();

	void setTexture(IDirect3DTexture9* texture);
	void setAccel(D3DXVECTOR3 accel);
	void setEyePos(D3DXVECTOR3 eyePos);
	void setTime(float time);
	void setWVP(D3DXMATRIX wvp);
	void setViewportHeight(float height);
	void setTech();

	ID3DXEffect* getEffect();
	EffectId getId();
//private:
	ID3DXEffect* mFX;
	D3DXHANDLE mhTech;
	D3DXHANDLE mhWVP;
	D3DXHANDLE mhEyePosL;
	D3DXHANDLE mhTex;
	D3DXHANDLE mhTime;
	D3DXHANDLE mhAccel;
	D3DXHANDLE mhViewportHeight;

	EffectId mId;
	string mFilename;
	string mTechnique;
};