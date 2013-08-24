#include <vector>
#include <string>
using namespace std;

#ifndef _IMAGE_2D
#define _IMAGE_2D

enum ImageDirection
{
	ImageDirection_Left = -1,
	ImageDirection_Right = 1,
};

class Image_2D
{//2D画像クラス
public:
	vector<int> Graph;//画像
	int Anime_Speed;//アニメーションのスピード
	int Anime_ShowNum;
	int Anime_Time;
	int Time;
	vector<int> SIHandle;
	bool Visible;//画像を描画するかどうか
	int x;
	int y;
	int Center_x;
	int Center_y;
	ImageDirection Direction;
	float Ext;
	float Ext_x;
	float Ext_y;
	float Angle;
	void Load(string Pass);//指定パスから画像を取得
	void Load(vector<int> Graphs);//配列から画像を取得
	void Unload();//画像のアンロード
	virtual void Initialize();//初期化
	virtual bool Draw(bool Trans);//描画
	virtual int GetAlpha(int X,int Y);//指定座標のアルファ値を取得
	virtual bool HitTestPoint(int Target_X,int Target_Y,bool Shape);//点との当たり判定
	virtual int HitTestGraph(Image_2D *Target,bool Shape);//画像との当たり判定
	virtual bool HitTestRect(int RX,int RY,int RWidth,int RHeight,bool Shape);//矩形との当たり判定
};

#endif