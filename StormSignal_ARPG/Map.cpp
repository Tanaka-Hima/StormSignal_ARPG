#include <string>
#include <DxLib.h>
#include <Box2D\Box2D.h>
#include "Map.h"
#include "Functions.h"
#include "ConstantValue.h"

void Map::Initialize(b2World *World)
{
	MapChips.clear();
	//空欄 0
	MapChips.push_back(-1);

	//土 1
	MapChips.push_back(LoadGraph("Image/Map/Clay.png"));

	//土上 2
	MapChips.push_back(LoadGraph("Image/Map/ClayFloor.png"));

	//木箱 3
	MapChips.push_back(LoadGraph("Image/Map/Woodbox.png"));

	//スイッチ1(off) 4
	MapChips.push_back(LoadGraph("Image/Map/Switch1.png"));

	//スイッチ2(on) 5
	MapChips.push_back(LoadGraph("Image/Map/Switch2.png"));


	PlayerData.Load("Image/Chara/None.png");
	PlayerData.Initialize(World,"Player",1,1,100);

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

		//改行コード、null文字の削除
		MapData[i] = split(Data,",");
	}
	Width = MapData[0].size();
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
			else if(MapData[y][x] == Mapchip_Player)
			{
				PlayerData.GetBody()->SetTransform(b2Vec2((x*32+16)/Box_Rate,(y*32+16)/Box_Rate),0);
				continue;
			}else if(MapData[y][x] == Mapchip_Woodbox)
			{
				Object Body;
				RigidBodies.push_back(Body);
				RigidBodies[RigidBodies.size()-1].Graph.push_back(MapChips[3]);
				RigidBodies[RigidBodies.size()-1].Initialize(World,"Mapchip_Woodbox",1,1,-1);
				RigidBodies[RigidBodies.size()-1].GetBody()->SetTransform(b2Vec2((x*32+16)/Box_Rate,(y*32+16)/Box_Rate),0);
				continue;
			}else if(MapData[y][x] == Mapchip_TrainingBag)
			{
				Enemy EnemyTemp;
				EnemyData.push_back(EnemyTemp);
				EnemyData[EnemyData.size()-1].Load("Font/Big_Green/A.png");
				EnemyData[EnemyData.size()-1].Initialize(World,"Mapchip_TrainingBag",1,1,100);
				EnemyData[EnemyData.size()-1].GetBody()->SetTransform(b2Vec2((x*32+16)/Box_Rate,(y*32+16)/Box_Rate),0);
				continue;
			}
			GroundBox.SetAsBox(16/Box_Rate,16/Box_Rate,b2Vec2((x*32+16)/Box_Rate,(y*32+16)/Box_Rate),0);
			GroundBody->CreateFixture(&GroundBox,0.f);
		}
	}
	//GroundBody->CreateFixture(&GroundBox,0.f);
}

void Map::Step()
{
	PlayerData.Ctrl();
	PlayerData.Step();

	int Length = EnemyData.size();
	for(int i=0;i<Length;i++)
	{
		EnemyData[i].Step();
	}

	//マップのスクロール
	b2Transform PlayerTrans = PlayerData.GetBody()->GetTransform();
	b2Transform MapTrans = GroundBody->GetTransform();

	if(PlayerTrans.p.x < Screen_Width*(3.0/7.0)/Box_Rate)
	{
		float ScrollDistance = Screen_Width*(3.0/7.0)/Box_Rate - PlayerTrans.p.x;
		MapTrans.p.x += ScrollDistance;
		PlayerTrans.p.x += ScrollDistance;

		int Length = PlayerData.CharacterList.size();
		for(int i=0;i<Length;i++)
		{
			b2Transform Trans = PlayerData.CharacterList[i]->GetBody()->GetTransform();
			Trans.p.x += ScrollDistance;
			PlayerData.CharacterList[i]->GetBody()->SetTransform(Trans.p,Trans.q.GetAngle());
		}

		GroundBody->SetTransform(MapTrans.p,MapTrans.q.GetAngle());
	}else if(PlayerTrans.p.x > Screen_Width*(4.0/7.0)/Box_Rate)
	{
		float ScrollDistance = PlayerTrans.p.x - Screen_Width*(4.0/7.0)/Box_Rate;
		MapTrans.p.x -= ScrollDistance;

		int Length = PlayerData.CharacterList.size();
		for(int i=0;i<Length;i++)
		{
			b2Transform Trans = PlayerData.CharacterList[i]->GetBody()->GetTransform();
			Trans.p.x -= ScrollDistance;
			PlayerData.CharacterList[i]->GetBody()->SetTransform(Trans.p,Trans.q.GetAngle());
		}

		GroundBody->SetTransform(MapTrans.p,MapTrans.q.GetAngle());
	}
}

void Map::Draw()
{
	b2Transform MapTrans = GroundBody->GetTransform();
	for(int y=0;y<14;y++)
	{
		for(int x=0;x<Width;x++)
		{
			if(MapData[y][x] == Mapchip_Blank)continue;
			int Graph = -1;
			if(MapData[y][x] == Mapchip_Clay)Graph = MapChips[1];
			if(MapData[y][x] == Mapchip_ClayFloor)Graph = MapChips[2];
			if(MapData[y][x] == Mapchip_Woodbox)continue;
			if(MapData[y][x] == Mapchip_Switch1)Graph = MapChips[4];
			if(MapData[y][x] == Mapchip_Switch2)Graph = MapChips[5];
			DrawGraph(MapTrans.p.x*Box_Rate+x*32,y*32,Graph,true);
		}
	}

	PlayerData.Draw(true);

	int Length = EnemyData.size();
	for(int i=0;i<Length;i++)
	{
		EnemyData[i].Draw();
	}

	Length = RigidBodies.size();
	for(int i=0;i<Length;i++)
	{
		RigidBodies[i].Draw();
	}

	PlayerData.StepSkillWindow();
}