#include "tinyxml\tinystr.h";
#include "tinyxml\tinyxml.h"
#include "EnemyHandler.h"

EnemyHandler::EnemyHandler()
{
	// Load all different items
	TiXmlDocument doc("data\\enemies.xml");
	doc.LoadFile();

	// Get the root element
	TiXmlElement* root = doc.FirstChildElement();

	// Load items from the file
	for(TiXmlElement* item = root->FirstChildElement("Enemy"); item != NULL; item = item->NextSiblingElement("Enemy"))
	{
		EnemyData data;
		data.name = item->Attribute("name") == NULL ? "#NOVALUE" : item->Attribute("name");
		data.filename = item->Attribute("filename") == NULL ? "#NOVALUE" : item->Attribute("filename");
		data.visionRange= item->Attribute("visionRange") == NULL ? 1000.0f : atof(item->Attribute("visionRange"));
		data.damage = item->Attribute("damage") == NULL ? 10.0f : atof(item->Attribute("damage"));
		data.attackRate = item->Attribute("attackRate") == NULL ? 0.5f : atof(item->Attribute("attackRate"));
		data.attackRange = item->Attribute("attackRange") == NULL ? 100.0f : atof(item->Attribute("attackRange"));
		data.health = item->Attribute("health") == NULL ? 100.0f : atof(item->Attribute("health"));
		data.speed = item->Attribute("speed") == NULL ? 1.0f : atof(item->Attribute("speed"));

		mDataMap[item->Attribute("name")] = data;
	}
}
	
EnemyHandler::~EnemyHandler()
{

}

EnemyData EnemyHandler::getData(string name)
{
	return mDataMap[name];
}