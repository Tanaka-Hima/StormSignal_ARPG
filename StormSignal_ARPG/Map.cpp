#include <string>
#include <DxLib.h>
#include <Box2D\Box2D.h>
#include "Map.h"
#include "Functions.h"
#include "ConstantValue.h"

void Map::Initialize(void)
{
	//空欄 0
	MapChips.push_back(-1);

	//土 1
	MapChips.push_back(LoadGraph("Image/Map/1.png"));

	//土上 2
	MapChips.push_back(LoadGraph("Image/Map/2.png"));

}

void Map::LoadMapData(string Pass)
{
	int LineData = FileRead_open(Pass.c_str());
	Width = FileRead_size(Pass.c_str())/14+2;
	for(int i=0;i<14;i++)
	{
		char *Data;
		Data = (char*)malloc(Width);
		FileRead_gets(Data,Width,LineData);
		vector<char> DataTemp;
		
		for(int j=0;j<Width;j++)
		{
			//改行コード、null文字の削除
			if(Data[j] == 10 || Data[j] == 0 || Data[j] == 13)continue;
			MapData[i].push_back(Data[j]);
		}
	}
	Width-=3;
}

void Map::CreateMap(b2World *World)
{
	GroundBodyDef.position.Set(0,0);
	GroundBody = World->CreateBody(&GroundBodyDef);
	GroundBody->SetUserData("Ground");
	for(int y=0;y<14;y++)
	{
		for(int x=0;x<Width;x++)
		{
			if(MapData[y][x] == Mapchip_Blank)continue;
			GroundBox.SetAsBox(16/Box_Rate,16/Box_Rate,b2Vec2((x*32+16)/Box_Rate,(y*32+16)/Box_Rate),0);
			GroundBody->CreateFixture(&GroundBox,0.f);
		}
	}
	//GroundBody->CreateFixture(&GroundBox,0.f);
}

void Map::Draw(void)
{
	for(int y=0;y<14;y++)
	{
		for(int x=0;x<Width;x++)
		{
			if(MapData[y][x] == Mapchip_Blank)continue;
			int Graph = -1;
			if(MapData[y][x] == Mapchip_Clay)Graph = MapChips[1];
			if(MapData[y][x] == Mapchip_ClayFloor)Graph = MapChips[2];
			DrawGraph(x*32,y*32,Graph,true);
		}
	}
}