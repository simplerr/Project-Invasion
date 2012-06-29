#pragma once
#include <map>
#include <string>
using namespace std;

// Skill data.
struct SkillData 
{
	string name;
	string icon;
	float range;
	float damage;
	float cooldown;
	float energy;
};

class SkillHandler
{
public:
	SkillHandler();
	~SkillHandler();

	SkillData getData(string name);
private:
	map<string, SkillData> mDataMap;
};

extern SkillHandler* gSkillHandler;