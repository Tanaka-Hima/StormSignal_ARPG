#include <vector>
#include <string>
using namespace std;

#ifndef _IMAGE_2D
#define _IMAGE_2D

class Image_2D
{//2D‰æ‘œƒNƒ‰ƒX
public:
	vector<int> Graph;
	int Anime_Speed;
	int Anime_ShowNum;
	int Anime_Time;
	int Time;
	vector<int> SIHandle;
	bool Visible;
	int x;
	int y;
	int Center_x;
	int Center_y;
	float Ext;
	float Angle;
	void Load(string Pass);
	void Unload(void);
	virtual void Initialize(void);
	virtual bool Draw(bool Trans);
	virtual int GetAlpha(int X,int Y);
	virtual bool HitTestPoint(int Target_X,int Target_Y,bool Shape);
	virtual int HitTestGraph(Image_2D *Target,bool Shape);
};

#endif