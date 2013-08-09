#include "Enemy.h"
#include "Map.h"

void Enemy::Initialize(b2World *World,void* UserData,float Density,float Friction,int MaxHP)
{
	//画像指定
	string str = (char*)UserData;
	if(str.find(Mapchip_TrainingBag) != string::npos)Load("Font/Big_Green/A.png");

	InitChara(World,UserData,Density,Friction,MaxHP);

	State = -1;
}

void Enemy::Ctrl()
{
	GetBody()->GetUserData();
}