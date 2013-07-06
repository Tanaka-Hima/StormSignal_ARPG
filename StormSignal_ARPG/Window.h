#include <string>
#include <DxLib.h>
using namespace std;

#ifndef _WINDOW
#define _WINDOW

class Window
{
private:
	int Screen;
	int Width,Height;
	int EdgeColor,PanelColor;
	bool Inited;
public:
	double x,y;
	double Ext;
	double Angle;
	bool Visible;
	void DeleteScreen(void);
	void Initialize(double Input_x,double Input_y,int Input_Width,int Input_Height,int Input_EdgeColor,int Input_PanelColor);
	void ReWindow(void);
	void Draw(void);
	void DrawStringInWindow(int Input_x,int Input_y,int Input_Pos,string Input_String,int FontData,int Color);
	void SetDrawThisWindow(void);
	int GetWidth(void);
	int GetHeight(void);
};

#endif