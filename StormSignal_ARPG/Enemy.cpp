#include "Enemy.h"

void Enemy::Initialize(b2World *World,void* UserData,float Density,float Friction,int MaxHP)
{
	InitChara(World,UserData,Density,Friction,MaxHP);

	State = -1;
}

void Enemy::Ctrl()
{
	GetBody()->GetUserData();
}