#include "SlotItem.h"
#include "Graphics.h"

SlotItem::SlotItem()
{

}

SlotItem::~SlotItem()
{

}

void SlotItem::loadTexture(string source)
{
	mTexture = gGraphics->loadTexture(source);
}

void SlotItem::update(float dt)
{

}

void SlotItem::setId(int id)
{
	mId = id;
}

void SlotItem::setSlotId(SlotId id)
{
	mSlotId = id;
}

int SlotItem::getId()
{
	return mId;
}

SlotId SlotItem::getSlotId()
{
	return mSlotId;
}

IDirect3DTexture9* SlotItem::getTexture()
{
	return mTexture;
}