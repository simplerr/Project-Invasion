#include "Effect.h"

Effect::Effect(string filename, string technique, EffectId id)
{
	mFilename = filename;
	mTechnique = technique;
	mId = id;

	// Create the FX.
	ID3DXBuffer* errors = 0;
	HR(D3DXCreateEffectFromFile(gd3dDevice, filename.c_str(),
		0, 0, D3DXSHADER_DEBUG, 0, &mFX, &errors));
	if(errors)
		MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);

	mhTech    = mFX->GetTechniqueByName(technique.c_str());
	mhWVP     = mFX->GetParameterByName(0, "gWVP");
	mhEyePosL = mFX->GetParameterByName(0, "gEyePosL");
	mhTex     = mFX->GetParameterByName(0, "gTex");
	mhTime    = mFX->GetParameterByName(0, "gTime");
	mhAccel   = mFX->GetParameterByName(0, "gAccel");
	mhViewportHeight = mFX->GetParameterByName(0, "gViewportHeight");
}
	
Effect::~Effect()
{
	ReleaseCOM(mFX);
}

void Effect::init()
{

}
	
void Effect::begin()
{
	UINT numPasses = 0;
	HR(mFX->Begin(&numPasses, 0));
	HR(mFX->BeginPass(0));
}
	
void Effect::end()
{
	HR(mFX->EndPass());
	HR(mFX->End());
}

void Effect::setTech()
{
	HR(mFX->SetTechnique(mhTech));
}

void Effect::setTexture(IDirect3DTexture9* texture)
{
	HR(mFX->SetTexture(mhTex, texture));
}

void Effect::setAccel(D3DXVECTOR3 accel)
{
	HR(mFX->SetValue(mhAccel, accel, sizeof(D3DXVECTOR3)));
}

void Effect::setEyePos(D3DXVECTOR3 eyePos)
{
	HR(mFX->SetValue(mhEyePosL, &eyePos, sizeof(D3DXVECTOR3)));
}
	
void Effect::setTime(float time)
{
	HR(mFX->SetFloat(mhTime, time));
}
	
void Effect::setWVP(D3DXMATRIX wvp)
{
	HR(mFX->SetMatrix(mhWVP, &wvp));
}
	
void Effect::setViewportHeight(float height)
{
	HR(mFX->SetInt(mhViewportHeight, height));
}

EffectId Effect::getId()
{
	return mId;
}