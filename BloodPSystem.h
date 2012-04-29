#pragma once
#include "ParticleSystem.h"

class BloodPSystem : public ParticleSystem
{
public:
	BloodPSystem(D3DXVECTOR3 position, const std::string& fxName, 
		const std::string& techName, 
		const std::string& texName, 
		const D3DXVECTOR3& accel, 
		const AABB& box,
		int maxNumParticles,
		float timePerSecond)
		: ParticleSystem(position, fxName, techName, texName, accel, box, maxNumParticles, timePerSecond)
	{
	}

	void initParticle(Particle& out)
	{
		// Generate about the origin.
		out.initialPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
 
		out.initialTime     = mTime;
		out.lifeTime        = 0.5f;
		out.initialSize		= 512.0f;
		out.mass            = 0.1f;
		out.initialColor	= RED;
		
		float speed = 150.0f;
		out.initialVelocity.x = GetRandomFloat(-1.0f, 1.0f);
		out.initialVelocity.y = GetRandomFloat(0.9f, 1.0f);
		out.initialVelocity.z = GetRandomFloat(-1.0f, 1.0f);
		out.initialVelocity *= speed;
	}
};