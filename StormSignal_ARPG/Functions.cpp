#include "DxLib.h"
#include "Functions.h"
#include "ConstantValue.h"

void ShowFPS()
{//FPS値表示
	static int Count = -1;
	static double Show_Count = 0;
	static int Second = 0;
	int Now_Second;

	if(Count < 0)
	{
		Second = GetNowCount();
		Count = 0;
	}

	Count++;

	Now_Second = GetNowCount();
	if(Second + 1000 <= Now_Second)
	{
		Show_Count = Count * 1000. / (Now_Second - Second);
		Second = Now_Second;
		Count = 0;
	}
	DrawFormatString(0,0,Black,"FPS : %f",Show_Count);	
}

//処理初め(0)、処理終わり(1)
void ScreenTimer(int Type,int FrameRate)
{
	//タイマー
	static int Timer = 0;
	static double Wait = 0.0;
	//フレームスキップフラグ
	static int FrameSkip = 0;

	if(!Type)
	{
		Timer = GetNowCount();
		ClearDrawScreen();
	}else
	{
		#ifdef _DEBUG
		ShowFPS();
		#endif
		FrameSkip = 0;
		if(!FrameSkip)ScreenFlip() ;
		Wait += (1000.0 / FrameRate - (GetNowCount() - Timer));
		if(Wait > 0)
		{
			Sleep((DWORD)Wait);
			Wait -= (int)Wait;
		}else
		{
			FrameSkip = 1;
		}
	}
}

//キーが押されたかどうかを確かめる
bool CheckKeyDown(int KeyCode)
{
	static char OldKeyBuf[256];
	char KeyBuf[256];
	if(OldKeyBuf[KeyCode] == 0)
	{
		GetHitKeyStateAll(KeyBuf);
		if(KeyBuf[KeyCode] == 1)
		{
			OldKeyBuf[KeyCode] = 1;
			return true;
		}else
		{
			OldKeyBuf[KeyCode] = 0;
			return false;
		}
	}else
	{
		GetHitKeyStateAll(KeyBuf);
		if(KeyBuf[KeyCode] == 0)
		{
			OldKeyBuf[KeyCode] = 0;
		}
		return false;
	}
}

//文字列を文字ごとに区切る
vector<string> split(string str, string delim)
{
    vector<string> result;
    int cutAt;
    while( (cutAt = str.find_first_of(delim)) != str.npos )
    {
        if(cutAt > 0)
        {
            result.push_back(str.substr(0, cutAt));
        }
        str = str.substr(cutAt + 1);
    }
    if(str.length() > 0)
    {
        result.push_back(str);
    }
return result;
}

//ファイルリストの取得
vector<string> GetFolderList(string folder)
{
	// 宣言
	vector<string> fileList;
	HANDLE hFind;
	WIN32_FIND_DATA fd;

	// ファイル名検索のためにワイルドカード追加
	// 例 : "D:\\Users\\Pictures\\*.*"
	stringstream ss;
	ss << folder;
	string::iterator itr = folder.end();
	itr--;
	if(*itr != '\\') ss << '\\';
	ss << "*.*";

	// ファイル探索
	// FindFirstFile(ファイル名, &fd);
	hFind = FindFirstFile(ss.str().c_str(), &fd);

	// 検索失敗
	if(hFind == INVALID_HANDLE_VALUE){
		std::cout << "ファイル一覧を取得できませんでした" << std::endl;
		exit(1); // エラー終了
	}

	// ファイル名をリストに格納するためのループ
	do{
		// フォルダは除く
		if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		  && !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
		{
			//ファイル名をリストに格納
			char *file = fd.cFileName;
			string str = file;
			fileList.push_back(str);
		}
	}while(FindNextFile(hFind, &fd)); //次のファイルを探索
	
	// hFindのクローズ
	FindClose(hFind); 

	return fileList;
}

/**
 * 文字列中から文字列を検索して別の文字列に置換する
 * @param str  : 置換対象の文字列。上書かれます。
 * @param from : 検索文字列
 * @param to   : 置換後の文字列
 */
void strReplace (std::string& str, const std::string& from, const std::string& to) {
    std::string::size_type pos = 0;
    while(pos = str.find(from, pos), pos != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
    }
}
