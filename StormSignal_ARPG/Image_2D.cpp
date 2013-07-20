#include "Image_2D.h"
#include "DxLib.h"

struct Vect
{
	int x;
	int y;
};

void Image_2D::Load(string Pass)
{
	Graph.push_back(LoadGraph(Pass.c_str()));
	SIHandle.push_back(LoadSoftImage(Pass.c_str()));
}

void Image_2D::Unload()
{
	int Length = Graph.size();
	for(int i=0;i<Length;i++)
	{
		DeleteGraph(Graph[i]);
		DeleteSoftImage(SIHandle[i]);
	}
	Graph.clear();
	SIHandle.clear();
}

void Image_2D::Initialize()
{//初期化
	int SizeW,SizeH;
	//while(CheckHandleASyncLoad(Graph[0]) && ProcessMessage() == 0){}
	GetGraphSize(Graph[0],&SizeW,&SizeH);
	Visible = true;
	x = 0;
	y = 0;
	Center_x = SizeW / 2;
	Center_y = SizeH / 2;
	Ext = 1.0f;
	Angle = 0.0f;
	
	Anime_Speed = 16;
	Anime_Time = 0;
	Anime_ShowNum = 0;

	Time = GetNowCount();
}

bool Image_2D::Draw(bool Trans)
{//描画
	if(Graph.size() > 1)
	{
		int NowTime = GetNowCount();
		Anime_Time += NowTime - Time;
		Time = NowTime;
	}
	if(Anime_Speed <= 0)Anime_Speed = 1;

	if(Visible)DrawRotaGraph2(x,y,Center_x,Center_y,Ext,Angle,Graph[Anime_ShowNum],Trans);

	if(Anime_Time >= Anime_Speed)
	{
		Anime_ShowNum++;
		Anime_Time = 0;
	}
	if(Anime_ShowNum >= (int)Graph.size())
	{
		Anime_ShowNum = 0;
		return true;
	}
	return false;
}

int Image_2D::GetAlpha(int X,int Y)
{//アルファ値を取得
	int R,G,B,A;
	GetPixelSoftImage(SIHandle[Anime_ShowNum],X - (x - Center_x),Y - (y - Center_y),&R,&G,&B,&A);
	return A;
}

bool Image_2D::HitTestPoint(int Target_X,int Target_Y,bool Shape)
{//点との当たり判定
	int R,G,B,A;
	if(x - Center_x <= Target_X && Target_X <= Center_x + x && y - Center_y <= Target_Y && Target_Y <= Center_y + y)
	{
		if(!Shape)return true;
		else
		{//画像見た目通りの判定
			GetPixelSoftImage(SIHandle[Anime_ShowNum],Target_X - (x - Center_x),Target_Y - (y - Center_y),&R,&G,&B,&A);
			if(!A)return false;
			else return true;
		}
	}
	else return false;
}

int Image_2D::HitTestGraph(Image_2D *Target,bool Shape)
{//画像との当たり判定
	Vect A1,A2,B1,B2;
	A1.x = x - Center_x;
	A1.y = y - Center_y;
	A2.x = x + Center_x;
	A2.y = y + Center_y;
	B1.x = Target->x - Target->Center_x;
	B1.y = Target->y - Target->Center_y;
	B2.x = Target->x + Target->Center_x;
	B2.y = Target->y + Target->Center_y;

	if(B1.x < A2.x && A2.y > B1.y && B2.x > A1.x && B2.y > A1.y)
	{//大雑把な当たり判定
		if(!Shape)
		{
			return true;
		}else
		{//見た目通りの当たり判定
			int X,Y,Width,Height;
			if(B1.x > A1.x)X = B1.x;
			else X = A1.x;
			if(B1.y > A1.y)Y = B1.y;
			else Y = A1.y;
			if(A2.x < B2.x)Width = A2.x;
			else Width = B2.x;
			if(B2.y > A2.y)Height = A2.y;
			else Height = B2.y;

			for(int i=X;i<Width;i++)
			{
				for(int i2=Y;i2<Height;i2++)
				{
					if(Target->GetAlpha(i,i2) <= 127)continue;
					if(GetAlpha(i,i2) <= 127)continue;
					return true;
				}
			}
			return false;
		}
	}else
	{//かすりもしない場合
		return false;
	}
}
