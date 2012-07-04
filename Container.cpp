#include <algorithm>
#include "Input.h"
#include "Graphics.h"
#include "Container.h"
#include "Skill.h"

bool pointInsideRect(Vector point, Rect rect)
{
	if(point.x > rect.left && point.x < rect.right && point.y > rect.top && point.y < rect.bottom)
		return true;
	else
		return false;
}

Container::Container(int x, int y , int width, int height)
{
	// Create the item handler, NOTE: Could be static since it's suppose to never be more than one!
	mMovingItem = NULL;
	mIdCounter = 0;
	mMovedFrom = 0;
	mPosition = Vector(x, y);
	mWidth	= width;
	mHeight = height;

	// Load textures
	mSkillSlot = gGraphics->loadTexture("data\\skill_slot.png");

	// Not visible to start with
	hide();
}

Container::~Container()
{
	for(int i = 0; i < mSlotList.size(); i++) 
	{
		if(mSlotList[i].taken)
			delete mSlotList[i].item;
	}
}

void Container::update(float dt)
{
	for(int i = 0; i < mSlotList.size(); i++) {
		if(mSlotList[i].taken)
			mSlotList[i].item->update(dt);
	}

	// Visible?
	if(mVisible)
	{
		// Move if left clicked
		if(gInput->keyPressed(VK_LBUTTON))
		{
			// Find out if a item was pressed
			for(int i = 0; i < mSlotList.size(); i++) 
			{
				// Pressed, not moving an item, item in the slot
				if(pointInsideRect(gInput->mousePosition(), mSlotList[i].rect) && mMovingItem == NULL && mSlotList[i].taken)
				{
					mSlotList[i].taken = false;
					mMovingItem = mSlotList[i].item;
					selectedMovingItem(mMovingItem);
					mMovedFrom = i;
					break;
				}
			}
		}

		// Left mouse button released
		if(gInput->keyReleased(VK_LBUTTON) && mMovingItem != NULL)
		{
			bool any = false;
			for(int i = 0; i < mSlotList.size(); i++) 
			{
				// Inside any slot?
				if(pointInsideRect(gInput->mousePosition(), mSlotList[i].rect))
				{
					swapItems(&mSlotList[mMovedFrom], &mSlotList[i]);
					releasedMovingItem(mMovingItem);
					mMovingItem = NULL;
					any = true;
				}
			}

			// Not inside a slot, move back to original slot
			if(!any) {
				itemOutsideSlot(mMovingItem);
			}
		}
	}
}

void Container::itemOutsideSlot(SlotItem* item)
{
	mSlotList[mMovedFrom].taken = true;
	mMovingItem = NULL;
}

void Container::draw()
{
	char c[9] = {'1','2','3','4','5','6','7'};
	// Visible?
	if(mVisible)
	{
		// Background
		//gGraphics->drawScreenTexture(mBkgd, mPosition.x, mPosition.y, mWidth, mHeight);
		Vector v = gInput->mousePosition();
		// Loop through all slots
		for(int i = 0; i < mSlotList.size(); i++)
		{
			Rect rect = mSlotList[i].rect;

			IDirect3DTexture9* texture = mSkillSlot;;

			gGraphics->drawScreenTexture(texture, mSlotList[i].rect);

			if(mSlotList[i].taken)
				gGraphics->drawScreenTexture(mSlotList[i].item->getTexture(), rect.left + rect.getWidth()/2, rect.top + rect.getHeight()/2, rect.getWidth()-12, rect.getHeight()-12);

			// Draw the slot item.
			if(mSlotList[i].taken)
				mSlotList[i].item->draw(rect);
		}

		// The moving item
		if(mMovingItem != NULL)
		{
			gGraphics->drawScreenTexture(mMovingItem->getTexture(), gInput->mousePosition().x, gInput->mousePosition().y, 50, 50);
		}
	}
}

void Container::drawSlots()
{
	char c[9] = {'1','2','3','4','5','6','7'};
	// Visible?
	if(mVisible)
	{
		// Background
		//gGraphics->drawScreenTexture(mBkgd, mPosition.x, mPosition.y, mWidth, mHeight);
		// Loop through all slots
		for(int i = 0; i < mSlotList.size(); i++)
		{
			Rect rect = mSlotList[i].rect;

			IDirect3DTexture9* texture = mSkillSlot;

			gGraphics->drawScreenTexture(texture, mSlotList[i].rect);
		}
	}
}

void Container::drawItems()
{
	char c[9] = {'1','2','3','4','5','6','7'};
	// Visible?
	if(mVisible)
	{
		// Background
		Vector v = gInput->mousePosition();
		// Loop through all slots
		for(int i = 0; i < mSlotList.size(); i++)
		{	
			Rect rect = mSlotList[i].rect;
			if(mSlotList[i].taken)
				gGraphics->drawScreenTexture(mSlotList[i].item->getTexture(), rect.left + rect.getWidth()/2, rect.top + rect.getHeight()/2, rect.getWidth()-10, rect.getHeight()-10);
		}

		// The moving item
		if(mMovingItem != NULL)
		{
			gGraphics->drawScreenTexture(mMovingItem->getTexture(), gInput->mousePosition().x, gInput->mousePosition().y, 50, 50);
		}
	}
}

void Container::swapItems(Slot* from, Slot* to)
{
	// Ugly swapping method.. Also checks if one slot is empty or not
	Slot tmp = *from;
	if((*to).taken) {
		itemMoved((*to).item, to->slotId, from->slotId);

		(*from).item = (*to).item;
		(*from).taken = true;

		itemMoved((tmp).item, tmp.slotId, to->slotId);
		(*to).item = tmp.item;
		(*to).taken = true;
	}
	else {
		(*from).taken = false;

		itemMoved((*from).item, from->slotId, to->slotId);

		(*to).item =(*from).item;
		(*to).taken = true;
	}
}

void Container::setTexture(string texture)
{
	//mBkgd = gGraphics->loadTexture(texture);
}

void Container::show()
{
	mVisible = true;
}

void Container::hide()
{
	mVisible = false;
}

void Container::addSlot(int x, int y, int size, SlotId id)
{
	mSlotList.push_back(Slot(x, y, size, id));
}

void Container::setSlotSize(int size)
{
	mSlotSize = size;
}

Vector Container::getPosition()
{
	return mPosition;
}

SlotItem* Container::getMovingItem()
{
	return mMovingItem;
}

int Container::getWidth()
{
	return mWidth;
}

int Container::getHeight()
{
	return mHeight;
}

bool Container::getVisible()
{
	return mVisible;
}