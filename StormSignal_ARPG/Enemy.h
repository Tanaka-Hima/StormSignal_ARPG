#include "Character.h"

#ifndef _ENEMY
#define _ENEMY

class Enemy : public Character
{
private:

public:
	void Initialize(b2World *World,string UserData,float Density,float Friction,int MaxHP);//初期化
	void Ctrl();
	~Enemy()
	{

	}
};

#endif