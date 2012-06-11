#pragma once

#include <string>
#include "d3dUtil.h"
#include "Vector.h"

using namespace std;

class Bar
{
public:
	Bar(string barTexture, string iconTexture, float max);
	~Bar();

	void update(float dt);
	void draw();
	void setPosition(float x, float y);
	void setCurrent(float current);
private:
	IDirect3DTexture9* mBarTexture;
	IDirect3DTexture9* mIconTexture;
	IDirect3DTexture9* mBorderTexture;
	Vector mPosition;;
	float mWidth, mHeight;
	float mMax;
	float mCurrent;
};