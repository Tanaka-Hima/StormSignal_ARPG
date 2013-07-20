#include "Character.h"

#ifndef _ENEMY
#define _ENEMY

class Enemy : public Character
{
private:

public:
	void Initialize(b2World *World,void* UserData,float Density,float Friction,int MaxHP);
	void Ctrl(void);
};

#endif