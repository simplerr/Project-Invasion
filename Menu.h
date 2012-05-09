#pragma once

#include <vector>
#include <string>
#include "d3dUtil.h"
#include "Input.h"
#include "Rect.h"

#include <boost\function.hpp>
#include <boost\bind.hpp>

using namespace std;

class Vector;

enum ButtonState {STANDARD, PRESSED, SELECTED};
enum NavigationType {MOUSE, ARROWKEYS};
enum MenuType {HOR, VER};

class MenuItem
{
public:
	MenuItem(string name, string standardTextureSource, string onSelectTextureSource = "#none");
	virtual ~MenuItem();

	virtual void draw();
	void setId(int id);

	IDirect3DTexture9* standardTexture;
	IDirect3DTexture9* onSelectTexture;
	ButtonState state;
	string itemName;
	Rect rect;
	bool pressable;
	int id;
};

class Menu
{
public:
	Menu(string name, NavigationType navigation, MenuType type = HOR, int breakCount = 4, bool useFonts = false, int itemAmount = 2, int spacing = 30);
	~Menu();

	void draw(void);
	void update(Vector mousePos);

	void addMenuItem(MenuItem* newItem);
	void buildMenu(int itemWidth = 100, int itemHeight = 50);

	void setSize(int x, int y, int width, int height);
	void setPressable(string name, bool b);

	boost::function<bool(string name)> callback;

	template <class T>
	void connect(bool(T::*_callback)(string name), T* _object)	{
		callback = boost::bind(_callback, _object, _1);
	}

private:
	vector<MenuItem*> mItemList;
	RECT mBackgroundRect;
	NavigationType mNavigation;
	HWND mhMainWnd;
	MenuType mType;
	string mName;
	int mSpacing;
	int mBkgdX, mBkgdY;
	int mNumbersOfItems;
	int mIdCounter;
	int mBreakCount;
	bool mUseFonts;
};