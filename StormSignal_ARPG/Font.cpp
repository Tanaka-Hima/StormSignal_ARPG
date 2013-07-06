#include "Font.h"
#include "DxLib.h"

//画像を使用して文字を描画する
void Font::DrawString_UseImage(int x,int y,int Space,float ext,const char* String,int FontIndex)
{
	int StrLen = strlen(String);
	int Img = 0;
	int x_Pos = x;
	int y_Pos = y;
	int Width = 0;
	int Height = 0;
	int ImgWidth = 0;
	int AverageSpace = Space;
	for(int i=0;i<StrLen;i++)
	{
		char Alphabet = String[i];
		//改行判定
		if(Alphabet == '\\')
		{
			x_Pos = ImgWidth;
			x_Pos = x;
			y_Pos += Height + Space;
			continue;
		}
		//指定文字の画像を読み込む
		if(String[i] == ' ')goto Blank;
		
		Img = FontData[FontIndex][Alphabet-'A'];

		//描画
		if(Img)
		{
			GetGraphSize(Img,&Width,&Height);
			Width = (int)(Width * ext);
			Height = (int)(Height * ext);
			DrawRotaGraph2(x_Pos,y_Pos,0,0,ext,0,Img,true);
			//次の描画位置を指定
			x_Pos += Width + Space;
		}else
		{//ブランク挿入
			Blank:;
			x_Pos += AverageSpace;
		}
		AverageSpace = (AverageSpace + Width) / 2;
	}
}

int Font::CalculateWidth(int x,int y,int Space,float ext,const char* String,int FontIndex)
{
	int StrLen = strlen(String);
	int Img = 0;
	int x_Pos = x;
	int y_Pos = y;
	int Width = 0;
	int Height = 0;
	int ImgWidth = 0;
	int AverageSpace = Space;
	for(int i=0;i<StrLen;i++)
	{
		char Alphabet = String[i];
		//改行判定
		if(Alphabet == '\\')
		{
			x_Pos = ImgWidth;
			x_Pos = x;
			y_Pos += Height + Space;
			continue;
		}
		//指定文字の画像を読み込む
		if(String[i] == ' ')goto Blank;
		
		Img = FontData[FontIndex][Alphabet-'A'];

		//描画
		if(Img)
		{
			GetGraphSize(Img,&Width,&Height);
			Width = (int)(Width * ext);
			Height = (int)(Height * ext);
			//DrawRotaGraph2(x_Pos,y_Pos,0,0,ext,0,Img,true);
			//次の描画位置を指定
			x_Pos += Width + Space;
		}else
		{//ブランク挿入
			Blank:;
			x_Pos += AverageSpace;
		}
		AverageSpace = (AverageSpace + Width) / 2;
	}
	if(ImgWidth < x_Pos)ImgWidth = x_Pos;
	return ImgWidth;
}

Font::Font()
{
	
}

void Font::DrawString(int x,int y,int Space,float ext,string String,string Pass,int Position,float Alpha_Speed)
{
	bool isLoaded = false;
	
	int Length = FontIndexs.size();
	int i=0;
	for(i=0;i<Length;i++)
	{
		if(FontIndexs[i] == Pass)
		{
			isLoaded = true;
			break;
		}
	}

	if(!isLoaded)
	{//フォントデータの読み込み
		vector<int> Images;
		for(char Alphabet='A';Alphabet<='Z';Alphabet++)
		{
			char Str[7];
			sprintf_s(Str,7,"/%c.png",Alphabet);
			Images.push_back(LoadGraph((Pass + Str).c_str()));
		}
		FontData.push_back(Images);
		FontIndexs.push_back(Pass);
	}

	int Width = CalculateWidth(0,0,Space,ext,String.c_str(),i);
	DrawString_UseImage(x-(int)(Width * (Position / 2.0)),y,Space,ext,String.c_str(),i);
	
	return;
}