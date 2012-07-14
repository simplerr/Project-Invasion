#include "tinyxml\tinystr.h";
#include "tinyxml\tinyxml.h"
#include "SkillHandler.h"

SkillHandler::SkillHandler()
{
	// Load all different skills.
	TiXmlDocument doc("data\\xml\\skills.xml");
	doc.LoadFile();

	// Get the root element
	TiXmlElement* root = doc.FirstChildElement();

	// Load items from the file
	for(TiXmlElement* item = root->FirstChildElement("Skill"); item != NULL; item = item->NextSiblingElement("Skill"))
	{
		SkillData data;
		data.name = item->Attribute("name") == NULL ? "#NOVALUE" : item->Attribute("name");
		data.icon = item->Attribute("icon") == NULL ? "#NOVALUE" : item->Attribute("icon");
		data.range = item->Attribute("range") == NULL ? 0 : atof(item->Attribute("range"));
		data.damage = item->Attribute("damage") == NULL ? 0 : atof(item->Attribute("damage"));
		data.cooldown = item->Attribute("cooldown") == NULL ? 0 : atof(item->Attribute("cooldown"));
		data.energy = item->Attribute("energy") == NULL ? 0 : atof(item->Attribute("energy"));
		
		mDataMap[item->Attribute("name")] = data;
	}
}
	
SkillHandler::~SkillHandler()
{

}

SkillData SkillHandler::getData(string name)
{
	return mDataMap[name];
}