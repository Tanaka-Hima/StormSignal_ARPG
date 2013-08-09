#include "Character.h"

#ifndef _OBJECT
#define _OBJECT

class Object : public Character
{
private:

public:
	void Initialize(b2World *World,string CharaType,float Density,float Friction,int MaxHP);
	void Ctrl();
};

#endif