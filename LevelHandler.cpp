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

		// Load the spawn position.
		D3DXVECTOR3 spawnPosition;
		TiXmlElement* spawnPosElement = levelElement->FirstChild("SpawnPosition")->ToElement();
		spawnPosition.x = atof(spawnPosElement->Attribute("x"));
		spawnPosition.y = atof(spawnPosElement->Attribute("y"));
		spawnPosition.z = atof(spawnPosElement->Attribute("z"));

		// Load all the waves.
		vector<Wave*> waveList;
		TiXmlElement* waves = levelElement->FirstChildElement("Waves");
		for(TiXmlElement* waveElement = waves->FirstChildElement("Wave"); waveElement != NULL; waveElement = waveElement->NextSiblingElement("Wave"))
		{
			Wave* wave = NULL;

			// Read the wave attributes.
			int enemies = atoi(waveElement->FirstChild("Enemies")->ToElement()->GetText());
			int initialEnemies = atoi(waveElement->FirstChild("InitialEnemies")->ToElement()->GetText());
			float spawnRate = atof(waveElement->FirstChild("SpawnRate")->ToElement()->GetText());
			float speedAdjust = atof(waveElement->FirstChild("SpeedAdjust")->ToElement()->GetText());
			float damageAdjust = atof(waveElement->FirstChild("DamageAdjust")->ToElement()->GetText());
			float healthAdjust = atof(waveElement->FirstChild("HealthAdjust")->ToElement()->GetText());

			// Load all the spawns.
			vector<Spawner*> spawnList;
			TiXmlElement* spawns = waveElement->FirstChildElement("Spawns");
			for(TiXmlElement* spawnElement = spawns->FirstChildElement("Spawn"); spawnElement != NULL; spawnElement = spawnElement->NextSiblingElement("Spawn"))
			{
				D3DXVECTOR3 position;

				position.x = atof(spawnElement->Attribute("x"));
				position.y = atof(spawnElement->Attribute("y"));
				position.z = atof(spawnElement->Attribute("z"));

				spawnList.push_back(new Spawner(position));
			}

			// Create the wave and add it to the list.
			wave = new Wave(mWorld, mPlayer);
			wave->setData(spawnList, enemies, initialEnemies, spawnRate, speedAdjust, damageAdjust, healthAdjust);
			waveList.push_back(wave);
		}

		// Set the levels wave list.
		level = new Level(name, description, spawnPosition);
		level->setWaves(waveList);
		mLevelList.push_back(level);
	}
}

vector<Wave*> LevelHandler::loadWaves(TiXmlElement* level)
{
	vector<Wave*> waveList;

	return waveList;
}

Level* LevelHandler::getLevel(int num)
{
	return mLevelList[num];
}

void LevelHandler::setWorld(World* world)
{
	mWorld = world;
}
	
void LevelHandler::setPlayer(Player* player)
{
	mPlayer = player;
}