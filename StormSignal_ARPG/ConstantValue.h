#include <DxLib.h>

#ifndef _CONST
#define _CONST

//画面設定
const int RefreshRate = 60;
const int Screen_Width = 960;
const int Screen_Height = 540;

//色
const int White = GetColor(255,255,255);
const int Black = GetColor(0,0,0);
const int LightBlack = GetColor(128,128,128);
const int Red = GetColor(255,0,0);
const int LightRed = GetColor(255,128,128);
const int Green = GetColor(0,255,0);
const int LightGreen = GetColor(128,255,128);
const int Blue = GetColor(0,0,255);
const int LightBlue = GetColor(128,128,255);
const int Pink = GetColor(255,0,255);
const int LightPink = GetColor(255,128,255);
const int Yellow = GetColor(255,217,0);
const int LightYellow = GetColor(255,217,128);

//Font関連
const int DrawString_Left = 0;//文字を描画する位置
const int DrawString_Center = 1;
const int DrawString_Right = 2;

//Scene
const int Title = 0;
const int ModeSelect = 1;
const int StoryMode = 2;
const int ArcadeMode = 3;
const int TrainingMode = 4;
const int OptionMode = 5;

//BOX2D関連
const bool doSleep = true;//スリープを有効にするか否か
const float Box_Rate = 10;//Box2Dの1 = DxlibのBox_Rate Pixel

#endif