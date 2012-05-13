#pragma once

struct Rect
{
	Rect() {};
	Rect(float left, float right, float top, float bottom)	{
		this->left = left;
		this->right = right;
		this->top = top;
		this->bottom = bottom;
	}

	float left;
	float right;
	float top;
	float bottom;
};