#pragma once

#include "d3dUtil.h"
#include "Vertex.h"
#include "AABB.h"
#include <vector>
#include <string>
#include "Object3D.h"
using namespace std;

class Effect;

//===============================================================
class ParticleSystem : public Object3D
{
public:
	ParticleSystem(D3DXVECTOR3 position, string fxName, string techName, string texName, D3DXVECTOR3 accel, AABB box, int maxNumParticles, float timePerParticle);
	virtual ~ParticleSystem();

	// Access Methods
	float getTime();
	void  setTime(float t);
	void  setLifetime(float lifetime);
	const AABB& getAABB()const;

	void addParticle();

	virtual void onLostDevice();
	virtual void onResetDevice();

	virtual void initParticle(Particle& out) = 0;
	virtual void update(float dt);
	virtual void draw();

protected:
	Effect* mEffect;
	IDirect3DTexture9* mTex;
	IDirect3DVertexBuffer9* mVB;
	D3DXVECTOR3 mAccel;
	AABB	mBox;
	float	mTime;
	float	mLifetime;
	int		mMaxNumParticles;
	float	mTimePerParticle;

	std::vector<Particle> mParticles;
	std::vector<Particle*> mAliveParticles;
	std::vector<Particle*> mDeadParticles; 
};