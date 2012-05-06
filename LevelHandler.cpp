#include "tinyxml\tinystr.h";
#include "tinyxml\tinyxml.h"
#include "LevelHandler.h"
#include "Spawner.h"

LevelHandler::LevelHandler()
{

}
	
LevelHandler::~LevelHandler()
{

}

void LevelHandler::loadLevels()
{
	// Load all different items
	TiXmlDocument doc("data\\levels.xml");
	doc.LoadFile();

	// Get the root element
	TiXmlElement* root = doc.FirstChildElement();

	// Load all levels from the file
	for(TiXmlElement* levelElement = root->FirstChildElement("Level"); levelElement != NULL; levelElement = levelElement->NextSiblingElement("Level"))
	{
		Level* level;

		// Load the name and description.
		string name = levelElement->FirstChild("Name")->ToElement()->GetText();
		string description = levelElement->FirstChild("Desc")->ToElement()->GetText();

		// Load the difficulty adjusts.
		TiXmlElement* difficultyElement = levelElement->FirstChildElement("DifficultyAdjusts");
		float speedAdjust = atof(difficultyElement->Attribute("speed"));
		float healthAdjust = atof(difficultyElement->Attribute("health"));
		float damageAdjust = atof(difficultyElement->Attribute("damage"));

		// Load the players spawn position.
		TiXmlElement* spawnPosElement = levelElement->FirstChild("SpawnPosition")->ToElement();
		D3DXVECTOR3 spawnPosition(atof(spawnPosElement->Attribute("x")), atof(spawnPosElement->Attribute("y")), atof(spawnPosElement->Attribute("z")));

		// Load all the waves.
		vector<Wave*> waveList;
		TiXmlElement* waves = levelElement->FirstChildElement("Waves");
		for(TiXmlElement* waveElement = waves->FirstChildElement("Wave"); waveElement != NULL; waveElement = waveElement->NextSiblingElement("Wave"))
		{
			Wave* wave = NULL;

			// Read the wave attributes.
			int enemies = atoi(waveElement->Attribute("enemies"));
			int initialEnemies = atoi(waveElement->Attribute("initialEnemies"));
			float spawnRate = atof(waveElement->Attribute("spawnRate"));

			// Create the wave and add it to the list.
			wave = new Wave(enemies, initialEnemies, spawnRate);
			waveList.push_back(wave);
		}

		// Load all the spawns.
		vector<Spawner*> spawnList;
		TiXmlElement* spawns = levelElement->FirstChildElement("Spawns");
		for(TiXmlElement* spawnElement = spawns->FirstChildElement("Spawn"); spawnElement != NULL; spawnElement = spawnElement->NextSiblingElement("Spawn"))
		{
			D3DXVECTOR3 position;

			position.x = atof(spawnElement->Attribute("x"));
			position.y = atof(spawnElement->Attribute("y"));
			position.z = atof(spawnElement->Attribute("z"));

			spawnList.push_back(new Spawner(position));
		}

		// Set the levels wave list.
		level = new Level(name, description, spawnPosition, spawnList);
		level->setDifficultyAdjusts(speedAdjust, healthAdjust, damageAdjust);
		level->setWaves(waveList);
		mLevelList.push_back(level);
	}
}


Level* LevelHandler::getLevel(int num)
{
	return mLevelList[num];
}