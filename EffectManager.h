#pragma once

#include <vector>
using namespace std;
class Effect;

enum EffectId
{
	TEXTURE_EFFECT,
	PARTICLE_EFFECT
};

class EffectManager
{
public:
	EffectManager();
	~EffectManager();

	void initEffects();
	Effect* getEffect(EffectId id);

	void onLostDevice();
	void onResetDevice();
private:
	vector<Effect*> mEffectList;
};

extern EffectManager* gEffectManager;