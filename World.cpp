#include "World.h"
#include "Object3D.h"
#include "Light.h"
#include "Graphics.h"
#include "Terrain.h"
#include "Camera.h"
#include "Input.h"
#include "Vertex.h"
#include "Sky.h"
#include "Terrain.h"
#include "Mesh.h"

World::World()
{
	// Create the terrain.
	mTerrain = new Terrain(D3DXVECTOR3(0, 0, 0), 257, 257, 100, 100, 16.0f);

	// Create the skybox.
	mSky = new Sky("data/grassenvmap1024.dds", 10000.0f);
	setGravity(-0.023f);
}
	
World::~World()
{
	// Cleanup.
	delete mTerrain;
	delete mSky;

	for(int i = 0; i < mObjectList.size(); i++)
		delete mObjectList[i];

	for(int i = 0; i < mLightList.size(); i++)
		delete mLightList[i];
}

void World::onLostDevice()
{
	for(int i = 0; i < mObjectList.size(); i++)
		mObjectList[i]->onLostDevice();

	mSky->onLostDevice();
}
	
void World::onResetDevice()
{
	for(int i = 0; i < mObjectList.size(); i++)
		mObjectList[i]->onResetDevice();

	mSky->onResetDevice();
}

void World::update(float dt)
{
	// Update all the objects.
	for(int i = 0; i < mObjectList.size(); i++)
	{
		if(!mObjectList[i]->getAlive())
			continue;
		Object3D* object = mObjectList[i];

		// Get the distance above the ground.
		float distance = object->getPosition().y - mTerrain->getHeight(object->getPosition().x, object->getPosition().z);

		// Snap to ground.
		// Note the +50 to give it some margin (only for enemies).
		float margin = object->getType() != PLAYER ? 50 : 0;
		if(distance < object->getHeightOffset() + margin && object->getVelocity().y <= 0)
		{
			object->setPosition(object->getPosition() - D3DXVECTOR3(0, distance, 0) + D3DXVECTOR3(0, object->getHeightOffset(), 0));
			object->setVelocity(D3DXVECTOR3(object->getVelocity().x, 0, object->getVelocity().z));
		}

		// Set on ground.
		if(distance < object->getHeightOffset() + 0.2)
			object->setOnGround(true);
		else
			object->setOnGround(false);
	
		// Update the object.
		object->update(dt);

		// Gravity.
		object->accelerate(0, getGravity(), 0);

		// Friction.
		if(object->getOnGround())
		{
			float friction = 0.12f;
			D3DXVECTOR3 velocity = object->getVelocity() * -friction;
			object->accelerate(velocity.x, 0.0f, velocity.z);
		}

		// Move the object with it's speed. 
		// [NOTE] Only update the objects position here! Change the speed on other places instead!
		D3DXVECTOR3 speed = object->getVelocity();
		object->move(speed.x, speed.y, speed.z);
	}

	// Update the camera.
	gCamera->update(mTerrain, dt);

	// Terrain editing.
	// [NOTE] Disabled.
	//editTerrain();
}
	
void World::draw()
{
	// Draw the skybox.
	mSky->draw();

	// Draw the terrain.
	mTerrain->draw();

	// Draw all the objects.
	for(int i = 0; i < mObjectList.size(); i++) {
		if(!mObjectList[i]->getAlive())
			continue;
		mObjectList[i]->draw();
	}

	//gGraphics->drawRay(gCamera->getPosition(), gCamera->getDirection(), 200.0f, 20.0f);
	/*for(int i = 0; i < mLightList.size(); i++) {
		D3DXMATRIX m;
		D3DXMatrixIdentity(&m);
		gGraphics->drawTexturedCube(NULL, mLightList[i]->getPosition(), Dimensions(3, 3,3), Material(), 0, true, m);
	}*/
}

void World::editTerrain()
{
	BOOL hit = false;
	DWORD faceIndex, hitCount;
	float u, v, dist;
	LPD3DXBUFFER allHits;
	ID3DXMesh* mesh = mTerrain->getMesh()->getMesh();

	if(gInput->keyDown(VK_LBUTTON) || gInput->keyDown(VK_RBUTTON))
	{
		float strength = 300.0f;
		if(gInput->keyDown(VK_RBUTTON))
			strength *= -1;  

		D3DXVECTOR3 origin, dir;
		gInput->getWorldPickingRay(origin, dir);

		D3DXIntersect(mesh, &origin, &dir, &hit, &faceIndex, &u, &v, &dist, &allHits, &hitCount);

		if(hit)
		{
			DWORD* k = 0;

			mesh->LockIndexBuffer(0, (void**)&k);
			int pressed = k[faceIndex*3];
			mesh->UnlockIndexBuffer();

			VertexPNT* v = 0;
			mesh->LockVertexBuffer(0, (void**)&v);

			int size = 10;

			int x = pressed % mTerrain->getColums();
			int z = pressed / mTerrain->getColums();

			for(int i = z - size/2; i < z + size/2; i++)
				for(int j = x - size/2; j < x + size/2; j++) {
					if(i < 0 || j < 0 || i >= mTerrain->getRows() || j >= mTerrain->getColums())
						continue;

					float dist = sqrt((float)(z-i)*(z-i) + (float)(x-j)*(x-j));
					dist = max(dist, 3.0f);

					if(strength > 0)
						v[i*mTerrain->getColums() + j].pos.y += min(strength / dist, strength)/100;
					else
						v[i*mTerrain->getColums() + j].pos.y += max(strength / dist, strength)/100;
				}

			mesh->UnlockVertexBuffer();

			//mTerrain->smothOut(x, z, 10);
		}
	}

	if(gInput->keyPressed(VK_MBUTTON)) {
		D3DXIntersect(mesh, &gCamera->getPosition(), &gCamera->getDirection(), &hit, &faceIndex, &u, &v, &dist, &allHits, &hitCount);

		if(hit)
		{
			DWORD* k = 0;

			mesh->LockIndexBuffer(0, (void**)&k);
			int pressed = k[faceIndex*3];
			mesh->UnlockIndexBuffer();

			VertexPNT* v = 0;
			mesh->LockVertexBuffer(0, (void**)&v);

			int size = 10;

			int x = pressed % mTerrain->getColums();
			int z = pressed / mTerrain->getColums();


			mTerrain->smothOut(x, z, 5);
		}
	}
}

Object3D* World::getIntersectedObject(D3DXVECTOR3 position, D3DXVECTOR3 direction, DWORD& faceIndex, float& distance)
{
	Object3D* closestObject = NULL;
	float closestDist = 9999999;
	for(int i = 1; i < mObjectList.size(); i++)
	{
		if(mObjectList[i]->getAlive() && mObjectList[i]->rayIntersectAABB(gCamera->getPosition(), gCamera->getDirection()))
		{
			mObjectList[i]->rayIntersectMesh(gCamera->getPosition(), gCamera->getDirection(), faceIndex, distance);
			if(distance < closestDist) {
				closestObject = mObjectList[i];
				closestDist = distance;
			}
		}
	}

	return closestObject;
}

void World::addObject(Object3D* object)
{
	// Add object to back of list.
	object->setWorld(this);
	mObjectList.push_back(object);
}
	
void World::addLight(Light* light)
{
	// Add light to back of list.
	mLightList.push_back(light);
}

vector<Light*>& World::getLights()
{
	return mLightList;
}

void World::addAmbientLight(D3DXCOLOR color)
{
	// Create and add an ambient light.
	Light* light = new Light();
	light->setMaterial(Material(color, D3DXCOLOR(0, 0, 0, 0), D3DXCOLOR(0, 0, 0, 0)));
	light->setPosition(D3DXVECTOR3(0, 0, 0));
	light->setDirection(D3DXVECTOR3(0, 0, 0));
	light->setSpotPower(0.0f);
	light->setLength(10000.0f);
	addLight(light);
}

Terrain* World::getTerrain()
{
	return mTerrain;
}

void World::setGravity(float gravity)
{
	mGravity = gravity;
}
	
float World::getGravity()
{
	return mGravity;
}

