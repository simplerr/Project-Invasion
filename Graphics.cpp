#include <tchar.h>
#include "Graphics.h"
#include "Vertex.h"
#include "Camera.h"
#include "BufferFactory.h"
#include "Game.h"
#include "Material.h"
#include "Object3D.h"
#include "Light.h"
#include "World.h"
#include "SkinnedMesh.h"
#include "Mesh.h"
#include "Terrain.h"

Graphics::Graphics()
{
	mBufferFactory = new BufferFactory();

	initBuffers();
	initEffect();
	initFonts();

	mWhiteTexture = loadTexture("data/white.bmp");

	// TESTING.. 
	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	UINT numElems = 0;
	VertexPNT::Decl->GetDeclaration(elems, &numElems);
	D3DXCreateMesh(2, 4, D3DXMESH_MANAGED, elems, gd3dDevice, &mRayMesh);

	// Ortho projection when not using shader pipeline.
	D3DXMATRIX m;
    D3DXMatrixIdentity(&m);

	gd3dDevice->SetTransform(D3DTS_WORLD, &m);
    gd3dDevice->SetTransform(D3DTS_VIEW, &m);

    D3DXMatrixOrthoOffCenterRH (&m, 0, 1200, 800, 0, 0, 1);
    gd3dDevice->SetTransform(D3DTS_PROJECTION, &m);
}
	
Graphics::~Graphics()
{
	delete mBufferFactory;
	ReleaseCOM(mFont);
	ReleaseCOM(mCubeIB);
	ReleaseCOM(mCubeVB);
	ReleaseCOM(mFX);
}

void Graphics::onLostDevice()
{
	mFX->OnLostDevice();
	mFont->OnLostDevice();
}
	
void Graphics::onResetDevice()
{
	mFX->OnResetDevice();
	mFont->OnResetDevice();
}

IDirect3DTexture9* Graphics::loadTexture(string filename, DWORD colorKey)
{
	// Already loaded?
	if(mTextureMap.find(filename) != mTextureMap.end())
		return mTextureMap[filename];

	IDirect3DTexture9 *texture;		// Texture to return.
	D3DXIMAGE_INFO SrcInfo;			// Optional.

	// Load texture from file image.
	if (FAILED(D3DXCreateTextureFromFileEx(gd3dDevice, filename.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, 
        D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
        colorKey, &SrcInfo, NULL, &texture)))
	{
		char buffer[256];
		sprintf(buffer, "Error loading texture: %s", filename);
		MessageBox(0, buffer, 0, 0);
		return NULL;
	}

	// Add to the texture map
	mTextureMap[filename] = texture;
	return texture;
}

void Graphics::initEffect()
{
	// Create the FX from a .fx file.
	ID3DXBuffer* errors = 0;
	HR(D3DXCreateEffectFromFile(gd3dDevice, "effects.fx", 
		0, 0, D3DXSHADER_DEBUG, 0, &mFX, &errors));
	if( errors )
		MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);

	// Default handles
	mhTexTech		= mFX->GetTechniqueByName("TexTech");
	mhWVP           = mFX->GetParameterByName(0, "gWVP");
	mhWorld			= mFX->GetParameterByName(0, "gWorld");
	mhTex           = mFX->GetParameterByName(0, "gTex");
	mhWorldInverseTranspose = mFX->GetParameterByName(0, "gWorldInverseTranspose");
	mhEyePos		= mFX->GetParameterByName(0, "gEyePosW");
	mhObjectMaterial  = mFX->GetParameterByName(0, "gObjectMaterial");
	mhLights		= mFX->GetParameterByName(0, "gLights");
	mhNumLights		= mFX->GetParameterByName(0, "gNumLights");

	// Terrain handles
	mhTerrainTech   = mFX->GetTechniqueByName("TerrainTech");
	mhTex0          = mFX->GetParameterByName(0, "gTex0");
	mhTex1          = mFX->GetParameterByName(0, "gTex1");
	mhTex2          = mFX->GetParameterByName(0, "gTex2");
	mhBlendMap      = mFX->GetParameterByName(0, "gBlendMap");

	// Skinned mesh handles
	mhSkinnedMeshTech = mFX->GetTechniqueByName("SkinnedMeshTech");
	mhFinalXForms     = mFX->GetParameterByName(0, "gFinalXForms");
	mhBoneTransform   = mFX->GetParameterByName(0, "gBoneTransform");

	// Load textures from file - TODO: move to Terrain
	mTex0 = loadTexture("data/grass.dds");
	mTex1 = loadTexture("data/stone.dds");
	mTex2 = loadTexture("data/dirt.dds");
	mBlendMap = loadTexture("data/blendmap.jpg");
}

void Graphics::initBuffers()
{
	// Let the buffer factory build the buffers
	mCubeVB = mBufferFactory->coloredCubeBuffer();
	mTexturedCubeVB = mBufferFactory->textureCubeBuffer();
	mCubeIB = mBufferFactory->coloredCubeIndices();
	mTexturedCubeIB = mBufferFactory->texturedCubeIndices();
	HR(gd3dDevice->CreateVertexBuffer(24 * sizeof(VertexPNT), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &mTexturedRectangleVB, 0));
}

void Graphics::initFonts()
{
	D3DXFONT_DESC fontDesc;
	fontDesc.Height          = 18;
    fontDesc.Width           = 8;
    fontDesc.Weight          = 0;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = 10;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
	 _tcscpy(fontDesc.FaceName, _T("Bitstream Vera Sans Mono"));

	HR(D3DXCreateFontIndirect(gd3dDevice, &fontDesc, &mFont));
}

void Graphics::setEffectData(IDirect3DTexture9* texture, D3DXVECTOR3 position, Dimensions dimensions, Material material, float rotation, D3DXMATRIX m, bool useM)
{
	// Setup the world matrix
	D3DXMATRIX T, R, S;
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);
	D3DXMatrixRotationX(&R, rotation);

	if(useM)
		R *= m;

	D3DXMatrixScaling(&S, dimensions.width, dimensions.height, dimensions.depth);
	D3DXMATRIX world = (S*R*(T));

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

void Graphics::setEffectParameters(D3DXMATRIX world)
{
	// Get the lights in the world.
	Game* game = (Game*)gGame;

	Light lights1[4];
	for(int i = 0; i < game->getWorld()->mLightList.size(); i++)
		lights1[i] = *game->getWorld()->mLightList[i];

	// Calculate the inverse transpose matrix.
	D3DXMATRIX worldInverseTranspose;
	D3DXMatrixInverse(&worldInverseTranspose, 0, &world);
	D3DXMatrixTranspose(&worldInverseTranspose, &worldInverseTranspose);

	// Set the parameters.
	HR(mFX->SetMatrix(mhWVP, &(world*gCamera->getViewMatrix()*gCamera->getProjectionMatrix())));
	HR(mFX->SetMatrix(mhWorld, &world));
	HR(mFX->SetVector(mhEyePos, &D3DXVECTOR4(gCamera->getPosition(), 0)));
	HR(mFX->SetMatrix(mhWorldInverseTranspose, &worldInverseTranspose));
	HR(mFX->SetInt(mhNumLights, game->getWorld()->mLightList.size()));
	HR(mFX->SetRawValue(mhLights, (void*)&lights1, 0, sizeof(Light)*game->getWorld()->mLightList.size()));

	// Set the DEFUALT = WHITE object material and the light array (max 4 lights right now).
	HR(mFX->SetRawValue(mhObjectMaterial, (void*)&Material(), 0, sizeof(Material)));
}

void Graphics::drawTexturedCube(IDirect3DTexture9* texture, D3DXVECTOR3 position, Dimensions dimensions, Material material, float rotation, bool solid, D3DXMATRIX m)
{
	// Set effect data
	HR(mFX->SetTechnique(mhTexTech));
	setEffectData(texture, position, dimensions, material, rotation, m);

	// Set stream source and vertex declarations
	HR(gd3dDevice->SetVertexDeclaration(VertexPNT::Decl));
	HR(gd3dDevice->SetStreamSource(0, mTexturedCubeVB, 0, sizeof(VertexPNT)));
	HR(gd3dDevice->SetIndices(mTexturedCubeIB));

	// Begin passes.
	UINT numPasses = 0;
	HR(mFX->Begin(&numPasses, 0));
	for(UINT i = 0; i < numPasses; ++i)
	{
		HR(mFX->BeginPass(i));
		HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12));
		HR(mFX->EndPass());
	}
	HR(mFX->End());
}

void Graphics::drawBoundingBox(Object3D* object, D3DXCOLOR color, float opacity)
{
	// Set effect data
	HR(mFX->SetTechnique(mhTexTech));
	Material material(color, 8.0f);
	material.diffuse.a = opacity;

	D3DXMATRIX S;
	D3DXMatrixScaling(&S, object->getAABB().getDimensions().width, object->getAABB().getDimensions().height, object->getAABB().getDimensions().depth);

	setEffectParameters(S*object->getWorldMatrix());

	// Set stream source and vertex declarations
	HR(gd3dDevice->SetVertexDeclaration(VertexPNT::Decl));
	HR(gd3dDevice->SetStreamSource(0, mTexturedCubeVB, 0, sizeof(VertexPNT)));
	HR(gd3dDevice->SetIndices(mTexturedCubeIB));

	HR(gd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true));
	HR(gd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
	HR(gd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA));

	HR(mFX->SetTexture(mhTex, mWhiteTexture));
	HR(mFX->SetRawValue(mhObjectMaterial, (void*)&material, 0, sizeof(Material)));

	// Begin passes.
	UINT numPasses = 0;
	HR(mFX->Begin(&numPasses, 0));
	for(UINT i = 0; i < numPasses; ++i)
	{
		HR(mFX->BeginPass(i));
		HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12));
		HR(mFX->EndPass());
	}
	HR(mFX->End());

	HR(gd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false));
}

void Graphics::drawBoundingBox(D3DXVECTOR3 position, Dimensions dimensions, D3DXCOLOR color, float opacity)
{
	// Set effect data
	HR(mFX->SetTechnique(mhTexTech));
	Material material(color, 8.0f);
	material.diffuse.a = opacity;
	setEffectData(mWhiteTexture, position, dimensions, material);

	// Set stream source and vertex declarations
	HR(gd3dDevice->SetVertexDeclaration(VertexPNT::Decl));
	HR(gd3dDevice->SetStreamSource(0, mTexturedCubeVB, 0, sizeof(VertexPNT)));
	HR(gd3dDevice->SetIndices(mTexturedCubeIB));

	HR(gd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true));
	HR(gd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
	HR(gd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA));

	// Begin passes.
	UINT numPasses = 0;
	HR(mFX->Begin(&numPasses, 0));
	for(UINT i = 0; i < numPasses; ++i)
	{
		HR(mFX->BeginPass(i));
		HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12));
		HR(mFX->EndPass());
	}
	HR(mFX->End());

	HR(gd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false));
}

void Graphics::drawTerrain(Terrain* terrain)
{
	// Set effect data
	HR(mFX->SetTechnique(mhTerrainTech));
	D3DXMATRIX m;
	D3DXMatrixIdentity(&m);
	setEffectData(mTex0, terrain->getPosition(), Dimensions(1, 1, 1), terrain->getMaterial(), 0, m);
	
	// Texture handles
	HR(mFX->SetTexture(mhTex0, mTex0));
	HR(mFX->SetTexture(mhTex1, mTex1));
	HR(mFX->SetTexture(mhTex2, mTex2));
	HR(mFX->SetTexture(mhBlendMap, mBlendMap));

	// Begin passes.
	UINT numPasses = 0;
	HR(mFX->Begin(&numPasses, 0));
	for(UINT i = 0; i < numPasses; ++i)
	{
		HR(mFX->BeginPass(i));
		terrain->getMesh()->getMesh()->DrawSubset(0);
		HR(mFX->EndPass());
	}
	HR(mFX->End());
}

void Graphics::drawSkinnedMesh(SkinnedMesh* skinnedMesh)
{
	// Set effect data
	HR(mFX->SetTechnique(mhTexTech));

	setEffectParameters(skinnedMesh->getWorldMatrix());

	// Begin passes.
	UINT numPasses = 0;
	HR(mFX->Begin(&numPasses, 0));
	for(UINT i = 0; i < numPasses; ++i)
	{
		HR(mFX->BeginPass(i));
		skinnedMesh->drawFrameHierarchy();
		HR(mFX->EndPass());
	}
	HR(mFX->End());
}

void Graphics::drawMesh(Mesh* mesh)
{
	HR(mFX->SetTechnique(mhTexTech));
	setEffectParameters(mesh->getWorldMatrix());

	HR(mFX->Begin(0, 0));
	HR(mFX->BeginPass(0));

	vector<IDirect3DTexture9*> textures = mesh->getTextures();
	ID3DXMesh* d3dmesh = mesh->getMesh();

	for(int i = 0; i < textures.size(); i++)
	{
		HR(mFX->SetTexture(mhTex, textures[i]));
		HR(mFX->CommitChanges());
		d3dmesh->DrawSubset(i);
	}

	HR(mFX->EndPass());
	HR(mFX->End());
}

void Graphics::drawRay(D3DXVECTOR3 start, D3DXVECTOR3 direction, float length, float width)
{
	// NOTE: The vertices are not positioned 100% correctly.
	VertexPNT* v;
	WORD* k;

	mRayMesh->LockVertexBuffer(0, (void**)&v);
	mRayMesh->LockIndexBuffer(0, (void**)&k);

	D3DXVECTOR3 right = gCamera->getRight();

	v[0].pos = start - right * width;
	v[1].pos = start + D3DXVECTOR3(0, -10, 0)  + right * width;
	v[2].pos = start + direction * length + right * width;
	v[3].pos = start + direction * length - right * width;

	v[0].tex0 = D3DXVECTOR2(0, 0);
	v[1].tex0 = D3DXVECTOR2(0, 1);
	v[2].tex0 = D3DXVECTOR2(1, 1);
	v[3].tex0 = D3DXVECTOR2(1, 0);

	k[0] = 0;
	k[1] = 1;
	k[2] = 2;
	k[3] = 0;
	k[4] = 2;
	k[5] = 3;

	mRayMesh->UnlockVertexBuffer();
	mRayMesh->UnlockIndexBuffer();

	mFX->SetTechnique(mhTex);
	//D3DXMATRIX I;
	//D3DXMatrixIdentity(&I);
	//setEffectParameters(I);
	setEffectData(mWhiteTexture, D3DXVECTOR3(0, 0, 0), Dimensions(), Material(BLUE, D3DXCOLOR(0, 0, 1, 0.5), BLUE));

	HR(gd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true));
	HR(gd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
	HR(gd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA));

	HR(mFX->Begin(0, 0));
	HR(mFX->BeginPass(0));

	mRayMesh->DrawSubset(0);

	HR(mFX->EndPass());
	HR(mFX->End());

	HR(gd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false));
}

void Graphics::drawScreenTexture(IDirect3DTexture9* texture, float x, float y, int width, int height)
{
	gd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	gd3dDevice->SetVertexDeclaration(VertexPNT::Decl);
	gd3dDevice->SetStreamSource(0, mTexturedRectangleVB, 0, sizeof(VertexPNT));	

	VertexPNT *vertices = 0;
	mTexturedRectangleVB->Lock(0, 0, (void**)&vertices, 0);

	vertices[0].pos.x = x - width/2;
	vertices[0].pos.y = y - height/2;
	vertices[0].pos.z = 0.0f;
	vertices[0].tex0.x = 0.0f;
	vertices[0].tex0.y = 0.0f;

	vertices[1].pos.x = x + width/2;
	vertices[1].pos.y = y - height/2;
	vertices[1].pos.z = 0.0f;
	vertices[1].tex0.x = 1.0f;
	vertices[1].tex0.y = 0.0f;

	vertices[2].pos.x = x + width/2;
	vertices[2].pos.y = y + height/2;
	vertices[2].pos.z = 0.0f;
	vertices[2].tex0.x = 1.0f;
	vertices[2].tex0.y = 1.0f;

	vertices[3].pos.x = x - width/2;
	vertices[3].pos.y = y + height/2;
	vertices[3].pos.z = 0.0f;
	vertices[3].tex0.x = 0.0f;
	vertices[3].tex0.y = 1.0f;

	// Unlock the vertex buffer.
	mTexturedRectangleVB->Unlock();

	// Set texture.
	gd3dDevice->SetTexture(0, texture);

	// Draw image.
	gd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	gd3dDevice->SetTexture(0, NULL);
	gd3dDevice->SetRenderState(D3DRS_LIGHTING, true);
}

void Graphics::drawText(string text, int x, int y, D3DCOLOR textColor)
{
	RECT pos = {x, y, 0, 0};
	HR(mFont->DrawText(0, text.c_str(), -1, &pos, DT_NOCLIP, textColor));
}

void Graphics::drawTest(ID3DXMesh* mesh, IDirect3DTexture9* texture, D3DXVECTOR3 position, D3DXVECTOR3 rotation)
{
	HR(mFX->SetTechnique(mhTexTech));

	D3DXMATRIX M;

	float rotY = atan2f(rotation.x, rotation.z);
	D3DXMatrixRotationY(&M, rotY);

	setEffectData(texture, position, Dimensions(500), Material(), 0, M, true);

	HR(mFX->Begin(0, 0));
	HR(mFX->BeginPass(0));

	mesh->DrawSubset(0);

	HR(mFX->EndPass());
	HR(mFX->End());
}