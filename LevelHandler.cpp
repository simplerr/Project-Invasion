#include "LevelHandler.h"
#include "Spawner.h"

LevelHandler::LevelHandler() : mDocument("data/levels.xml")
{
	
}
	
LevelHandler::~LevelHandler()
{
	saveProgress();

	for(int i = 0; i < mLevelList.size(); i++)
		delete mLevelList[i];
}

void LevelHandler::saveProgress()
{
	// Update the CompletedWaves attribute.
	TiXmlElement* root = mDocument.FirstChildElement();

	// Load all levels from the file
	int i = 0;
	for(TiXmlElement* levelElement = root->FirstChildElement("Level"); levelElement != NULL; levelElement = levelElement->NextSiblingElement("Level"), i++)
	{
		char buffer[256];
		TiXmlElement* waves = levelElement->FirstChildElement("Waves");
		int j = mLevelList[i]->getCompletedWaves();
		waves->SetAttribute("completed", mLevelList[i]->getCompletedWaves());
	}

	mDocument.SaveFile("data\\levels.xml");
}

void LevelHandler::loadLevels()
{
	// Cleanup.
	for(int i = 0; i < mLevelList.size(); i++)
		delete mLevelList[i];

	mLevelList.clear();

	// Load all different items
	mDocument.LoadFile();

	// Get the root element
	TiXmlElement* root = mDocument.FirstChildElement();

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

		// Completed waves.
		int completedWaves = atoi(waves->Attribute("completed"));

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
		level->setCompletedWaves(completedWaves);
		level->setWaves(waveList);
		mLevelList.push_back(level);
	}
}

Level* LevelHandler::getLevel(int num)
{
	return mLevelList[num];
}

Level* LevelHandler::getLevel(string name)
{
	for(int i = 0; i < mLevelList.size(); i++) {
		if(mLevelList[i]->getName() == name)
			return mLevelList[i];
	}

	return NULL;
}

int LevelHandler::getNumLevels()
{
	return mLevelList.size();
}

void LevelHandler::deleteSpawners()
{
	for(int i = 0; i < mLevelList.size(); i++)
		mLevelList[i]->deleteSpawners();
}