#include "Bar.h"
#include "Graphics.h"
#include "Rect.h"
#include "Game.h"

Bar::Bar(string barTexture, string iconTexture, float max)
{
	mBarTexture = gGraphics->loadTexture(barTexture);
	mIconTexture = gGraphics->loadTexture(iconTexture);
	mBorderTexture= gGraphics->loadTexture("data\\bar_border.bmp");
	mMax = max;
	mCurrent = max;
	mWidth = 210;
	mHeight = 28;
}
	
Bar::~Bar()
{

}

void Bar::update(float dt)
{

}
	
void Bar::draw()
{
	float filled = (mCurrent / mMax);

	Rect sourceRect;
	sourceRect.left = 0.0f;
	sourceRect.right = filled * 32;	// 32 is image width
	sourceRect.top = 0.0f;
	sourceRect.bottom = 1.0f;

	// Draw the bar textures.
	//gGraphics->drawScreenTexture(mBorderTexture, mPosition.x - 18, mPosition.y, mWidth + 22, mHeight);
	gGraphics->drawScreenTextureAtlas(mBarTexture, mPosition.x - (float)(mWidth-20)/2 + (float)(mWidth-20)*filled/2, mPosition.y, (mWidth-20)*filled, mHeight, &sourceRect);
	gGraphics->drawScreenTexture(mIconTexture, mPosition.x - 115, mPosition.y, 20, 20);

	// Draw text.
	char buffer[256];
	sprintf(buffer, "%.0f/%.0f", mCurrent, mMax);
	gGraphics->drawText(buffer, mPosition.x * gGame->widthRatio() - 40, mPosition.y * gGame->heightRatio() - 10, WHITE, 18);
}
	
void Bar::setPosition(float x, float y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void Bar::setCurrent(float current)
{
	mCurrent = current;
}