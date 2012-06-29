#pragma once

#include <map>
#include <string>
using namespace std;

struct WeaponData
{
	string filename;
	string name;
	int clipSize;
	float rateOfFire;
	float range;
	float damage;
};

class WeaponHandler
{
public:
	WeaponHandler();
	~WeaponHandler();

	WeaponData getData(string name);
private:
	map<string, WeaponData> mDataMap;
};

extern WeaponHandler* gWeaponHandler;