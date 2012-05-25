#pragma once

#include <vector>
#include "d3dUtil.h"
#include "EnemyHandler.h"
using namespace std;

class Object3D;
class Light;
class Terrain;
class Sky;
class RenderTarget;
enum ObjectType;

class World
{
public:
	World();
	~World();

	void update(float dt);
	void draw();
	void drawToMinimap(RenderTarget* renderTarget);

	void editTerrain();
	void reset();

	void addObject(Object3D* object);
	void addLight(Light* light);
	void addAmbientLight(D3DXCOLOR color);

	void removeObject(Object3D* object);

	void getObjectsInRadius(vector<Object3D*>& objects, D3DXVECTOR3 position, float radius, ObjectType type);

	void setGravity(float gravity);
	float getGravity();
	EnemyData getEnemyData(string name);

	vector<Light*>* getLights();

	Terrain* getTerrain();
	Object3D* getIntersectedObject(D3DXVECTOR3 position, D3DXVECTOR3 direction, DWORD& faceIndex, float& distance);

	void onLostDevice();
	void onResetDevice();

private:
	vector<Object3D*>		mObjectList;
	vector<Light*>			mLightList;
	EnemyHandler*			mEnemyHandler;
	Terrain*				mTerrain;
	Sky*					mSky;
	float					mGravity;
};