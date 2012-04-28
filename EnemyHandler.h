#pragma once
#include <map>
#include <string>
using namespace std;

struct EnemyData
{
	string name;
	string filename;
	float damage;
	float health;
	float attackRange;
	float attackRate;
	float visionRange;
	float speed;
};

class EnemyHandler
{
public:
	EnemyHandler();
	~EnemyHandler();

	EnemyData getData(string name);
private:
	map<string, EnemyData> mDataMap;
};