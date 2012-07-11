#pragma once
#include "d3dUtil.h"
#include "Vector.h"

class Graphics;
class Terrain;
struct AABB;

enum CameraType
{
	FPS,
	NOCLIP,
	THIRD_PERSON
};

class Camera
{
public:
	Camera();
	~Camera();

	void update(float dt);
	void move();
	void rotate();
	void rotate(float pitch, float yaw);
	void drawDebug();
	void updateView();

	void setPosition(D3DXVECTOR3 position);
	void setTarget(D3DXVECTOR3 target);
	void setSensitivity(float sensitivity);
	void setSpeed(float speed);
	void setType(CameraType type);
	void setHeightOffset(float heightOffset);
	void setDirection(D3DXVECTOR3 direction);
	void setYaw(float yaw);
	void setPitch(float pitch);

	float getYaw();
	float getPitch();

	// Box coordinates should be relative to world space.
	bool isVisible(const AABB& box)const;

	void buildWorldFrustumPlanes();

	D3DXMATRIX getViewMatrix();
	D3DXMATRIX getProjectionMatrix();
	D3DXVECTOR3 getPosition();
	D3DXVECTOR3 getTarget();
	D3DXVECTOR3 getDirection();
	D3DXVECTOR3 getRight();
private:
	CameraType  mType;
	D3DXMATRIX	mView;
	D3DXMATRIX	mProj;
	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mTarget;
	D3DXVECTOR3 mUp;
	D3DXVECTOR3 mRight;

	float		mVelocity;
	float		mSensitivity;
	float		mHeightOffset;
	float height;
	float mYaw, mPitch;

	// Frustum Planes
	D3DXPLANE mFrustumPlanes[6]; // [0] = near
	                             // [1] = far
	                             // [2] = left
	                             // [3] = right
	                             // [4] = top
	                             // [5] = bottom
};

extern Camera* gCamera;