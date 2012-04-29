#include "EffectManager.h"
#include "Effect.h"

EffectManager::EffectManager()
{
	initEffects();
}

EffectManager::~EffectManager()
{
	for(int i = 0; i < mEffectList.size(); i++)
		delete mEffectList[i];
}

void EffectManager::initEffects()
{
	mEffectList.push_back(new Effect("particle.fx", "ParticleTech", PARTICLE_EFFECT));
}
	
Effect* EffectManager::getEffect(EffectId id)
{
	for(int i = 0; i < mEffectList.size(); i++)
	{
		if(mEffectList[i]->getId() == id)
			return mEffectList[i];
	}
}

void EffectManager::onLostDevice()
{
	for(int i = 0; i < mEffectList.size(); i++)
		mEffectList[i]->onLostDevice();
}
	
void EffectManager::onResetDevice()
{
	for(int i = 0; i < mEffectList.size(); i++)
		mEffectList[i]->onResetDevice();
}