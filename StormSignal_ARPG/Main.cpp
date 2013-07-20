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

	//Alt、F10等のポーズを無効化
	SetSysCommandOffFlag(true);

	//非アクティブ時でも処理を行う
	SetAlwaysRunFlag(true);

	//BOX2D
	b2Vec2 Gravity(0.f,9.8f);
	b2World World(Gravity);

	float32 TimeStep = 2.0f / (float)RefreshRate;
	int32 VelocityIterations = 6;
	int32 PositionIterations = 2;

	DebugDraw DebugDraw;
	DebugDraw.SetFlags(0xff);
	World.SetDebugDraw(&DebugDraw);

	Font Fonts;

	int Scene = TrainingMode;

	Map Map;
	Map.Initialize(&World);
	Map.LoadMapData("Map/Training.txt");
	Map.CreateMap(&World);

	//メインループ
	while( ProcessMessage() == 0)
	{
		ScreenTimer(0,RefreshRate);

		switch(Scene)
		{
			case Title:
			{
				#pragma region タイトル

				Fonts.DrawString(Screen_Width / 2,Screen_Height/5,5,1,"STORMSIGNAL ARPG","Font/Big_Red",DrawString_Center);

				Fonts.DrawString(Screen_Width / 2,Screen_Height*0.6,5,1,"PRESS ENTER KEY","Font/Big_Red",DrawString_Center);

				break;
				#pragma endregion
			}
			case ModeSelect:
			{
				#pragma region モードセレクト

				Fonts.DrawString(Screen_Width / 2,100,5,1,"STORMSIGNAL ARPG","Font/Big_Red",DrawString_Center);

				break;
				#pragma endregion
			}
			case StoryMode:
			{
				#pragma region ストーリーモード

				Fonts.DrawString(Screen_Width / 2,100,5,1,"STORMSIGNAL ARPG","Font/Big_Red",DrawString_Center);

				break;
				#pragma endregion
			}
			case ArcadeMode:
			{
				#pragma region アーケードモード

				Fonts.DrawString(Screen_Width / 2,100,5,1,"STORMSIGNAL ARPG","Font/Big_Red",DrawString_Center);

				break;
				#pragma endregion
			}
			case TrainingMode:
			{
				#pragma region トレーニングモード

				Fonts.DrawString(Screen_Width / 2,100,5,1,"TRAINING","Font/Big_Red",DrawString_Center);

				Map.Step();
				Map.Draw();

				World.Step(TimeStep, VelocityIterations, PositionIterations);
				World.DrawDebugData();

				break;
				#pragma endregion
			}
			case OptionMode:
			{
				#pragma region オプション

				Fonts.DrawString(Screen_Width / 2,100,5,1,"STORMSIGNAL ARPG","Font/Big_Red",DrawString_Center);

				break;
				#pragma endregion
			}

		}
		

		ScreenTimer(1,RefreshRate);
	}
	DxLib_End();
	return 0;
}