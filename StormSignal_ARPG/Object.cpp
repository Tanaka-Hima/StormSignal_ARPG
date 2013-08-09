#include "Object.h"

void Object::Initialize(b2World *World,string CharaType,float Density,float Friction,int MaxHP)
{
	InitChara(World,CharaType,Density,Friction,MaxHP);

	State = -1;
}

void Object::Ctrl()
{
	GetBody()->GetUserData();
}