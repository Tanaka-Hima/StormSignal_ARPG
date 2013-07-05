#include "Character.h"
#include "HitBox.h"
#include "ConstantValue.h"
#include <DxLib.h>

vector<Character*> Character::CharacterList;
vector<HitBox> Character::HitBoxList;
vector<vector<int>> Character::AnimeGraphs;

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

	vector<int> TempGraphs;

	//Skill_None_None 0
	TempGraphs.push_back(LoadGraph("Image/Chara/None.png"));
	AnimeGraphs.push_back(TempGraphs);
	TempGraphs.clear();

	//Skill_Sword_Front 1
	TempGraphs.push_back(LoadGraph("Image/Chara/Sword/Front_0.png"));
	TempGraphs.push_back(LoadGraph("Image/Chara/Sword/Front_1.png"));
	TempGraphs.push_back(LoadGraph("Image/Chara/Sword/Front_2.png"));
	TempGraphs.push_back(LoadGraph("Image/Chara/Sword/Front_3.png"));
	AnimeGraphs.push_back(TempGraphs);
	TempGraphs.clear();
}

bool Character::UseSkill(int Number)
{
	int NowTime = GetNowCount();
	StateTime -= NowTime - Time;
	Time = NowTime;

	switch(Number)
	{
		case Skill_None_None:
		{
			return true;
			break;
		}

		#pragma region 地上剣
		case Skill_Sword_Front:
		{
			if(StateTime > 100)return false;

			State = Skill_Sword_Front;
			StateTime = 500;
			HitBox Box;
			b2PolygonShape Shape;
			Shape.SetAsBox(2.7/2,3.7/2);
			b2Transform Trans;
			b2Vec2 Pos = GetBody()->GetPosition();
			Pos.x += 2*Direction;
			Trans.Set(Pos,0);
			Box.Initialize(Shape,Trans,this,false,b2Vec2(0,-30),10,1,200,500,true);
			HitBoxList.push_back(Box);

			return true;
			break;
		}
		#pragma endregion
	}
}

void Character::Step(void)
{
	int NowTime = GetNowCount();
	StateTime -= NowTime - Time;
	Time = NowTime;

	//アニメーション処理
	switch(State)
	{
		case Skill_None_None:
		{
			Graph[0] = AnimeGraphs[State][0];
			break;
		}

		#pragma region 地上剣
		case Skill_Sword_Front:
		{
			if(StateTime > 450)Graph[0] = AnimeGraphs[State][0];
			else if(StateTime > 350)Graph[0] = AnimeGraphs[State][1];
			else if(StateTime > 300)Graph[0] = AnimeGraphs[State][2];
			else if(StateTime > 0)Graph[0] = AnimeGraphs[State][3];
			else
			{
				State = Skill_None_None;
				Graph[0] = AnimeGraphs[State][0];
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
