#include "Camera.h"
#include "Input.h"
#include "Graphics.h"
#include "Terrain.h"
#include "Mesh.h"

float angle;
float Y;

D3DXVECTOR3 dirXZ;

Camera::Camera()
{
	// Set the sensitivity and speed
	setSensitivity(2.0f);
	setSpeed(0.50f);
	setType(FPS);
	setHeightOffset(4.0f);

	// Default position and target
	mPosition   = D3DXVECTOR3(100, 200, 100);
	mUp			= D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// Weird up vector
	mTarget		= D3DXVECTOR3(0, 0, 0);
	mYaw = 0.0f;
	mPitch = 0.0f;

	// Build the projection matrix
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI * 0.25f, 1200/800, 1.0f, 5000.0f);
}
	
Camera::~Camera()
{

}

void Camera::update(Terrain* terrain, float dt)
{
	// Rotate and move
	//move();
	rotate();

	// Walk on the ground if FPS camera.
	//if(mType == FPS)
	//{
	//	BOOL hit = false;
	//	DWORD faceIndex, hitCount;
	//	float u, v, dist;
	//	LPD3DXBUFFER allHits;
	//	ID3DXMesh* mesh = terrain->getMesh()->getMesh();

	//	D3DXIntersect(mesh, &gCamera->getPosition(), &D3DXVECTOR3(0, -1, 0), &hit, &faceIndex, &u, &v, &dist, &allHits, &hitCount);
	//	if(hit) 
	//	{
	//		height = dist;
	//		D3DXVECTOR3 pos = getPosition();
	//		pos.y = pos.y - dist + mHeightOffset;
	//		setPosition(pos);
	//	}

	//	/*D3DXVECTOR3 direction = mTarget - mPosition;
	//	mPosition.y = terrain->getHeight(getPosition().x, getPosition().z) + mHeightOffset;
	//	mTarget = mPosition + direction;*/
	//}
	
	// Update the right vector.
	D3DXVec3Cross(&mRight, &mUp, &getDirection());
	D3DXVec3Normalize(&mRight, &mRight);

	// Update the view matrix
	D3DXMatrixLookAtLH(&mView, &mPosition, &mTarget, &mUp);
}

void Camera::move()
{
	// Get the look direction
	D3DXVECTOR3 direction;
	D3DXVec3Normalize(&direction, &(mTarget - mPosition));

	// Fast scroll zooming
	mPosition += direction * gInput->mouseDz() / 8;
	mTarget += direction * gInput->mouseDz() / 8;

	// Check for keypresses
	if(gInput->keyDown('W')) {
		mPosition += direction*mVelocity;
		mTarget += direction*mVelocity;
	}
	if(gInput->keyDown('S')) {
		mPosition -= direction*mVelocity;
		mTarget -= direction*mVelocity;
	}
	if(gInput->keyDown('A')) {
		D3DXVec3Cross(&direction, &direction, &mUp);
		D3DXVec3Normalize(&direction, &direction);
		mPosition += direction*mVelocity;
		mTarget += direction*mVelocity;
	}
	if(gInput->keyDown('D')) {
		D3DXVec3Cross(&direction, &direction, &mUp);
		D3DXVec3Normalize(&direction, &direction);
		mPosition -= direction*mVelocity;
		mTarget -= direction*mVelocity;
	}
}
	
void Camera::rotate()
{
	// Increase the pitch and yaw angles.
	mPitch += gInput->mouseDy() / (-13.0f / mSensitivity);
	mYaw += gInput->mouseDx() / (13.0f / mSensitivity);

	// Limit to PI/2 radians.
	if(mPitch > 0)
		mPitch = min(mPitch, 1.54f);
	else
		mPitch = max(mPitch, -1.54f);
	
	// Calculate the new direction.
	D3DXVECTOR3 direction;
	float r = cosf(mPitch);
	direction.y = sinf(mPitch);
	direction.z = r * cosf(mYaw);
	direction.x = r * sinf(mYaw);

	// Set the new target.
	mTarget = mPosition + direction;
}

void Camera::drawDebug()
{
	char buffer[256];
	sprintf(buffer, "Up: (%f, %f, %f)\nTarget: (%f, %f, %f)Height: %f", mUp.x, mUp.y, mUp.z, mTarget.x, mTarget.y, mTarget.z, height);
	gGraphics->drawText(buffer, 20, 200, BLUE);
	sprintf(buffer, "[Direction] x: %f, y: %f, z: %f", getDirection().x, getDirection().y, getDirection().z);
	gGraphics->drawText(buffer, 20, 240, BLUE);
	sprintf(buffer, "[dirXZ] x: %f, y: %f, z: %f", dirXZ.x, dirXZ.y, dirXZ.z);
	gGraphics->drawText(buffer, 20, 280, BLUE);
	sprintf(buffer, "Angle: %f", angle);
	gGraphics->drawText(buffer, 20, 320, WHITE);
	sprintf(buffer, "Yaw: %f", Y);
	gGraphics->drawText(buffer, 20, 360, WHITE);
}

void Camera::setHeightOffset(float heightOffset)
{
	mHeightOffset = heightOffset;
}

void Camera::setPosition(D3DXVECTOR3 position)
{
	D3DXVECTOR3 direction = mTarget - mPosition;
	mPosition = position;
	mTarget = mPosition + direction;
}
	
void Camera::setTarget(D3DXVECTOR3 target)
{
	mTarget = target;
}

void Camera::setSensitivity(float sensitivity)
{
	mSensitivity = sensitivity/100.0f;
}

void Camera::setSpeed(float speed)
{
	mVelocity = speed;
}

void Camera::setType(CameraType type)
{
	mType = type;
}

D3DXMATRIX Camera::getViewMatrix()
{
	return mView;
}
	
D3DXMATRIX Camera::getProjectionMatrix()
{
	return mProj;
}

D3DXVECTOR3 Camera::getPosition()
{
	return mPosition;
}

D3DXVECTOR3 Camera::getTarget()
{
	return mTarget;
}

D3DXVECTOR3 Camera::getRight()
{
	return mRight;
}

D3DXVECTOR3 Camera::getDirection()
{
	D3DXVECTOR3 direction;
	D3DXVec3Normalize(&direction, &(mTarget - mPosition));
	return direction;
}