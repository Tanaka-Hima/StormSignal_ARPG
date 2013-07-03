#include "Physics2DImage.h"

#ifndef _CHARACTER
#define _CHARACTER

class HitBox;

const int Skill_None_None = 0;
const int Skill_Sword_Front = 1;

class Character : public Physics2DImage
{
private:
	
public:
	int MaxHP;
	int HP;
	int State;
	int StateTime;
	int Time;
	static vector<Character*> CharacterList;
	static vector<HitBox> HitBoxList;
	static vector<vector<int>> AnimeGraphs;
	static vector<vector<int>> LoadAnimeGraphs();

	void InitChara(b2World *World,void* UserData,float Density,float Friction,int MaxHP);
	virtual bool UseSkill(int Number);
	virtual void Step(void);
};

#endif