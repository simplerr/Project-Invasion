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
#include "EnemyHandler.h"
#include "PlayState.h"
#include "Wave.h"
#include "RenderTarget.h"
#include "Player.h"
#include "Powerup.h"

World::World()
{
	// Create the terrain.
	mTerrain = new Terrain(D3DXVECTOR3(0, 0, 0), 257, 257, 100, 100, 16.0f);

	// Create the skybox.
	mSky = new Sky("data/grassenvmap1024.dds", 10000.0f);
	setGravity(-0.13f);
}
	
World::~World()
{
	// Cleanup.
	delete mTerrain;
	delete mSky;

	for(int i = 0; i < mObjectList.size(); i++) {
		if(mObjectList[i]->getType() != SPAWNER) {
			delete mObjectList[i];
			mObjectList[i] = NULL;
		}
	}

	// [NOTE] Fix this!!
	//for(int i = 0; i < mLightList.size(); i++)
	//	delete mLightList[i];

	mObjectList.clear();
	//mLightList.clear();
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
	Player* player = NULL;

	// Update all the objects.
	for(int i = 0; i < mObjectList.size(); i++)
	{
		Object3D* object = mObjectList[i];
		// Remove from the list if dead.
		if(!object->getAlive()) {
			// Inform the current wave about the dead enemy. [TODO] Check if we are in PlayState.
			if(object->getType() == ENEMY)
				PlayState::Instance()->getCurrentWave()->enemyKilled();

			removeObject(object);
			continue;
		}

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
			D3DXVECTOR3 velocity = object->getVelocity() * -friction * object->getFriction();
			object->accelerate(velocity.x, 0.0f, velocity.z);
		}

		// Move the object with it's speed. 
		// [NOTE] Only update the objects position here! Change the speed on other places instead!
		D3DXVECTOR3 speed = object->getVelocity();
		object->move(speed.x, speed.y, speed.z);

		if(object->getType() == PLAYER)
			player = dynamic_cast<Player*>(object);
	}

	// [HACK]
	// Test if player is in range of a powerup.
	if(player != NULL) 
	{
		for(int i = 0; i < mObjectList.size(); i++) {
			if(mObjectList[i]->getType() == ENERGY_POWERUP) {
				D3DXVECTOR3 dist = mObjectList[i]->getPosition() - player->getPosition();
				dist.y = 0.0f;
				float d = sqrt(dist.x*dist.x + dist.z*dist.z);
				if(d < 70) // PICKUP_RADIUS
					(dynamic_cast<Powerup*>(mObjectList[i]))->pickup(player);
			}
			else
				continue;
		}
	}

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
		gGraphics->drawScreenTexturedCube(NULL, mLightList[i]->getPosition(), Dimensions(3, 3,3), Material(), 0, true, m);
	}*/
}

void World::getObjectsInRadius(vector<Object3D*>& objects, D3DXVECTOR3 position, float radius, ObjectType type)
{
	for(int i = 0; i < mObjectList.size(); i++)
	{
		if(mObjectList[i]->getType() != type)
			continue;

		D3DXVECTOR3 diff = mObjectList[i]->getPosition() - position;
		float dist = sqrt(diff.x*diff.x + diff.z*diff.z);

		if(dist <= radius)
			objects.push_back(mObjectList[i]);
	}
}

void World::drawToMinimap(RenderTarget* renderTarget)
{
	// Save the current render target to restore it later.
	LPDIRECT3DSURFACE9 backBuffer;
	gd3dDevice->GetRenderTarget(0, &backBuffer);
	gd3dDevice->SetRenderTarget(0, renderTarget->getSurface());

	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xffffff00, 1.0f, 0xffff));

	// Set the cameras position to bird view.
	D3DXVECTOR3 position = gCamera->getPosition();
	float yaw = gCamera->getYaw();
	float pitch = gCamera->getPitch();

	gCamera->setPosition(position + D3DXVECTOR3(0, 5000, 0));
	gCamera->setPitch(-D3DX_PI/2);
	gCamera->rotate();
	gCamera->updateView();

	// Draw the terrain.
	mTerrain->draw();

	// Draw all the enemies with a red box and the player with a green.
	for(int i = 0; i < mObjectList.size(); i++) {
		if(!mObjectList[i]->getAlive() || mObjectList[i]->getType() == PARTICLE_SYSTEM || mObjectList[i]->getType() == SPAWNER)
			continue;

		// Draw minimap icon.
		gGraphics->drawBoundingBox(mObjectList[i]->getMinimapTexture(), mObjectList[i]->getPosition() + D3DXVECTOR3(0, 1500, 0), Dimensions(200, 1, 200), WHITE, 1.0f);
	}

	// Restore the camera.
	gCamera->setPosition(position);
	gCamera->setYaw(yaw);
	gCamera->setPitch(pitch);
	gCamera->rotate();

	// Change back to the backbuffer.
	gd3dDevice->SetRenderTarget(0, backBuffer);
	ReleaseCOM(backBuffer);
}

void World::reset()
{
	// Loop through all objects and find out which one to delete.
	// [NOTE] Skip the player (i = 1).
	int i = 1;
	std::vector<Object3D*>::iterator iter =  mObjectList.begin();
	iter++;
	while(iter != mObjectList.end() && i < mObjectList.size())
	{
		if(mObjectList[i]->getType() != SPAWNER) {
			delete mObjectList[i];		
			mObjectList[i] = NULL;
			iter = mObjectList.erase(iter);	
		}
		else
			iter++;
	}
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
			bool intersect = mObjectList[i]->rayIntersectMesh(gCamera->getPosition(), gCamera->getDirection(), faceIndex, distance);
			if(intersect && distance < closestDist) {
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
	static int id = 0;
	object->setWorld(this);
	object->setId(id);
	object->init();
	mObjectList.push_back(object);
	id++;
}
	
void World::removeObject(Object3D* object)
{
	// Loop through all objects and find out which one to delete.
	int i = 0;
	std::vector<Object3D*>::iterator itr =  mObjectList.begin();
	while(itr != mObjectList.end() && i < mObjectList.size())
	{
		if(mObjectList[i]->getId() == object->getId())
		{
			delete mObjectList[i];		
			mObjectList[i] = NULL;
			itr = mObjectList.erase(itr);	
			break;
		}
		else	{
			itr++;
			i++;
		}
	}
}

void World::addLight(Light* light)
{
	// Add light to back of list.
	mLightList.push_back(light);
}

vector<Light*>* World::getLights()
{
	return &mLightList;
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