#include <DxLib.h>
#include "Functions.h"
#include "ConstantValue.h"
#include "Window.h"

void Window::DeleteScreen()
{
	DeleteGraph(Screen);
}

void Window::Initialize(double Input_x,double Input_y,int Input_Width,int Input_Height,int Input_EdgeColor,int Input_PanelColor)
{
	x = Input_x;
	y = Input_y;
	Ext = 1.0;
	Angle = 0.0;
	Width = Input_Width;
	Height = Input_Height;
	PanelColor = Input_PanelColor;
	EdgeColor = Input_EdgeColor;
	//DeleteGraph(Screen);
	Screen = MakeScreen(Width,Height,true);
	Visible = true;

	SetDrawScreen(Screen);
	DrawBox(0,0,Width,Height,GetColor(255,255,255),true);
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 128);
	DrawBox(0,0,Width,Height,PanelColor,true);
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 256);
	DrawBox(0,0,Width,Height,EdgeColor,false);
	SetDrawScreen(DX_SCREEN_BACK);
}

void Window::ReWindow()
{
	//SetDrawScreen(DX_SCREEN_BACK);
	SetDrawScreen(Screen);
	DrawBox(0,0,Width,Height,GetColor(255,255,255),true);
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 128);
	DrawBox(0,0,Width,Height,PanelColor,true);
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 256);
	DrawBox(0,0,Width,Height,EdgeColor,false);
	SetDrawScreen(DX_SCREEN_BACK);
}

void Window::Draw()
{
	if(!Visible)return;
	int Draw_x = (int)x;
	int Draw_y = (int)y;
	DrawRotaGraph2(Draw_x,Draw_y,0,0,Ext,Angle,Screen,true);
}

void Window::DrawStringInWindow(int Input_x,int Input_y,int Input_Pos,string Input_String,int FontData,int Color)
{
	vector<string> Strs = split(Input_String,"<>");
	int Height;
	GetFontStateToHandle(NULL,&Height,NULL,FontData);
	Height *= 1.4;
	int Length = Strs.size();
	int Draw_y = Input_y-Height;
	for(int i=0;i<Length;i++)
	{
		int Graph;
		int Draw_x = Input_x;
		Draw_y += Height;
		int DrawWidth = GetDrawStringWidthToHandle(Strs[i].c_str(),Strs[i].size(),FontData);
		int Size;
		float Ext = 1.0f;
		if(Input_Pos == DrawString_Center)Draw_x -= DrawWidth/2;
		else if(Input_Pos == DrawString_Right)Draw_x -= DrawWidth;
		GetFontStateToHandle(0,&Size,0,FontData);
		Graph = MakeScreen(DrawWidth,Size*2,true);
		SetDrawScreen(Graph);
		DrawStringToHandle(0,0,Strs[i].c_str(),Color,FontData);

		if(DrawWidth + Draw_x >= Width)
		{
			Ext = (float)Width / (float)(DrawWidth  + Draw_x) - 0.01f;
		}

		SetDrawScreen(Screen);
		DrawRotaGraph2(Draw_x,Draw_y,0,0,Ext,0.f,Graph,true);
		DeleteGraph(Graph);
	}
	SetDrawScreen(DX_SCREEN_BACK);
}

void Window::SetDrawThisWindow()
{
	SetDrawScreen(Screen);
}

int Window::GetWidth()
{
	return Width;
}

int Window::GetHeight()
{
	return Height;
}
