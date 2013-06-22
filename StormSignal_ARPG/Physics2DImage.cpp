#include "Physics2DImage.h"
#include "ConstantValue.h"
#include <DxLib.h>

void Physics2DImage::Initialize(b2World *World,void* UserData,float Density,float Friction)
{
	int SizeW,SizeH;
	//while(CheckHandleASyncLoad(Graph[0]) && ProcessMessage() == 0){}
	GetGraphSize(Graph[0],&SizeW,&SizeH);
	Visible = true;
	x = 0;
	y = 0;
	Center_x = SizeW / 2;
	Center_y = SizeH / 2;
	Ext = 1.0f;
	Angle = 0.0f;
	
	Anime_Speed = 1;
	Anime_Frame = 0;
	Anime_ShowNum = 0;

	BodyDef.type = b2_dynamicBody;
	BodyDef.position.Set(x/Box_Rate, y/Box_Rate);
	Body = World->CreateBody(&BodyDef);
	Body->SetUserData(UserData);

	DynamicBox.SetAsBox((float)Center_x/(float)Box_Rate, (float)Center_y/(float)Box_Rate);

	FixtureDef.shape = &DynamicBox;
	FixtureDef.density = Density;
	FixtureDef.friction = Friction;

	Body->CreateFixture(&FixtureDef);
}

//•`‰æ
bool Physics2DImage::Draw(bool Trans)
{//•`‰æ
	if(Graph.size() > 1)Anime_Frame++;
	if(Anime_Speed <= 0)Anime_Speed = 1;

	if(Visible)
	{
		b2Vec2 Vect = Body->GetLinearVelocity();
		b2Vec2 Pos = Body->GetPosition();
		Pos.x *= Box_Rate;
		Pos.y *= Box_Rate;

		if(Vect.x < 0)DrawRotaGraph2(Pos.x,Pos.y,Center_x,Center_y,Ext,Body->GetAngle(),Graph[Anime_ShowNum],Trans,false);
		else DrawRotaGraph2(Pos.x,Pos.y,Center_x,Center_y,Ext,Body->GetAngle(),Graph[Anime_ShowNum],Trans,true);
	}

	if((Anime_Frame % Anime_Speed) == 0)Anime_ShowNum++;
	if(Anime_ShowNum >= (int)Graph.size())
	{
		Anime_ShowNum = 0;
		return true;
	}
	return false;

}