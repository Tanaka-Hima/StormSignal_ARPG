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
vector<Image_2D> Character::Effects;

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

void Character::InitChara(b2World *World,string CharaType,float Density,float Friction,int InputMaxHP)
{
	CharacterType = CharaType;
	Init(World,&CharacterType,Density,Friction);

	MaxHP = InputMaxHP;
	HP = MaxHP;
	GetBody()->SetSleepingAllowed(false);

	CharacterList.push_back(this);

	State = Skill_None_None;
	StateTime = 0;
	AffectStanTime = 0;
	ComboCount = 0;
	BeforeStateTime = 0;
	BeforeVect = b2Vec2(0,0);
	ScrollDistance = 0;
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

	//Equipment_Handgun_Normal 3
	EquipmentGraphs.push_back(TempGraphs);
	EquipmentGraphs[EquipmentGraphs.size()-1].Load("Image/Equipment/Handgun/Normal_0.png");


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

	//Skill_Sword_Knockup 6
	AnimeGraphs.push_back(TempGraphs);
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Knockup_0.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Knockup_1.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Knockup_2.png");

	//Skill_Handgun_Fire 7
	AnimeGraphs.push_back(TempGraphs);
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Handgun/Fire_0.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Handgun/Fire_1.png");

	//Skill_Sword_Smash 8
	AnimeGraphs.push_back(TempGraphs);
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Smash_0.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Smash_1.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Smash_2.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Smash_3.png");

	//Skill_Sword_Spin 9
	AnimeGraphs.push_back(TempGraphs);
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Spin_0.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Spin_1.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Spin_2.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Spin_3.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/Spin_4.png");

	//Skill_Sword_StrikeTop 10
	AnimeGraphs.push_back(TempGraphs);
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/StrikeTop_0.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/StrikeTop_1.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/StrikeTop_2.png");
	AnimeGraphs[AnimeGraphs.size()-1].Load("Image/Skill/Sword/StrikeTop_3.png");

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

	//Effect_Handgun_Bullet 1
	EffectGraphs.push_back(TempGraphs);
	EffectGraphs[EffectGraphs.size()-1].Load("Image/Effect/Handgun/Bullet.png");
	EffectGraphs[EffectGraphs.size()-1].Initialize();

	//Effect_Other_Hit0 2
	EffectGraphs.push_back(TempGraphs);
	int Hit0Array[5];
	int SIHandle = LoadSoftImage("Image/Effect/Other/Hit0.dds");
	int Width,Height;
	GetSoftImageSize(SIHandle,&Width,&Height);
	for(int x=0;x<Width;x++)
	{
		for(int y=0;y<Height;y++)
		{
			int r,g,b,a;
			GetColor2(Red,&r,&g,&b);
			GetPixelSoftImage(SIHandle,x,y,0,0,0,&a);
			DrawPixelSoftImage(SIHandle,x,y,r,g,b,a);
		}
	}
	CreateDivGraphFromSoftImage(SIHandle,5,5,1,240,240,Hit0Array);
	for(int i=0;i<5;i++)EffectGraphs[EffectGraphs.size()-1].Graph.push_back(Hit0Array[i]);
	EffectGraphs[EffectGraphs.size()-1].Initialize();
	EffectGraphs[EffectGraphs.size()-1].Ext = 0.2f;
	EffectGraphs[EffectGraphs.size()-1].Anime_Speed = 30;

	//Effect_Other_Hit1 3
	EffectGraphs.push_back(TempGraphs);
	int Hit1Array[5];
	SIHandle = LoadSoftImage("Image/Effect/Other/Hit1.dds");
	GetSoftImageSize(SIHandle,&Width,&Height);
	for(int x=0;x<Width;x++)
	{
		for(int y=0;y<Height;y++)
		{
			int r,g,b,a;
			GetColor2(Red,&r,&g,&b);
			GetPixelSoftImage(SIHandle,x,y,0,0,0,&a);
			DrawPixelSoftImage(SIHandle,x,y,r,g,b,a);
		}
	}
	CreateDivGraphFromSoftImage(SIHandle,5,5,1,240,240,Hit1Array);
	for(int i=0;i<5;i++)EffectGraphs[EffectGraphs.size()-1].Graph.push_back(Hit1Array[i]);
	EffectGraphs[EffectGraphs.size()-1].Initialize();
	EffectGraphs[EffectGraphs.size()-1].Ext = 0.3f;
	EffectGraphs[EffectGraphs.size()-1].Anime_Speed = 40;

	//Effect_Other_StrikeGround 4
	EffectGraphs.push_back(TempGraphs);

	int StrikeGroundArray[6];
	SIHandle = LoadSoftImage("Image/Effect/Other/StrikeGround.dds");
	GetSoftImageSize(SIHandle,&Width,&Height);
	for(int x=0;x<Width;x++)
	{
		for(int y=0;y<Height;y++)
		{
			int r,g,b,a;
			GetColor2(SaddleBrown,&r,&g,&b);
			GetPixelSoftImage(SIHandle,x,y,0,0,0,&a);
			if(a != 0)a *= 2;
			DrawPixelSoftImage(SIHandle,x,y,r,g,b,a);
		}
	}
	CreateDivGraphFromSoftImage(SIHandle,6,6,1,240,240,StrikeGroundArray);
	for(int i=0;i<6;i++)EffectGraphs[EffectGraphs.size()-1].Graph.push_back(StrikeGroundArray[i]);
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
		}

		#pragma region 地上剣
		case Skill_Sword_Front:
		{//前方へ剣を振り下ろす
			if(!JudgeSkillCancel())return false;

			State = Skill_Sword_Front;
			StateTime = 500;

			return true;
		}
		case Skill_Sword_Shockwave:
		{//前方へ衝撃波
			if(!JudgeSkillCancel())return false;

			State = Skill_Sword_Shockwave;
			StateTime = 700;

			return true;
		}
		case Skill_Sword_Knockup:
		{//打ち上げ
			if(!JudgeSkillCancel())return false;

			State = Skill_Sword_Knockup;
			StateTime = 700;

			return true;
		}
		case Skill_Sword_Smash:
		{//前方へ吹き飛ばし攻撃
			if(!JudgeSkillCancel())return false;

			State = Skill_Sword_Smash;
			StateTime = 700;

			return true;
		}
		case Skill_Sword_Spin:
		{//剣を回転させる
			if(!JudgeSkillCancel())return false;

			State = Skill_Sword_Spin;
			StateTime = 600;

			return true;
		}
		case Skill_Sword_StrikeTop:
		{//上昇しつつ打ち上げる
			if(!JudgeSkillCancel())return false;

			State = Skill_Sword_StrikeTop;
			StateTime = 700;

			return true;
		}
		#pragma endregion

		#pragma region ハンドガン
		case Skill_Handgun_Fire:
		{
			if(!JudgeSkillCancel())return false;

			State = Skill_Handgun_Fire;
			StateTime = 200;
			return true;
		}
		#pragma endregion

		#pragma region 行動

		case Skill_None_Frontstep:
		{
			if(!JudgeSkillCancel())return false;

			State = Skill_None_Frontstep;
			StateTime = 1000;

			GetBody()->ApplyForceToCenter(b2Vec2((int)Direction*1000,-100));
			return true;
		}

		case Skill_None_Backstep:
		{
			if(!JudgeSkillCancel())return false;

			State = Skill_None_Backstep;
			StateTime = 1000;

			GetBody()->ApplyForceToCenter(b2Vec2(-(int)Direction*1000,-100));
			return true;
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
			if(StateTime < 350)return true;
			else return false;
		case Skill_Sword_Shockwave:
			if(StateTime < 300)return true;
			else return false;
		case Skill_Sword_Knockup:
			if(StateTime < 450)return true;
			else return false;
		case Skill_Sword_Smash:
			if(StateTime < 300)return true;
			else return false;
		case Skill_Sword_Spin:
			if(StateTime < 150)return true;
			else return false;
		case Skill_Sword_StrikeTop:
			if(StateTime < 300)return true;
			else return false;
		case Skill_Handgun_Fire:
			if(StateTime < 80)return true;
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
	AffectStanTime -= NowTime - Time;
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
					HitBoxList[HitBoxList.size()-1].SetEffect(EffectGraphs[Effect_Other_Hit0]);
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
					HitBoxList[HitBoxList.size()-1].SetEffect(EffectGraphs[Effect_Other_Hit1]);
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
		case Skill_Sword_Knockup:
		{//打ち上げ
			if(StateTime > 550)Graph[0] = AnimeGraphs[State].Graph[0];
			else if(StateTime > 450)
			{
				if(BeforeStateTime > 550)
				{
					b2PolygonShape Shape;
					Shape.SetAsBox(2.7/2,3.7/2);
					b2Transform Trans;
					b2Vec2 Pos = GetBody()->GetPosition();
					Pos.x += 2*Direction;
					Trans.Set(Pos,0);
					HitBox Box;
					HitBoxList.push_back(Box);
					HitBoxList[HitBoxList.size()-1].Initialize(Shape,Trans,this,false,b2Vec2(0,-30),10,1,50,1000,false);
					HitBoxList[HitBoxList.size()-1].SetEffect(EffectGraphs[Effect_Other_Hit0]);
				}
				Graph[0] = AnimeGraphs[State].Graph[1];
			}else if(StateTime > 100)Graph[0] = AnimeGraphs[State].Graph[2];
			else
			{
				State = Skill_None_None;
				Graph[0] = AnimeGraphs[State].Graph[0];
			};
			break;
		}
		case Skill_Sword_Smash:
		{//前方へ吹き飛ばし攻撃
			if(StateTime > 600)Graph[0] = AnimeGraphs[State].Graph[0];
			else if(StateTime > 400)Graph[0] = AnimeGraphs[State].Graph[1];
			else if(StateTime > 300)
			{
				if(BeforeStateTime > 400)
				{//ヒットボックスを発生させる
					b2PolygonShape Shape;
					Shape.SetAsBox(6.8/2,2.0/2);
					b2Transform Trans;
					b2Vec2 Pos = GetBody()->GetPosition();
					Pos.x += Direction;
					Trans.Set(Pos,0);
					HitBox Box;
					HitBoxList.push_back(Box);
					HitBoxList[HitBoxList.size()-1].Initialize(Shape,Trans,this,false,b2Vec2(30*Direction,-5),30,1,100,1000,true);
					HitBoxList[HitBoxList.size()-1].SetEffect(EffectGraphs[Effect_Other_Hit0]);
				}
				Graph[0] = AnimeGraphs[State].Graph[2];
			}else if(StateTime > 100)Graph[0] = AnimeGraphs[State].Graph[3];
			else
			{
				State = Skill_None_None;
				Graph[0] = AnimeGraphs[State].Graph[0];
			};
			break;
		}
		case Skill_Sword_Spin:
		{//剣を回転させる
			if(StateTime > 550)Graph[0] = AnimeGraphs[State].Graph[0];
			else if(StateTime > 500)
			{
				if(BeforeStateTime > 550)
				{//ヒットボックスを発生させる
					b2PolygonShape Shape;
					Shape.SetAsBox(2.7/2,3.7/2);
					b2Transform Trans;
					b2Vec2 Pos = GetBody()->GetPosition();
					Pos.x += 2*Direction;
					Trans.Set(Pos,0);
					HitBox Box;
					HitBoxList.push_back(Box);
					HitBoxList[HitBoxList.size()-1].Initialize(Shape,Trans,this,false,b2Vec2(-5*Direction,-5),10,1,50,500,true);
					HitBoxList[HitBoxList.size()-1].SetEffect(EffectGraphs[Effect_Other_Hit0]);
				}
				Graph[0] = AnimeGraphs[State].Graph[1];
			}else if(StateTime > 450)
			{
				if(BeforeStateTime > 500)
				{//ヒットボックスを発生させる
					b2PolygonShape Shape;
					Shape.SetAsBox(5.3/2,2.4/2);
					b2Transform Trans;
					b2Vec2 Pos = GetBody()->GetPosition();
					Pos.x += 0.1*Direction;
					Pos.y += -2;
					Trans.Set(Pos,0);
					HitBox Box;
					HitBoxList.push_back(Box);
					HitBoxList[HitBoxList.size()-1].Initialize(Shape,Trans,this,false,b2Vec2(-5*Direction,5),10,1,50,500,true);
					HitBoxList[HitBoxList.size()-1].SetEffect(EffectGraphs[Effect_Other_Hit0]);
				}
				Graph[0] = AnimeGraphs[State].Graph[2];
			}else if(StateTime > 400)
			{
				if(BeforeStateTime > 450)
				{//ヒットボックスを発生させる
					b2PolygonShape Shape;
					Shape.SetAsBox(2.7/2,3.7/2);
					b2Transform Trans;
					b2Vec2 Pos = GetBody()->GetPosition();
					Pos.x += -2*Direction;
					Trans.Set(Pos,0);
					HitBox Box;
					HitBoxList.push_back(Box);
					HitBoxList[HitBoxList.size()-1].Initialize(Shape,Trans,this,false,b2Vec2(5*Direction,5),10,1,50,500,true);
					HitBoxList[HitBoxList.size()-1].SetEffect(EffectGraphs[Effect_Other_Hit0]);
				}
				Graph[0] = AnimeGraphs[State].Graph[3];
			}else if(StateTime > 350)
			{
				if(BeforeStateTime > 400)
				{//ヒットボックスを発生させる
					b2PolygonShape Shape;
					Shape.SetAsBox(5.3/2,2.4/2);
					b2Transform Trans;
					b2Vec2 Pos = GetBody()->GetPosition();
					Pos.x += 0.1*Direction;
					Pos.y += 2;
					Trans.Set(Pos,0);
					HitBox Box;
					HitBoxList.push_back(Box);
					HitBoxList[HitBoxList.size()-1].Initialize(Shape,Trans,this,false,b2Vec2(5*Direction,-5),10,1,50,500,true);
					HitBoxList[HitBoxList.size()-1].SetEffect(EffectGraphs[Effect_Other_Hit0]);
				}
				Graph[0] = AnimeGraphs[State].Graph[4];
			}else if(StateTime > 300)
			{
				if(BeforeStateTime > 350)
				{//ヒットボックスを発生させる
					b2PolygonShape Shape;
					Shape.SetAsBox(2.7/2,3.7/2);
					b2Transform Trans;
					b2Vec2 Pos = GetBody()->GetPosition();
					Pos.x += 2*Direction;
					Trans.Set(Pos,0);
					HitBox Box;
					HitBoxList.push_back(Box);
					HitBoxList[HitBoxList.size()-1].Initialize(Shape,Trans,this,false,b2Vec2(-5*Direction,-5),10,1,50,500,true);
					HitBoxList[HitBoxList.size()-1].SetEffect(EffectGraphs[Effect_Other_Hit0]);
				}
				Graph[0] = AnimeGraphs[State].Graph[1];
			}else if(StateTime > 250)
			{
				if(BeforeStateTime > 300)
				{//ヒットボックスを発生させる
					b2PolygonShape Shape;
					Shape.SetAsBox(5.3/2,2.4/2);
					b2Transform Trans;
					b2Vec2 Pos = GetBody()->GetPosition();
					Pos.x += 0.1*Direction;
					Pos.y += -2;
					Trans.Set(Pos,0);
					HitBox Box;
					HitBoxList.push_back(Box);
					HitBoxList[HitBoxList.size()-1].Initialize(Shape,Trans,this,false,b2Vec2(-5*Direction,5),10,1,50,500,true);
					HitBoxList[HitBoxList.size()-1].SetEffect(EffectGraphs[Effect_Other_Hit0]);
				}
				Graph[0] = AnimeGraphs[State].Graph[2];
			}else if(StateTime > 200)
			{
				if(BeforeStateTime > 250)
				{//ヒットボックスを発生させる
					b2PolygonShape Shape;
					Shape.SetAsBox(2.7/2,3.7/2);
					b2Transform Trans;
					b2Vec2 Pos = GetBody()->GetPosition();
					Pos.x += -2*Direction;
					Trans.Set(Pos,0);
					HitBox Box;
					HitBoxList.push_back(Box);
					HitBoxList[HitBoxList.size()-1].Initialize(Shape,Trans,this,false,b2Vec2(5*Direction,5),10,1,50,500,true);
					HitBoxList[HitBoxList.size()-1].SetEffect(EffectGraphs[Effect_Other_Hit0]);
				}
				Graph[0] = AnimeGraphs[State].Graph[3];
			}else if(StateTime > 150)
			{
				if(BeforeStateTime > 200)
				{//ヒットボックスを発生させる
					b2PolygonShape Shape;
					Shape.SetAsBox(5.3/2,2.4/2);
					b2Transform Trans;
					b2Vec2 Pos = GetBody()->GetPosition();
					Pos.x += 0.1*Direction;
					Pos.y += 2;
					Trans.Set(Pos,0);
					HitBox Box;
					HitBoxList.push_back(Box);
					HitBoxList[HitBoxList.size()-1].Initialize(Shape,Trans,this,false,b2Vec2(5*Direction,-5),10,1,50,500,true);
					HitBoxList[HitBoxList.size()-1].SetEffect(EffectGraphs[Effect_Other_Hit0]);
				}
				Graph[0] = AnimeGraphs[State].Graph[4];
			}else
			{
				State = Skill_None_None;
				Graph[0] = AnimeGraphs[State].Graph[0];
			};
			break;
		case Skill_Sword_StrikeTop:
		{//上昇しつつ打ち上げる
			if(StateTime > 450)Graph[0] = AnimeGraphs[State].Graph[0];
			else if(StateTime > 400)Graph[0] = AnimeGraphs[State].Graph[1];
			else if(StateTime > 350)
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
					HitBoxList[HitBoxList.size()-1].Initialize(Shape,Trans,this,false,b2Vec2(0,-35),10,1,100,1000,true);
					HitBoxList[HitBoxList.size()-1].SetEffect(EffectGraphs[Effect_Other_Hit0]);

					b2Vec2 Vect = GetBody()->GetLinearVelocity();
					Vect.y = -MoveSpeed*2.5;
					GetBody()->SetLinearVelocity(Vect);
				}
				Graph[0] = AnimeGraphs[State].Graph[2];
			}else if(StateTime > 300)Graph[0] = AnimeGraphs[State].Graph[3];
			else
			{
				State = Skill_None_None;
				Graph[0] = AnimeGraphs[State].Graph[0];
			};
			break;
		}
		}
		#pragma endregion

		#pragma region ハンドガン
		case Skill_Handgun_Fire:
		{
			if(StateTime > 80)Graph[0] = AnimeGraphs[State].Graph[0];
			else if(StateTime > 30)
			{
				if(BeforeStateTime > 80)
				{//ヒットボックスを発生させる
					b2PolygonShape Shape;
					Shape.SetAsBox(1.2/2,0.7/2);
					b2Transform Trans;
					b2Vec2 Pos = GetBody()->GetPosition();
					Pos.x += 2*Direction;
					Trans.Set(Pos,0);
					HitBox Box;
					HitBoxList.push_back(Box);
					HitBoxList[HitBoxList.size()-1].Initialize(Shape,Trans,this,false,b2Vec2(2*Direction,0),2,1,1000,500,false);
					EffectGraphs[Effect_Sword_Shockwave].Direction = Direction;
					HitBoxList[HitBoxList.size()-1].SetGraph(EffectGraphs[Effect_Handgun_Bullet]);
					HitBoxList[HitBoxList.size()-1].SetEffect(EffectGraphs[Effect_Other_Hit1]);

					//方向指定
					float Y = 0.f;
					if(CheckHitKey(KEY_INPUT_UP))Y = -10;
					else if(CheckHitKey(KEY_INPUT_DOWN))Y = 10;

					HitBoxList[HitBoxList.size()-1].SetMoveFlag(b2Vec2(Direction*40,Y),true);
				}
				Graph[0] = AnimeGraphs[State].Graph[1];
			}else if(StateTime > 0)Graph[0] = AnimeGraphs[State].Graph[0];
			else
			{
				State = Skill_None_None;
				Graph[0] = AnimeGraphs[State].Graph[0];
			};
			break;
		}
		#pragma endregion

	}

	for(b2ContactEdge *i = GetBody()->GetContactList();i;i = i->next)
	{
		if(i->contact->GetManifold()->pointCount <= 0)continue;

		//叩きつけエフェクトの処理
		double Angle;
		int Center_y = EffectGraphs[Effect_Other_StrikeGround].Center_y;
		
		if(i->contact->GetManifold()->localNormal.x == 0 && i->contact->GetManifold()->localNormal.y == 1
			&& fabs(BeforeVect.y) > MoveSpeed){Angle = 0;Center_y *= 1.5;}
		else if(i->contact->GetManifold()->localNormal.x == 0 && i->contact->GetManifold()->localNormal.y == -1
			&& fabs(BeforeVect.y) > MoveSpeed){Angle = PI;Center_y *= 1.5;}
		else if(i->contact->GetManifold()->localNormal.x == 1 && i->contact->GetManifold()->localNormal.y == 0
			&& fabs(BeforeVect.x) > MoveSpeed){Angle = -PI/2;Center_y *= 1.5;}
		else if(i->contact->GetManifold()->localNormal.x == -1 && i->contact->GetManifold()->localNormal.y == 0
			&& fabs(BeforeVect.x) > MoveSpeed){Angle = PI/2;Center_y *= 1.5;}
		else continue;

		Effects.push_back(EffectGraphs[Effect_Other_StrikeGround]);
		Effects[Effects.size()-1].x = ((i->contact->GetManifold()->points[0].localPoint.x + i->contact->GetManifold()->points[1].localPoint.x)/2+ScrollDistance)*Box_Rate;
		Effects[Effects.size()-1].y = (i->contact->GetManifold()->points[0].localPoint.y + i->contact->GetManifold()->points[1].localPoint.y)/2*Box_Rate;
		Effects[Effects.size()-1].Angle = Angle;
		Effects[Effects.size()-1].Center_y = Center_y;
		break;
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

	//コンボが途切れているかどうかの処理
	if(AffectStanTime < 0)
	{
		ComboCount = 0;
	}

	//BeforeStateTimeの更新
	BeforeStateTime = StateTime;

	//BeforeSpeedの更新
	BeforeVect = GetBody()->GetLinearVelocity();
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

void Character::SetScrollDistance(float Distance)
{
	ScrollDistance = Distance;
}