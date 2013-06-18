#include "Includes.h"
#include "Font.h"

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

	//非アクティブ時でも処理を行う
	SetAlwaysRunFlag(true);

	Font Fonts;

	//メインループ
	while( ProcessMessage() == 0)
	{
		ScreenTimer(0,RefreshRate);

		Fonts.DrawString(Screen_Width / 2,100,5,1,"STORMSIGNAL ARPG","Font/Big_Red",DrawString_Center);

		ScreenTimer(1,RefreshRate);
	}
	DxLib_End();
	return 0;
}