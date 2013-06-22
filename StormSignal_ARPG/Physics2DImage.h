#include "Image_2D.h"
#include <Box2D\Box2D.h>

#ifndef _PHYSICS2DIMAGE
#define _PHYSICS2DIMAGE

class Physics2DImage : public Image_2D
{
private:
	b2BodyDef BodyDef;
	b2Body* Body;
	b2PolygonShape DynamicBox;
	b2FixtureDef FixtureDef;

public:
	void Initialize(b2World *World,void* UserData,float Density,float Friction);
	bool Draw(bool Trans);
};

#endif