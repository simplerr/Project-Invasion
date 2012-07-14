#include "tinyxml\tinystr.h";
#include "tinyxml\tinyxml.h"
#include "WeaponHandler.h"

WeaponHandler::WeaponHandler()
{
	// Load all different items
	TiXmlDocument doc("data\\xml\\weapons.xml");
	doc.LoadFile();

	// Get the root element
	TiXmlElement* root = doc.FirstChildElement();

	// Load items from the file
	for(TiXmlElement* item = root->FirstChildElement("Weapon"); item != NULL; item = item->NextSiblingElement("Weapon"))
	{
		WeaponData data;
		data.name = item->Attribute("name") == NULL ? "#NOVALUE" : item->Attribute("name");
		data.filename = item->Attribute("filename") == NULL ? "#NOVALUE" : item->Attribute("filename");
		data.clipSize = item->Attribute("clipSize") == NULL ? 0 : atoi(item->Attribute("clipSize"));
		data.range = item->Attribute("range") == NULL ? 0 : atof(item->Attribute("range"));
		data.damage = item->Attribute("damage") == NULL ? 0 : atof(item->Attribute("damage"));
		data.rateOfFire = item->Attribute("rateOfFire") == NULL ? 0 : atof(item->Attribute("rateOfFire"));
		
		mDataMap[item->Attribute("name")] = data;
	}
}
	
WeaponHandler::~WeaponHandler()
{

}

WeaponData WeaponHandler::getData(string name)
{
	return mDataMap[name];
}