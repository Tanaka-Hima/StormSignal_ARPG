#include "Physics2DImage.h"

#ifndef _CHARACTER
#define _CHARACTER

class HitBox;

const int Skill_None_None = 0;
const int Skill_Sword_Front = 1;

const string SkillValueNames[] = {"Skill_None_None",
							"Skill_Sword_Front"};

const string SkillNames[] = {"無し",
							"前方へ剣を振り下ろす(仮)"};

const string SkillInfo[] = {"スキルを設定しません。\n必要装備 : 無し CD : 無し",
							"前方へ剣を振り下ろします。\n必要装備 : 剣 CD : 無し"};


const int Equipment_Sword_Normal = 0;
const int Equipment_Sword_Flame = 1;

const string EquipmentValueNames[] = {"Equipment_Sword_Normal",
							"Equipment_Sword_Flame"};

const string EquipmentNames[] = {"通常剣",
							"炎剣"};

const string EquipmentInfo[] = {"通常剣を用いて攻撃を行います。\n装備種類 : 剣",
							"炎剣を用いて攻撃を行います。\n装備種類 : 剣"};


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