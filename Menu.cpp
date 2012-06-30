#include "Menu.h"
#include "Graphics.h"
#include "Vector.h"

MenuItem::MenuItem(string name, string standardTextureSource, string onSelectTextureSource, string inactiveTextureSource)
{
	this->itemName = name;
	this->state = STANDARD;
	this->standardTexture = gGraphics->loadTexture(standardTextureSource);

	if(onSelectTextureSource == "#none")
		this->onSelectTexture = gGraphics->loadTexture(standardTextureSource);
	else
		this->onSelectTexture = gGraphics->loadTexture(onSelectTextureSource);

	if(inactiveTextureSource == "#none")
		this->inactiveTexture = gGraphics->loadTexture(standardTextureSource);
	else
		this->inactiveTexture = gGraphics->loadTexture(inactiveTextureSource);

	this->pressable = true;
}
	
MenuItem::~MenuItem()
{

}

void MenuItem::draw()
{
	if(state == STANDARD)
		gGraphics->drawScreenTexture(standardTexture, rect);
	else if(state == SELECTED)
		gGraphics->drawScreenTexture(onSelectTexture, rect);
	else if(state == INACTIVE)
		gGraphics->drawScreenTexture(inactiveTexture, rect);
}

void MenuItem::setId(int id)
{
	this->id = id;
}

Menu::Menu(std::string menuName, NavigationType navigation, MenuType type, int breakCount, bool useFonts, int itemAmount, int spacing)
{
	mName = menuName;
	mSpacing = spacing;
	mNavigation = navigation;
	mNumbersOfItems = itemAmount;
	mUseFonts = useFonts;
	mType = type;
	mBreakCount = breakCount;
	setVisible(true);
	mBkgdTexture = NULL;
}

Menu::~Menu()
{
	for(int i = 0; i < mItemList.size(); i++) 
	{
		delete mItemList[i];
	}
}

void Menu::setSize(int x, int y, int width, int height)
{
	mBackgroundRect.left = x-(width/2);
	mBackgroundRect.right = x+(width/2);
	mBackgroundRect.top = y-(height/2);
	mBackgroundRect.bottom = y+(height/2);

	mBkgdX = x;
	mBkgdY = y;

	mIdCounter = 0;
}

void Menu::addMenuItem(MenuItem* newItem)
{
	static int idCounter = 0;
	newItem->setId(idCounter);
	mItemList.push_back(newItem);
	idCounter++;
}

void Menu::draw(void)
{	
	if(!getVisible())
		return;

	if(mBkgdTexture != NULL) {
		Rect rect;
		rect.right = mBackgroundRect.right;
		rect.left = mBackgroundRect.left;
		rect.top = mBackgroundRect.top;
		rect.bottom = mBackgroundRect.bottom;
		gGraphics->drawScreenTexture(mBkgdTexture, rect);
	}

	for(int i = 0; i < mItemList.size(); i++)
	{	
		mItemList[i]->draw();
	}	
}

void Menu::update(Vector mousePos)
{
	if(!getVisible())
		return;

	if(mNavigation == MOUSE)
	{
		for(int i = 0; i < mItemList.size(); i++)
		{	
			// Skip inactive elements.
			if(mItemList[i]->state == INACTIVE)
				continue;

			MenuItem* item = mItemList[i];
			// mouse is inside
			if(mousePos.x < item->rect.right && mousePos.x > item->rect.left && mousePos.y < item->rect.bottom && mousePos.y > item->rect.top)
			{
				item->state = SELECTED;
				if(gInput->keyPressed(VK_LBUTTON) && item->pressable)	{
					if(!callback(item->itemName))
						break;
				}		
			}
			else
				item->state = STANDARD;
		}
	}
	else if(mNavigation == ARROWKEYS)
	{		
		// updatera idCounter				
		if(gInput->keyPressed('W')){
			mIdCounter--;
		}
		else if(gInput->keyPressed('S'))	{
			mIdCounter++;
		}

		// kolla om man är utanför menyn
		if(mIdCounter < 0)
			mIdCounter = mNumbersOfItems-1;	// tänk 0-X
		else if(mIdCounter > mNumbersOfItems-1)
			mIdCounter = 0;			

		for(int i = 0; i < mItemList.size(); i++)
		{	
			// Skip inactive elements.
			if(mItemList[i]->state == INACTIVE)
				continue;

			MenuItem* item = mItemList[i];
			if(item->id == mIdCounter){				
				item->state = SELECTED;
			}
			else
				item->state = STANDARD;

			// a item was pressed
			if(gInput->keyPressed(VK_RETURN))
			{
				if(item->id == mIdCounter && item->pressable)	
				{
					if(!callback(item->itemName))
						break;
				}					
			}
		}
	}

}

void Menu::buildMenu(int itemWidth, int itemHeight)
{
	/* stuff neeeded */
	int		menuSize;
	int		widthInItems;
	int		heightInItems;
	int		loopX;
	int		loopY;
	int		dX;
	int		dY;
	int		curX;
	int		curY;
	int		resX, resY;
	int		itemNumber;

	/* number of items */
	menuSize = mItemList.size();

	/* find out width and height counted in items */
	widthInItems = menuSize % mBreakCount;

	/*
	*  this is neccessary to get a correct height
	*  has to do with integers rounding down
	*  will only occur when there's an uneven amount of menu items
	*  adds mBreakCount since adding 1 isn't suffice with large menus
	*/
	if(menuSize % mBreakCount != 0)	{
		heightInItems = (menuSize + mBreakCount) / mBreakCount;
		if(heightInItems != 1)
			widthInItems = mBreakCount;
	}
	else	{
		heightInItems = (menuSize) / mBreakCount;
		if(heightInItems != 0)
			widthInItems = mBreakCount;
	}

	if(menuSize == 1)	{
		widthInItems = 0;
		heightInItems = 0;
	}

	itemNumber = 0;

	/* loopa through all items */
	for(int i = 0; i < mItemList.size(); i++)
	{	
		MenuItem* item = mItemList[i];
		loopX = itemNumber %  mBreakCount;
		loopY = itemNumber /  mBreakCount;

		/* even amount of items in the widest x row */
		if(widthInItems % 2 == 0 && widthInItems != 0)	{
			dX = (loopX * (mSpacing + itemWidth));
			dX -= 0.5*(mSpacing + itemWidth);

			if(menuSize == 1)
				dX = 0;

			resX = mBkgdX - dX - widthInItems*(mSpacing + itemWidth)/2 ;//+ (spacing + itemWidth)/2;
		}
		/* uneven */
		else	{
			dX = loopX*(mSpacing +itemWidth);

			if(menuSize == 1)
				resX = mBkgdX;
			else
				resX = mBkgdX - dX - widthInItems*(mSpacing + itemWidth)/2 + (mSpacing + itemWidth)/2;;
		}

		/* even amount of items in the widest y row */
		if(heightInItems % 2 == 0 && heightInItems != 0)	{
			dY = (loopY * (mSpacing + itemHeight));
			dY -= 0.5*(mSpacing + itemHeight);	

			if(menuSize == 1)
				dY = 0;

			resY = mBkgdY - dY - heightInItems*(mSpacing + itemHeight)/2;
		}
		/* uneven */
		else	{		
				dY = loopY*(mSpacing +itemHeight);

				if(menuSize == 1)
					resY = mBkgdY;
				else
					resY = mBkgdY - dY - heightInItems*(mSpacing + itemHeight)/2 + (mSpacing + itemHeight)/2;
		}

		/* center the menu items */
		item->rect.left = resX-(itemWidth/2);
		item->rect.right = resX+(itemWidth/2);
		item->rect.top = resY-(itemHeight/2);
		item->rect.bottom = resY+(itemHeight/2); 

		itemNumber--;
	}
}

void Menu::setPressable(std::string name, bool b)
{
	for(int i = 0; i < mItemList.size(); i++)
	{	
		MenuItem* item = mItemList[i];
		if(item->itemName == name)
			item->pressable = b;
	}
}

void Menu::setVisible(bool visible)
{
	mVisible = visible;
}

void Menu::setBkgdTexture(string source)
{
	mBkgdTexture = gGraphics->loadTexture(source);
}

bool Menu::getVisible()
{
	return mVisible;
}