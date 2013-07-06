#include <Windows.h>
#include <mmsystem.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#ifndef _FUNCTIONS
#define _FUNCTIONS

//処理初め(0)、処理終わり(1)
void ScreenTimer(int Type,int FrameRate);

//キーが押されたかどうかを確かめる
bool CheckKeyDown(int KeyCode);

//文字列を分割する
vector<string> split(string str, string delim);

template <class T> string ntos(T Num)
{
	ostringstream Temp;
	Temp << Num;
	return Temp.str();
}

//A点とB点の距離を求める
template <class T> double inline GetDistance(T A_X,T A_Y,T B_X,T B_Y)
{
	return hypot(A_X - B_X,A_Y - B_Y);
}

int SetScoreFile(int Score,string Name,string ScoreFileName);

//スコアファイルの内容を返す
vector<string> GetScoreFile(string ScoreFileName);

//ファイルリストの取得
vector<string> GetFolderList(string folder);

/**
 * 文字列中から文字列を検索して別の文字列に置換する
 * @param str  : 置換対象の文字列。上書かれます。
 * @param from : 検索文字列
 * @param to   : 置換後の文字列
 */
void strReplace (std::string& str, const std::string& from, const std::string& to);

#endif