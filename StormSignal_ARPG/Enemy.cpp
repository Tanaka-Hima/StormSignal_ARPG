#include "Enemy.h"
#include "Map.h"
#include "ConstantValue.h"
#include "Functions.h"

void Enemy::Initialize(b2World *World,string CharaType,float Density,float Friction,int MaxHP)
{
	//画像指定
	if(CharaType == Mapchip_TrainingBag)Load("Font/Big_Green/A.png");
	else if(CharaType == Mapchip_CommonEnemy1)Load("Image/Chara/None.png");

	InitChara(World,CharaType,Density,Friction,MaxHP);

	if(CharaType == Mapchip_CommonEnemy1)
	{
		GetShape()->SetAsBox(12.f/2.f/Box_Rate,24.f/2.f/Box_Rate);

		GetFixtureDef()->shape = GetShape();
		GetFixtureDef()->density = Density;
		GetFixtureDef()->friction = Friction;

		GetBody()->DestroyFixture(GetFixture());
		GetBody()->CreateFixture(GetFixtureDef());

		Center_x = 64;
		Center_y = 52;
	}

	State = -1;
}

void Enemy::Ctrl()
{
	if(StateTime > 0)return;
	Character* Player = GetPlayer();
	if(GetCharaType() == Mapchip_CommonEnemy1)
	{
		double Dis = GetDistance(x+Center_x,y+Center_y,Player->x+Player->Center_x,Player->y+Player->Center_y);
		if(Dis < 150 && Dis > 30)
		{
			if(Player->x-x < 0)Direction = ImageDirection_Left;
			else Direction = ImageDirection_Right;
			if(Player->x != x)GetBody()->SetLinearVelocity(b2Vec2((Player->x-x) / abs(Player->x-x) * 3,0));
		}else if(Dis <= 30)
		{
			UseSkill(Skill_Sword_Front,Equipment_Sword_Normal);
		}
	}
}

