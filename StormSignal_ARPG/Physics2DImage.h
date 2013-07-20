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
	b2Fixture* Fixture;
public:
	int Direction;

	virtual void Init(b2World *World,void* UserData,float Density,float Friction);
	virtual bool Draw(bool Trans = true,bool AutoDirection = false);
	b2Body* GetBody();
	b2PolygonShape* GetShape();
	b2FixtureDef* GetFixtureDef();
	b2Fixture* GetFixture();
	/*
	~Physics2DImage()
	{
		Unload();
		b2World *World = Body->GetWorld();
		World->DestroyBody(Body);
	}
	*/
};

#endif