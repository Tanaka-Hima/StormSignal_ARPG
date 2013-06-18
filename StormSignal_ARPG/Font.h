#include "Includes.h"

class Font
{
private:
	vector<string> FontIndexs;
	vector<vector<int>> FontData;

	//‰æ‘œ‚ðŽg—p‚µ‚Ä•¶Žš‚ð•`‰æ‚·‚é
	void DrawString_UseImage(int x,int y,int Space,float ext,const char* String,int FontIndex);
	int CalculateWidth(int x,int y,int Space,float ext,const char* String,int FontIndex);
public:
	Font();
	void DrawString(int x,int y,int Space,float ext,string String,string Pass,int Position,float Alpha_Speed = 0.0f);
};