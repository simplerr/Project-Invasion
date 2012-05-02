#pragma once
#include "d3dUtil.h"
#include "Vector.h"

class Graphics;
class Terrain;

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

	void update(Terrain* terrain, float dt);
	void move();
	void rotate();
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
};

extern Camera* gCamera;