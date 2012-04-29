//=============================================================================
// ParticleSystem.cpp by Frank Luna (C) 2004 All Rights Reserved.
//=============================================================================

#include <cassert>
#include "ParticleSystem.h"
#include "Camera.h"
#include "d3dUtil.h"
#include "EffectManager.h"
#include "Effect.h"
#include "Game.h"

ParticleSystem::ParticleSystem(D3DXVECTOR3 position, string fxName, string techName, string texName, D3DXVECTOR3 accel, AABB box, int maxNumParticles, float timePerParticle)
	 : Object3D(position, PARTICLE_SYSTEM), mAccel(accel), mBox(box), mTime(0.0f),
	   mMaxNumParticles(maxNumParticles), mTimePerParticle(timePerParticle)
{
	// Allocate memory for maximum number of particles.
	mParticles.resize(mMaxNumParticles);
	mAliveParticles.reserve(mMaxNumParticles);
	mDeadParticles.reserve(mMaxNumParticles);

	// They start off all dead.
	for(int i = 0; i < mMaxNumParticles; ++i)
	{
		mParticles[i].lifeTime = -1.0f;
		mParticles[i].initialTime = 0.0f;
	}

	// Create the texture.
	HR(D3DXCreateTextureFromFile(gd3dDevice, texName.c_str(), &mTex));

	// Create the FX.
	mEffect = gEffectManager->getEffect(PARTICLE_EFFECT);

	// We don't need to set these every frame since they do not change.
	mEffect->setAccel(mAccel);
	mEffect->setTexture(mTex);

	HR(gd3dDevice->CreateVertexBuffer(mMaxNumParticles*sizeof(Particle),
		D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY|D3DUSAGE_POINTS,
		0, D3DPOOL_DEFAULT, &mVB, 0));
	
	setLifetime(5.0f);
}

ParticleSystem::~ParticleSystem()
{
	delete mEffect;
	ReleaseCOM(mTex);
	ReleaseCOM(mVB);
}

float ParticleSystem::getTime()
{
	return mTime;
}

void  ParticleSystem::setLifetime(float lifetime)
{
	mLifetime = lifetime;
}

void ParticleSystem::setTime(float t)
{
	mTime = t;
}

const AABB& ParticleSystem::getAABB()const
{
	return mBox;
}

void ParticleSystem::addParticle()
{
	if(mDeadParticles.size() > 0)
	{
		// Reinitialize a particle.
		Particle* p = mDeadParticles.back();
		initParticle(*p);

		// No longer dead.
		mDeadParticles.pop_back();
		mAliveParticles.push_back(p);
	}
}

void ParticleSystem::onLostDevice()
{
	//HR(mFX->OnLostDevice());

	// Default pool resources need to be freed before reset.
	ReleaseCOM(mVB);
}

void ParticleSystem::onResetDevice()
{
	//HR(mFX->OnResetDevice());

	// Default pool resources need to be recreated after reset.
	/*if(mVB == 0)
	{
		HR(gd3dDevice->CreateVertexBuffer(mMaxNumParticles*sizeof(Particle),
			D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY|D3DUSAGE_POINTS,
			0, D3DPOOL_DEFAULT, &mVB, 0));
	}*/
}

void ParticleSystem::update(float dt)
{
	mTime += dt;

	if(mTime >= mLifetime)
		kill();

	// Rebuild the dead and alive list.  Note that resize(0) does
	// not deallocate memory (i.e., the capacity of the vector does
	// not change).
	mDeadParticles.resize(0);
	mAliveParticles.resize(0);

	// For each particle.
	for(int i = 0; i < mMaxNumParticles; ++i)
	{
		// Is the particle dead?
  		if( (mTime - mParticles[i].initialTime) > mParticles[i].lifeTime)
		{
			mDeadParticles.push_back(&mParticles[i]);
		}
		else
		{
			mAliveParticles.push_back(&mParticles[i]);
		}
	}


	// A negative or zero mTimePerParticle value denotes
	// not to emit any particles.
	if( mTimePerParticle > 0.0f )
	{
		// Emit particles.
		static float timeAccum = 0.0f;
		timeAccum += dt;
		while( timeAccum >= mTimePerParticle )
		{
			addParticle();
			timeAccum -= mTimePerParticle;
		}
	}
}

void ParticleSystem::draw()
{
	mEffect->setTech();

	// Get camera position relative to world space system and make it 
	// relative to the particle system's local system.
	D3DXVECTOR3 eyePosW = gCamera->getPosition();
	D3DXVECTOR3 eyePosL;
	D3DXVec3TransformCoord(&eyePosL, &eyePosW, &getInvWorldMatrix());

	// Set FX parameters.
	mEffect->setEyePos(eyePosL);
	mEffect->setTime(mTime);
	mEffect->setWVP(getWorldMatrix()*gCamera->getViewMatrix()*gCamera->getProjectionMatrix());

	HWND hwnd = gGame->getMainWnd();
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	mEffect->setViewportHeight(clientRect.bottom);

	mEffect->begin();

	HR(gd3dDevice->SetStreamSource(0, mVB, 0, sizeof(Particle)));
	HR(gd3dDevice->SetVertexDeclaration(Particle::Decl));

	// Initial lock of VB for writing.
	Particle* p = 0;
	HR(mVB->Lock(0, 0, (void**)&p, D3DLOCK_DISCARD));
	int vbIndex = 0;

	// For each living particle.
	for(UINT i = 0; i < mAliveParticles.size(); ++i)
	{
		// Copy particle to VB
		p[vbIndex] = *mAliveParticles[i];
		++vbIndex;
	}
	HR(mVB->Unlock());

	// Render however many particles we copied over.
	if(vbIndex > 0)
	{
		HR(gd3dDevice->DrawPrimitive(D3DPT_POINTLIST, 0, vbIndex));
	}

	mEffect->end();
}
