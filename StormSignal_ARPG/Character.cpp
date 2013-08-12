#include "Character.h"
#include "HitBox.h"
#include "ConstantValue.h"
#include "Functions.h"
#include "Map.h"
#include <DxLib.h>

vector<Character*> Character::CharacterList;
vector<HitBox> Character::HitBoxList;
vector<Image_2D> Character::AnimeGraphs;
vector<Image_2D> Character::EquipmentGraphs;
vector<Image_2D> Character::EffectGraphs;

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

void Character::InitChara(b2World *World,string CharaType,float Density,float Friction,int MaxHP)
{
	CharacterType = CharaType;
	Init(World,&CharacterType,Density,Friction);

	HP = MaxHP;
	GetBody()->SetSleepingAllowed(false);

	CharacterList.push_back(this);

	State = Skill_None_None;
	StateTime = 0;
	BeforeStateTime = 0;
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

	//Skill_None_Damage 3
	AnimeGraphs.push_back(TempGraphs);
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/None/Damage_0.png");

	//Skill_None_Frontstep 4
	AnimeGraphs.push_back(TempGraphs);
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/None.png");

	//Skill_None_Backstep 5
	AnimeGraphs.push_back(TempGraphs);
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/None.png");

	#pragma endregion

	#pragma region エフェクト画像読み込み

	//Effect_Sword_Shockwave 0
	EffectGraphs.push_back(TempGraphs);
	int ShockwaveArray[6];
	LoadDivGraph("Image/Effect/Sword/Shockwave.dds",6,3,2,240,240,ShockwaveArray);
	for(int i=0;i<6;i++)EffectGraphs[EffectGraphs.size()-1].Graph.push_back(ShockwaveArray[i]);
	EffectGraphs[EffectGraphs.size()-1].Initialize();
	EffectGraphs[EffectGraphs.size()-1].Ext = 0.2f;
	EffectGraphs[EffectGraphs.size()-1].Anime_Speed = 50;

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
			if(!JudgeSkillCancel())return false;

			State = Skill_Sword_Front;
			StateTime = 500;

			return true;
			break;
		}
		case Skill_Sword_Shockwave:
		{//前方へ衝撃波
			if(!JudgeSkillCancel())return false;

			State = Skill_Sword_Shockwave;
			StateTime = 700;

			return true;
			break;
		}
		#pragma endregion

		#pragma region 行動

		case Skill_None_Frontstep:
		{
			State = Skill_None_Frontstep;
			StateTime = 1000;

			GetBody()->ApplyForceToCenter(b2Vec2((int)Direction*1000,-100));
			break;
		}

		case Skill_None_Backstep:
		{
			State = Skill_None_Backstep;
			StateTime = 1000;

			GetBody()->ApplyForceToCenter(b2Vec2(-(int)Direction*1000,-100));
			break;
		}

		#pragma endregion

	}
	return false;
}

bool Character::JudgeSkillCancel()
{
	switch(State)
	{
		case Skill_None_None:
			return true;
		case Skill_Sword_Front:
			if(StateTime < 450)return true;
			else return false;
		case Skill_Sword_Shockwave:
			if(StateTime < 400)return true;
			else return false;
		case Skill_None_Frontstep:
			if(StateTime < 950)return true;
			else return false;
		case Skill_None_Backstep:
			if(StateTime < 950)return true;
			else return false;
	}
}

void Character::Step()
{
	int NowTime = GetNowCount();
	StateTime -= NowTime - Time;
	if(StateTime < 0)State = Skill_None_None;
	Time = NowTime;

	//アニメーション処理
	switch(State)
	{
		case Skill_None_None:
		case Skill_None_Frontstep:
		case Skill_None_Backstep:
		{
			Graph[0] = AnimeGraphs[State].Graph[0];
			break;
		}

		case Skill_None_Stan:
		{
			Graph[0] = AnimeGraphs[State].Graph[0];
			break;
		}

		#pragma region 地上剣
		case Skill_Sword_Front:
		{//前方へ剣を振り下ろす
			if(StateTime > 450)Graph[0] = AnimeGraphs[State].Graph[0];
			else if(StateTime > 350)
			{
				if(BeforeStateTime > 450)
				{//ヒットボックスを発生させる
					b2PolygonShape Shape;
					Shape.SetAsBox(2.7/2,3.7/2);
					b2Transform Trans;
					b2Vec2 Pos = GetBody()->GetPosition();
					Pos.x += 2*Direction;
					Trans.Set(Pos,0);
					HitBox Box;
					HitBoxList.push_back(Box);
					HitBoxList[HitBoxList.size()-1].Initialize(Shape,Trans,this,false,b2Vec2(10*Direction,-5),10,1,50,500,true);
				}
				Graph[0] = AnimeGraphs[State].Graph[1];
			}else if(StateTime > 300)Graph[0] = AnimeGraphs[State].Graph[2];
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
			else if(StateTime > 300)
			{
				if(BeforeStateTime > 400)
				{
					b2PolygonShape Shape;
					Shape.SetAsBox(2.7/2,3.7/2);
					b2Transform Trans;
					b2Vec2 Pos = GetBody()->GetPosition();
					Pos.x += 2*Direction;
					Trans.Set(Pos,0);
					HitBox Box;
					HitBoxList.push_back(Box);
					HitBoxList[HitBoxList.size()-1].Initialize(Shape,Trans,this,false,b2Vec2(10*Direction,-5),10,1,1000,500,false);
					EffectGraphs[Effect_Sword_Shockwave].Direction = Direction;
					HitBoxList[HitBoxList.size()-1].SetGraph(EffectGraphs[Effect_Sword_Shockwave]);
					HitBoxList[HitBoxList.size()-1].SetMoveFlag(b2Vec2(Direction*10,0));
				}
				Graph[0] = AnimeGraphs[State].Graph[2];
			}else if(StateTime > 200)Graph[0] = AnimeGraphs[State].Graph[3];
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
			State = Skill_None_Stan;
			StateTime = HitBoxList[i].GetStanTime();
		}

		if(HitBoxList[i].Step())
		{
			HitBoxList.erase(HitBoxList.begin()+i);
			i--;
			Length--;
		}
	}

	//キャラが倒れないようにする
	GetBody()->SetTransform(GetBody()->GetPosition(),0);

	//BeforeStateTimeの更新
	BeforeStateTime = StateTime;
}

Character* Character::GetPlayer()
{
	int Length = CharacterList.size();
	for(int i=0;i<Length;i++)
	{
		if(CharacterList[i]->CharacterType == Mapchip_Player)return CharacterList[i];
	}
}

string Character::GetCharaType()
{
	return CharacterType;
}

void Character::DeleteCharacterList()
{
	int Length = CharacterList.size();
	for(int i=0;i<Length;i++)
	{
		if(CharacterList[i] == this)
		{
			CharacterList.erase(CharacterList.begin()+i);
			break;
		}
	}
}