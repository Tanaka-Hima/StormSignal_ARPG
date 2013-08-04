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
	int Direction;//進行方向

	virtual void Init(b2World *World,void* UserData,float Density,float Friction);//初期化
	virtual bool Draw(bool Trans = true,bool AutoDirection = false);//描画
	b2Body* GetBody();//b2Bodyを取得
	b2PolygonShape* GetShape();//b2PolygonShapeを取得
	b2FixtureDef* GetFixtureDef();//b2FixtureDefを取得
	b2Fixture* GetFixture();//b2Fixtureを取得
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