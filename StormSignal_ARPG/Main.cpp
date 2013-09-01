#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#include "Box2D/Box2D.h"
#include "DebugDraw.h"
#include "DxLib.h"
#include "Functions.h"
#include "ConstantValue.h"
#include "Font.h"
#include "Player.h"
#include "Enemy.h"
#include "Map.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						 LPSTR lpCmdLine, int nCmdShow )
{
	//ウィンドウモードを変更する
	ChangeWindowMode(true);
	SetDrawScreen(DX_SCREEN_BACK);

	//画面モード
	SetGraphMode(Screen_Width,Screen_Height,32);

	//背景色を白に設定
	SetBackgroundColor(255,255,255);

	//アンチエイリアス設定
	//SetFullSceneAntiAliasingMode(4,3);

	//タイトル
	SetMainWindowText("StormSignal ARPG");

	//垂直同期信号を待たない
	SetWaitVSyncFlag(false);

	if( DxLib_Init() == -1 )	// ＤＸライブラリ初期化処理
	{
		 return -1;	// エラーが起きたら直ちに終了
	}

	//マスク画面の作成
	CreateMaskScreen();

	//Alt、F10等のポーズを無効化
	SetSysCommandOffFlag(true);

	//非アクティブ時でも処理を行う
	SetAlwaysRunFlag(true);

	//BOX2D
	b2Vec2 Gravity(0.f,9.8f);
	b2World World(Gravity);

	int32 VelocityIterations = 6;
	int32 PositionIterations = 2;

	DebugDraw DebugDraw;
	DebugDraw.SetFlags(0xff);
	World.SetDebugDraw(&DebugDraw);

	Font Fonts;

	int Scene = Title;
	int Cursor = 0;
	int CursorAlpha = GetNowCount();

	//マップの生成
	Map Map;
	Map.Initialize(&World);

	//メインループ
	while( ProcessMessage() == 0)
	{
		ScreenTimer(0,RefreshRate);

		static int Time = GetNowCount();
		float32 TimeStep = (double)(GetNowCount() - Time) / 500.0;
		Time = GetNowCount();

		switch(Scene)
		{
			case Title:
			{
				#pragma region タイトル

				Fonts.DrawString(Screen_Width / 2,Screen_Height/5,5,1,"STORMSIGNAL ARPG","Font/Big_Red",DrawString_Center);

				SetDrawBlendMode( DX_BLENDMODE_ALPHA, fabs(sin((GetNowCount()-CursorAlpha)/300.f))*256);
				Fonts.DrawString(Screen_Width / 2,Screen_Height*0.6,5,1,"PRESS ENTER KEY","Font/Small_Red",DrawString_Center);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

				if(CheckKeyDown(KEY_INPUT_RETURN))
				{
					Scene = ModeSelect;
					Cursor = 0;
				}

				break;
				#pragma endregion
			}
			case ModeSelect:
			{
				#pragma region モードセレクト

				Fonts.DrawString(Screen_Width / 2,Screen_Height/5,5,1,"STORMSIGNAL ARPG","Font/Big_Red",DrawString_Center);

				if(Cursor == 0)SetDrawBlendMode( DX_BLENDMODE_ALPHA, fabs(sin((GetNowCount()-CursorAlpha)/300.f))*256);
				Fonts.DrawString(Screen_Width / 2,Screen_Height*0.5,5,1,"STORY MODE","Font/Big_Green",DrawString_Center);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

				if(Cursor == 1)SetDrawBlendMode( DX_BLENDMODE_ALPHA, fabs(sin((GetNowCount()-CursorAlpha)/300.f))*256);
				Fonts.DrawString(Screen_Width / 2,Screen_Height*0.6,5,1,"ARCADE MODE","Font/Big_Green",DrawString_Center);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

				if(Cursor == 2)SetDrawBlendMode( DX_BLENDMODE_ALPHA, fabs(sin((GetNowCount()-CursorAlpha)/300.f))*256);
				Fonts.DrawString(Screen_Width / 2,Screen_Height*0.7,5,1,"TRAINING MODE","Font/Big_Green",DrawString_Center);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

				if(Cursor == 3)SetDrawBlendMode( DX_BLENDMODE_ALPHA, fabs(sin((GetNowCount()-CursorAlpha)/300.f))*256);
				Fonts.DrawString(Screen_Width / 2,Screen_Height*0.8,5,1,"OPTION MODE","Font/Big_Green",DrawString_Center);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

				if(CheckKeyDown(KEY_INPUT_UP))Cursor--;
				if(CheckKeyDown(KEY_INPUT_DOWN))Cursor++;

				if(Cursor < 0)Cursor = 3;
				else if(Cursor > 3)Cursor = 0;

				if(CheckKeyDown(KEY_INPUT_RETURN))
				{
					Scene = Cursor + 2;
					Cursor = 0;
					if(Scene == ArcadeMode)
					{
						Map.LoadMapData("Map/AC_Stage1.txt");
						Map.LoadScriptData("Map/AC_Stage1_Script.txt");
						Map.CreateMap(&World);
					}else if(Scene == TrainingMode)
					{
						Map.LoadMapData("Map/Training.txt");
						Map.LoadScriptData("Map/Training_Script.txt");
						Map.CreateMap(&World);
					}
				}

				break;
				#pragma endregion
			}
			case StoryMode:
			{
				#pragma region ストーリーモード

				Fonts.DrawString(Screen_Width / 2,100,5,1,"STORY","Font/Big_Red",DrawString_Center);

				break;
				#pragma endregion
			}
			case ArcadeMode:
			{
				#pragma region アーケードモード

				string Name = Map.GetNextStageName();

				//プレイヤーのHPが0以下の場合、ステージをリスタートする
				if(Map.GetPlayerHP() <= 0)
				{
					Map.DestroyMap(&World);
					Map.Initialize(&World,false);
					Map.LoadMapData(Map.GetStagePass());
					Map.LoadScriptData(Map.GetScriptPass());
					Map.CreateMap(&World);
					Map.InitPlayerHP();
				}

				if(Name != "")
				{
					Map.DestroyMap(&World);
					Map.Initialize(&World,false);
					Map.LoadMapData("Map/"+Name+".txt");
					Map.LoadScriptData("Map/"+Name+"_Script.txt");
					Map.CreateMap(&World);

				}

				Map.Step();
				Map.Draw();

				if(!Map.GetPauseFlag() && !Map.GetMessageFlag())World.Step(TimeStep, VelocityIterations, PositionIterations);
				
				#ifdef _DEBUG
				World.DrawDebugData();
				#endif

				break;
				#pragma endregion
			}
			case TrainingMode:
			{
				#pragma region トレーニングモード

				//プレイヤーのHPが0以下の場合、ステージをリスタートする
				if(Map.GetPlayerHP() <= 0)
				{
					Map.DestroyMap(&World);
					Map.Initialize(&World,false);
					Map.LoadMapData(Map.GetStagePass());
					Map.LoadScriptData(Map.GetScriptPass());
					Map.CreateMap(&World);
					Map.InitPlayerHP();
				}

				Map.Step();
				Map.Draw();

				if(!Map.GetPauseFlag() && !Map.GetMessageFlag())World.Step(TimeStep, VelocityIterations, PositionIterations);
				
				#ifdef _DEBUG
				World.DrawDebugData();
				#endif

				break;
				#pragma endregion
			}
			case OptionMode:
			{
				#pragma region オプション

				Fonts.DrawString(Screen_Width / 2,100,5,1,"OPTION","Font/Big_Red",DrawString_Center);

				break;
				#pragma endregion
			}

		}
		

		ScreenTimer(1,RefreshRate);
	}
	DxLib_End();
	return 0;
}