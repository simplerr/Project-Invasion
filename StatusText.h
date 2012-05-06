#pragma once

#include <string>
#include "Vector.h"
#include "d3dUtil.h"
#include "Material.h"
using namespace std;

class StatusText
{
public:
	StatusText();
	StatusText(string text, float x, float y, float time = 2.0f);
	~StatusText();

	void draw();
	void update(float dt);
	void setText(string text, D3DXCOLOR color = BLACK, float time = 2.0f);
	void activate(float time);
private:
	D3DXCOLOR 	mColor;
	D3DXVECTOR3	mPosition;
	string	mText;
	float	mTime;
};