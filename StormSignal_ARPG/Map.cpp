#include <string>
#include <cwctype>
#include <DxLib.h>
#include <Box2D\Box2D.h>
#include "Map.h"
#include "Functions.h"
#include "ConstantValue.h"

void Map::Initialize(b2World *World)
{
	PauseFlag = false;
	MapChips.clear();

	Image_2D TempGraph;

	//空欄 0
	MapChips.push_back(TempGraph);

	//土 1
	MapChips.push_back(TempGraph);
	MapChips[MapChips.size()-1].Load("Image/Map/Clay.png");

	//土上 2
	MapChips.push_back(TempGraph);
	MapChips[MapChips.size()-1].Load("Image/Map/ClayFloor.png");

	//木箱 3
	MapChips.push_back(TempGraph);
	MapChips[MapChips.size()-1].Load("Image/Map/Woodbox.png");

	//スイッチ1(off) 4
	MapChips.push_back(TempGraph);
	MapChips[MapChips.size()-1].Load("Image/Map/Switch1.png");

	//スイッチ2(on) 5
	MapChips.push_back(TempGraph);
	MapChips[MapChips.size()-1].Load("Image/Map/Switch2.png");

	//看板 6
	MapChips.push_back(TempGraph);
	MapChips[MapChips.size()-1].Load("Image/Map/Board.png");


	PlayerData.Load("Image/Chara/None.png");
	PlayerData.Initialize(World,Mapchip_Player,1,1,100);

}

void Map::LoadMapData(string Pass)
{
	int LineData = FileRead_open(Pass.c_str());
	//マップの横幅を計算する
	Width = FileRead_size(Pass.c_str())/14+2;
	for(int i=0;i<14;i++)
	{
		char *Data;
		Data = (char*)malloc(Width);
		FileRead_gets(Data,Width,LineData);
		MapData[i] = split(Data,",");
		free(Data);

	}
	Width = MapData[0].size();
}

void Map::LoadScriptData(string Pass)
{
	int LineData = FileRead_open(Pass.c_str());
	while(FileRead_eof(LineData) == 0)
	{
		char Data[256];
		FileRead_gets(Data,256,LineData);
		ScriptData.push_back(split(Data,","));
	}
}

void Map::CreateMap(b2World *World)
{
	//当たり判定生成準備
	GroundBodyDef.position.Set(0,0);
	GroundBody = World->CreateBody(&GroundBodyDef);
	GroundBody->SetUserData("Ground");
	for(int y=0;y<14;y++)
	{
		for(int x=0;x<Width;x++)
		{
			//マップデータとマップチップを照らし合わせ、一致したものを設置していく
			if(MapData[y][x] == Mapchip_Blank)continue;
			else if(MapData[y][x] == Mapchip_Player)
			{
				PlayerData.GetBody()->SetTransform(b2Vec2((x*32+16)/Box_Rate,(y*32+16)/Box_Rate),0);
				continue;
			}else if(MapData[y][x] == Mapchip_Woodbox)
			{
				Object Body;
				RigidBodies.push_back(Body);
				RigidBodies[RigidBodies.size()-1].Graph = MapChips[3].Graph;
				RigidBodies[RigidBodies.size()-1].SIHandle = MapChips[3].SIHandle;
				RigidBodies[RigidBodies.size()-1].Initialize(World,"Mapchip_Woodbox",1,1,-1);
				RigidBodies[RigidBodies.size()-1].GetBody()->SetTransform(b2Vec2((x*32+16)/Box_Rate,(y*32+16)/Box_Rate),0);
				continue;
			}else if(MapData[y][x] == Mapchip_TrainingBag
				||	MapData[y][x] == Mapchip_CommonEnemy1)
			{
				Enemy EnemyTemp;
				EnemyData.push_back(EnemyTemp);
				EnemyData[EnemyData.size()-1].Initialize(World,MapData[y][x],1,1,100);
				EnemyData[EnemyData.size()-1].GetBody()->SetTransform(b2Vec2((x*32+16)/Box_Rate,(y*32+16)/Box_Rate),0);
				continue;
			}

			//特殊ブロック
			if(isalpha(static_cast<unsigned char>(MapData[y][x][0])))
			{
				int Length = ScriptData.size();
				int Flag = -1;
				for(int i=0;i<Length;i++)
				{
					if(ScriptData[i][0] == MapData[y][x])
					{
						if(ScriptData[i][2] == "1")
						{
							Flag = i;
							break;
						}
					}
				}
				if(Flag >= 0)
				{
					//当たり判定の生成
					GroundBox.SetAsBox(16/Box_Rate,16/Box_Rate,b2Vec2((x*32+16)/Box_Rate,(y*32+16)/Box_Rate),0);
					MapChipFixtures.push_back(GroundBody->CreateFixture(&GroundBox,0.f));
					string Str = ScriptData[Flag][0] + "," + ntos(MapChipFixtures.size()-1);
					FixtureDataToMapChip.push_back(Str);
				}
				continue;
			}

			//当たり判定の生成
			GroundBox.SetAsBox(16/Box_Rate,16/Box_Rate,b2Vec2((x*32+16)/Box_Rate,(y*32+16)/Box_Rate),0);
			GroundBody->CreateFixture(&GroundBox,0.f);
		}
	}
	//GroundBody->CreateFixture(&GroundBox,0.f);
}

bool Map::GetPauseFlag()
{
	return PauseFlag;
}

void Map::Step()
{
	if(CheckKeyDown(KEY_INPUT_P))PauseFlag = 1 - PauseFlag;
	if(PauseFlag)return;

	//プレイヤーの処理
	PlayerData.Ctrl();
	PlayerData.Step();

	//敵の処理
	int Length = EnemyData.size();
	for(int i=0;i<Length;i++)
	{
		EnemyData[i].Ctrl();
		EnemyData[i].Step();

		if(EnemyData[i].HP < 0)
		{
			EnemyData.erase(EnemyData.begin()+i);
			i--;
			Length--;
		}
	}

	/*
		マップのスクロール
		左端、右端にプレイヤーが位置していた場合、マップの方を移動させる
	*/
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

	//特殊ブロックの処理
	for(int y=0;y<14;y++)
	{
		for(int x=0;x<Width;x++)
		{
			//特殊ブロック
			if(isalpha(static_cast<unsigned char>(MapData[y][x][0])))
			{
				int Length = ScriptData.size();
				for(int i=0;i<Length;i++)
				{
					if(ScriptData[i][0] == MapData[y][x])
					{
						int Flag = false;
						//トリガー
						if(ScriptData[i][3].find(Trigger_Hit) != string::npos)
						{//対象に触れた瞬間のみ起動
							if(PlayerData.HitTestRect(MapTrans.p.x*Box_Rate+x*32,y*32,32,32,true))
							{
								if(ScriptData[i][3] != Trigger_Hitted)
								{
									Flag = true;
									ScriptData[i][3] = Trigger_Hitted;
								}
							}else
							{
								ScriptData[i][3] = Trigger_Hit;
							}
						}else if(ScriptData[i][3] == Trigger_Touch)
						{//対象に触れている間常に起動
							if(PlayerData.HitTestRect(MapTrans.p.x*Box_Rate+x*32,y*32,32,32,true))
							{
								Flag = true;
							}
						}else if(ScriptData[i][3] == Trigger_Use)
						{//対象の近くで↑キーを押すと起動
							int Dis = GetDistance(PlayerData.x,PlayerData.y,(int)(MapTrans.p.x*Box_Rate+x*32+16),y*32+16);
							if(GetDistance(PlayerData.x,PlayerData.y,(int)(MapTrans.p.x*Box_Rate+x*32+16),y*32+16) < 50
								&&  CheckHitKey(KEY_INPUT_UP))
							{
								Flag = true;
							}
						}else if(ScriptData[i][3] == Trigger_Flagged)
						{
							Flag = true;
							ScriptData[i][3] == Trigger_Flag;
						}

						int Count = atoi(ScriptData[i][5].c_str());
						if(Flag && (Count > 0 || Count < 0))
						{
							Count--;
							ScriptData[i][5] = ntos(Count);
							if(ScriptData[i][4].find(Action_Flag) != string::npos)
							{//Targetのフラグを立てる
								vector<string> Data = split(ScriptData[i][4],"|");
								for(int j=0;j<Length;j++)
								{
									if(ScriptData[j][0] == Data[1] && ScriptData[j][3] == Trigger_Flag)
									{
										ScriptData[j][3] = Trigger_Flagged;
									}
								}
							}else if(ScriptData[i][4].find(Action_Redraw) != string::npos)
							{//自分の見た目をNumberへ変更する
								vector<string> Data = split(ScriptData[i][4],"|");
								ScriptData[i][1] = Data[1];
							}else if(ScriptData[i][4] == Action_Delete)
							{//自分をマップから削除する
								int FixLength = FixtureDataToMapChip.size();
								for(int j=0;j<FixLength;j++)
								{
									vector<string> Data = split(FixtureDataToMapChip[j],",");
									if(Data[0] == MapData[y][x])
									{//当たり判定を削除
										GroundBody->DestroyFixture(MapChipFixtures[atoi(Data[1].c_str())]);
									}
								}
								MapData[y][x] = Mapchip_Blank;
							}
						}
					}
				}
			}
		}
	}
}

void Map::Draw()
{
	//マップのスクロール分を取得
	b2Transform MapTrans = GroundBody->GetTransform();
	for(int y=0;y<14;y++)
	{
		for(int x=0;x<Width;x++)
		{
			string GraphNum = MapData[y][x];

			//特殊ブロック
			if(isalpha(static_cast<unsigned char>(MapData[y][x][0])))
			{
				int Length = ScriptData.size();
				for(int i=0;i<Length;i++)
				{
					if(ScriptData[i][0] == MapData[y][x])
					{
						GraphNum = ScriptData[i][1];
						break;
					}
				}
			}

			if(GraphNum == Mapchip_Blank)continue;
			int Graph = -1;
			if(GraphNum == Mapchip_Clay)Graph = MapChips[1].Graph[0];
			if(GraphNum == Mapchip_ClayFloor)Graph = MapChips[2].Graph[0];
			if(GraphNum == Mapchip_Woodbox)continue;
			if(GraphNum == Mapchip_Switch1)Graph = MapChips[4].Graph[0];
			if(GraphNum == Mapchip_Switch2)Graph = MapChips[5].Graph[0];
			if(GraphNum == Mapchip_Board)Graph = MapChips[6].Graph[0];

			DrawGraph(MapTrans.p.x*Box_Rate+x*32,y*32,Graph,true);
		}
	}

	//プレイヤーの描画
	PlayerData.Draw(true);

	//敵の描画
	int Length = EnemyData.size();
	for(int i=0;i<Length;i++)
	{
		EnemyData[i].Draw();
	}

	//オブジェクトの描画
	Length = RigidBodies.size();
	for(int i=0;i<Length;i++)
	{
		RigidBodies[i].Draw();
	}

	//スキル設定ウインドウの処理
	PlayerData.StepSkillWindow();
}