#include "Enemy.h"
#include "Map.h"
#include "ConstantValue.h"
#include "Functions.h"

void Enemy::Initialize(b2World *World,string CharaType,float Density,float Friction,int MaxHP)
{
	//画像指定
	if(CharaType == Mapchip_TrainingBag)Load("Image/Chara/None.png");
	else if(CharaType == Mapchip_CommonEnemy1)Load("Image/Chara/None.png");

	InitChara(World,CharaType,Density,Friction,MaxHP);

	if(CharaType == Mapchip_CommonEnemy1
		|| CharaType == Mapchip_TrainingBag)
	{
		GetShape()->SetAsBox(12.f/2.f/Box_Rate,24.f/2.f/Box_Rate);

		GetFixtureDef()->shape = GetShape();
		GetFixtureDef()->density = Density;
		GetFixtureDef()->friction = Friction;

		GetBody()->DestroyFixture(GetFixture());
		GetBody()->CreateFixture(GetFixtureDef());

		Center_x = 64;
		Center_y = 52;

		Width = 9;
		Height = 24;
	}

	State = -1;
}

void Enemy::Ctrl()
{
	if(StateTime > 0 || fabs(GetBody()->GetLinearVelocity().y) > 0.2f)return;
	Character* Player = GetPlayer();
	if(GetCharaType() == Mapchip_CommonEnemy1)
	{
		double Dis = GetDistance(x,y,Player->x,Player->y);
		if(Dis < 150 && Dis > 30)
		{
			if(Player->x-x < 0)Direction = ImageDirection_Left;
			else Direction = ImageDirection_Right;
			if(Player->x != x)GetBody()->SetLinearVelocity(b2Vec2((Player->x-x) / abs(Player->x-x) * 3,0));
		}else if(Dis <= 30)
		{
			UseSkill(Skill_Sword_Front,Equipment_Sword_Normal);
			StateTime += 300;
		}
	}
}

