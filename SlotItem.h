#pragma once

#include <string>
#include "d3dUtil.h"
#include "Rect.h"
using namespace std;

// Enum for different type of slots.
enum SlotId {SKILL};

//! Abstract base class for all classes that are slot items.
//! Skills, Items inherits from this class.
class SlotItem
{
public:
	SlotItem();
	virtual ~SlotItem();

	void loadTexture(string source);
	virtual void update(float dt);
	virtual void draw(Rect rect){};
	virtual IDirect3DTexture9* getTexture();
	virtual SlotId	getSlotId();

	void	setId(int id);
	void	setSlotId(SlotId id);
	int		getId();
private:
	IDirect3DTexture9*	mTexture;
	int					mId;
	SlotId				mSlotId;
};