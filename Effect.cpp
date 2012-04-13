#include "Effect.h"
#include "Game.h"
#include "Light.h"
#include "World.h"

Effect::Effect(string effect, string tech)
{
	// Create the FX from a .fx file.
	ID3DXBuffer* errors = 0;
	HR(D3DXCreateEffectFromFile(gd3dDevice, effect.c_str(), 
		0, 0, D3DXSHADER_DEBUG, 0, &mFX, &errors));
	if( errors )
		MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);

	// Obtain handles.
	mhTech		= mFX->GetTechniqueByName(tech.c_str());
	mhWVP           = mFX->GetParameterByName(0, "gWVP");
	mhWorld			= mFX->GetParameterByName(0, "gWorld");
	mhTex           = mFX->GetParameterByName(0, "gTex");
	mhWorldInverseTranspose = mFX->GetParameterByName(0, "gWorldInverseTranspose");
	mhEyePos		= mFX->GetParameterByName(0, "gEyePosW");
	mhObjectMaterial  = mFX->GetParameterByName(0, "gObjectMaterial");
	mhLights		= mFX->GetParameterByName(0, "gLights");
	mhNumLights		= mFX->GetParameterByName(0, "gNumLights");
}
	
Effect::~Effect()
{

}
	
void Effect::setBaseData(IDirect3DTexture9* texture, D3DXMATRIX world, Material material)
{
	// Get the lights in the world.
	Game* game = (Game*)gGame;
	//vector<Light*> lights = game->getWorld()->getLights();	// NOTE: Maybe return by reference to make faster?

	Light lights1[4];
	for(int i = 0; i < game->getWorld()->mLightList.size(); i++)
		lights1[i] = *game->getWorld()->mLightList[i];

	// Effect data
	HR(mFX->SetMatrix(mhWVP, &(world*gCamera->getViewMatrix()*gCamera->getProjectionMatrix())));
	HR(mFX->SetMatrix(mhWorld, &world));
	HR(mFX->SetVector(mhEyePos, &D3DXVECTOR4(gCamera->getPosition(), 0)));
	HR(mFX->SetTexture(mhTex, texture));
	D3DXMATRIX worldInverseTranspose;
	D3DXMatrixInverse(&worldInverseTranspose, 0, &world);
	D3DXMatrixTranspose(&worldInverseTranspose, &worldInverseTranspose);
	HR(mFX->SetMatrix(mhWorldInverseTranspose, &worldInverseTranspose));
	HR(mFX->SetInt(mhNumLights, game->getWorld()->mLightList.size()));

	// Object material and light array
	HR(mFX->SetRawValue(mhObjectMaterial, (void*)&material, 0, sizeof(Material)));
	HR(mFX->SetRawValue(mhLights, (void*)&lights1, 0, sizeof(Light)*game->getWorld()->mLightList.size()));
}