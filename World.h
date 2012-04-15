#pragma once

#include <vector>
#include "d3dUtil.h"
using namespace std;

class Object3D;
class Light;
class Terrain;
class Sky;

class World
{
public:
	World();
	~World();

	void update(float dt);
	void draw();

	void editTerrain();

	void addObject(Object3D* object);
	void addLight(Light* light);
	void addAmbientLight(D3DXCOLOR color);

	void setGravity(float gravity);
	float getGravity();

	vector<Light*>& getLights();

	Terrain* getTerrain();

	void onLostDevice();
	void onResetDevice();

	// TODO: Add private
	vector<Object3D*>		mObjectList;
	vector<Light*>			mLightList;
	Terrain*				mTerrain;
	Sky*					mSky;
	float					mGravity;
};