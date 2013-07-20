#include "Enemy.h"

void Enemy::Initialize(b2World *World,void* UserData,float Density,float Friction,int MaxHP)
{
	Init(World,UserData,Density,Friction);

	HP = MaxHP;
	GetBody()->SetSleepingAllowed(false);
}

void Enemy::Ctrl(void)
{

}