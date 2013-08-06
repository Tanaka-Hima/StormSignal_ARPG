#include "Character.h"
#include "HitBox.h"
#include "ConstantValue.h"
#include "Functions.h"
#include <DxLib.h>

vector<Character*> Character::CharacterList;
vector<HitBox> Character::HitBoxList;
vector<Image_2D> Character::AnimeGraphs;
vector<Image_2D> Character::EquipmentGraphs;

vector<int> GetSkillList(string EquipmentName)
{
	vector<int> Skills;
	Skills.push_back(0);
	int Length = GetArrayLength(SkillValueNames);
	for(int i=0;i<Length;i++)
	{
		if(SkillValueNames[i].find(EquipmentName) != string::npos && i != 0)
		{
			Skills.push_back(i);
		}
	}
	return Skills;
}

string GetEquipmentNameforSkill(int Skill)
{
	int Pos1,Pos2;
	Pos1 = SkillValueNames[Skill].find("_") + 1;
	Pos2 = SkillValueNames[Skill].rfind("_") - 1;
	return SkillValueNames[Skill].substr(Pos1,Pos2);
}

string GetEquipmentNameforEquipment(int Equipment)
{
	int Pos1,Pos2;
	Pos1 = EquipmentValueNames[Equipment].find("_") + 1;
	Pos2 = EquipmentValueNames[Equipment].rfind("_") - 1;
	return EquipmentValueNames[Equipment].substr(Pos1,Pos2-Pos1);
}

void Character::InitChara(b2World *World,void* UserData,float Density,float Friction,int MaxHP)
{
	Init(World,UserData,Density,Friction);

	HP = MaxHP;
	GetBody()->SetSleepingAllowed(false);

	CharacterList.push_back(this);

	State = Skill_None_None;
	StateTime = 0;
	Time = GetNowCount();

	if(AnimeGraphs.size() != 0)return;

	Image_2D TempGraphs;

	#pragma region 装備画像読み込み

	//Equipment_None_None 0
	EquipmentGraphs.push_back(TempGraphs);
	EquipmentGraphs[EquipmentGraphs.size()-1].Load("Image/Equipment/None.png");

	//Equipment_Sword_Normal 1
	EquipmentGraphs.push_back(TempGraphs);
	EquipmentGraphs[EquipmentGraphs.size()-1].Load("Image/Equipment/Sword/Normal_0.png");

	//Equipment_Sword_Flame 2
	EquipmentGraphs.push_back(TempGraphs);
	EquipmentGraphs[EquipmentGraphs.size()-1].Load("Image/Equipment/Sword/Flame_0.png");

	#pragma endregion

	#pragma region スキル画像読み込み

	//Skill_None_None 0
	AnimeGraphs.push_back(TempGraphs);
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/None.png");

	//Skill_Sword_Front 1
	AnimeGraphs.push_back(TempGraphs);
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Front_0.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Front_1.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Front_2.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Front_3.png");

	//Skill_Sword_Shockwave 2
	AnimeGraphs.push_back(TempGraphs);
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Shockwave_0.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Shockwave_1.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Shockwave_2.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Shockwave_3.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Shockwave_4.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Shockwave_5.png");

	#pragma endregion

}

bool Character::UseSkill(int SkillNumber,int EquipmentNumber)
{
	int NowTime = GetNowCount();
	StateTime -= NowTime - Time;
	Time = NowTime;

	switch(SkillNumber)
	{
		case Skill_None_None:
		{
			return true;
			break;
		}

		#pragma region 地上剣
		case Skill_Sword_Front:
		{//前方へ剣を振り下ろす
			if(StateTime > 100)return false;

			State = Skill_Sword_Front;
			StateTime = 500;
			b2PolygonShape Shape;
			Shape.SetAsBox(2.7/2,3.7/2);
			b2Transform Trans;
			b2Vec2 Pos = GetBody()->GetPosition();
			Pos.x += 2*Direction;
			Trans.Set(Pos,0);
			HitBox Box;
			HitBoxList.push_back(Box);
			HitBoxList[HitBoxList.size()-1].Initialize(Shape,Trans,this,false,b2Vec2(10*Direction,-5),10,1,200,500,true);

			return true;
			break;
		}
		case Skill_Sword_Shockwave:
		{//前方へ衝撃波
			if(StateTime > 100)return false;

			State = Skill_Sword_Shockwave;
			StateTime = 700;

			return true;
			break;
		}
		#pragma endregion
	}
	return false;
}

void Character::Step()
{
	int NowTime = GetNowCount();
	StateTime -= NowTime - Time;
	Time = NowTime;

	//アニメーション処理
	switch(State)
	{
		case Skill_None_None:
		{
			Graph[0] = AnimeGraphs[State].Graph[0];
			break;
		}

		#pragma region 地上剣
		case Skill_Sword_Front:
		{//前方へ剣を振り下ろす
			if(StateTime > 450)Graph[0] = AnimeGraphs[State].Graph[0];
			else if(StateTime > 350)Graph[0] = AnimeGraphs[State].Graph[1];
			else if(StateTime > 300)Graph[0] = AnimeGraphs[State].Graph[2];
			else if(StateTime > 0)Graph[0] = AnimeGraphs[State].Graph[3];
			else
			{
				State = Skill_None_None;
				Graph[0] = AnimeGraphs[State].Graph[0];
			};
			break;
		}
		case Skill_Sword_Shockwave:
		{//前方へ衝撃波
			if(StateTime > 600)Graph[0] = AnimeGraphs[State].Graph[0];
			else if(StateTime > 400)Graph[0] = AnimeGraphs[State].Graph[1];
			else if(StateTime > 300)Graph[0] = AnimeGraphs[State].Graph[2];
			else if(StateTime > 200)Graph[0] = AnimeGraphs[State].Graph[3];
			else if(StateTime > 150)Graph[0] = AnimeGraphs[State].Graph[4];
			else if(StateTime > 100)Graph[0] = AnimeGraphs[State].Graph[5];
			else
			{
				State = Skill_None_None;
				Graph[0] = AnimeGraphs[State].Graph[0];
			};
			break;
		}
		#pragma endregion
	}

	//ヒットボックスとの当たり判定
	int Length = HitBoxList.size();
	for(int i=0;i<Length;i++)
	{
		if(HitBoxList[i].HitTestShape(this,this->GetShape(),this->GetBody()->GetTransform()))
		{//当たっていた場合
			GetBody()->ApplyLinearImpulse(HitBoxList[i].GetHitVect(),GetBody()->GetPosition());
			HP -= HitBoxList[i].GetDamage();
		}

		if(HitBoxList[i].Step())
		{
			HitBoxList.erase(HitBoxList.begin()+i);
			i--;
			Length--;
		}else
		{
			HitBoxList[i].Draw();
		}
	}

	//キャラが倒れないようにする
	GetBody()->SetTransform(GetBody()->GetPosition(),0);
}
