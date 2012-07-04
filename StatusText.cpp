#include "Vector.h"
#include "Graphics.h"
#include "StatusText.h"
#include "Game.h."

//! Constructor.
StatusText::StatusText()
{
	mColor = RED;
	setText("nothing", mColor, 0);
	mPosition = D3DXVECTOR3(0, 0, 0);
}

//! Constructor.
/**
@param text The text to draw.
@param time How long to draw the text (seconds).
*/
StatusText::StatusText(string text, float x, float y, float time)
{
	setText(text, RED, time);
	mPosition = D3DXVECTOR3(x, y, 0);
}

//! Destructor.
StatusText::~StatusText()
{

}

//! Draws the status text.
void StatusText::draw()
{
	if(mTime > 0)	{
		gGraphics->drawText(mText, mPosition.x * gGame->widthRatio(), mPosition.y * gGame->heightRatio(), mColor);
	}
}

//! Updates the timer.
void StatusText::update(float dt)
{
	mTime -= dt;
}

//! Set the text and how long to draw it.
void StatusText::setText(string text, D3DXCOLOR color, float time)
{
	mText = text;
	mTime = time;
	mColor = color;
}

//! Resets the timer.
void StatusText::activate(float time)
{
	mTime = time;
}