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


const int Equipment_None_None = 0;
const int Equipment_Sword_Normal = 1;
const int Equipment_Sword_Flame = 2;

const string EquipmentValueNames[] = {"Equipment_None_None",
							"Equipment_Sword_Normal",
							"Equipment_Sword_Flame"};

const string EquipmentNames[] = {"無し",
							"通常剣",
							"炎剣"};

const string EquipmentInfo[] = {"装備を使用しません。\n装備種類 : 無し",
							"通常剣を用いて攻撃を行います。\n装備種類 : 剣",
							"炎剣を用いて攻撃を行います。\n装備種類 : 剣"};

vector<int> GetSkillList(string EquipmentName);//装備に対応したスキルを取得
string GetEquipmentNameforSkill(int Skill);//スキル名から装備種類を取り出す
string GetEquipmentNameforEquipment(int Equipment);//装備名から装備種類を取り出す

class Character : public Physics2DImage
{
private:
	
public:
	int MaxHP;//最大HP
	int HP;//HP
	int State;//現在の状態
	int StateTime;//現在の状態が続く時間
	int Time;//時間
	static vector<Character*> CharacterList;//このプログラム上で生成されたすべてのCharacterクラス
	static vector<HitBox> HitBoxList;//Characterによって生成されたすべてのヒットボックス
	static vector<vector<int>> AnimeGraphs;//アニメーション用の画像
	static vector<vector<int>> EquipmentGraphs;//装備画像

	void InitChara(b2World *World,void* UserData,float Density,float Friction,int MaxHP);//Characterクラスの初期化
	virtual void Initialize(b2World *World,void* UserData,float Density,float Friction,int MaxHP) = 0;//初期化
	virtual bool UseSkill(int SkillNumber,int EquipmentNumber);//スキルの使用
	virtual void Step();//毎フレームの処理
	virtual void Ctrl() = 0;//操作
};

#endif