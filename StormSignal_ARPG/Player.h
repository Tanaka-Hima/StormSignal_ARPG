#include "Character.h"
#include "Window.h"
#include <DxLib.h>

#ifndef _PLAYER
#define _PLAYER

const float MoveSpeed = 5.0f;

class Player : public Character
{
private:
	int SkillSet[3][3][4];//x,y,{種類,レベル,経験値,CD}
	//vector<int*> SkillList;
	Window SkillWindow;//スキル表示用ウィンドウ
	vector<Window> SkillPanels;//スキル1つごとのパネル
	vector<Image_2D> SkillImages;//スキル説明時に表示する画像
	vector<Window> EquipmentPanels;//装備1つごとのパネル
	vector<Image_2D> EquipmentImages;//装備説明時に表示する画像
	Window InfoPanel,InfoPanel2;//スキル、装備説明文表示用パネル
	int Equipments[3];//装備中装備
	POINT SkillCursorPoint;//スキル選択時のカーソル座標
	POINT ChangeSkillPoint;//スキル変更時のカーソル座標
	bool SkillChangeFlag;//スキルを変更しているか否か
	vector<int> EnableSkillList[3];//使用可能なスキルリスト

	int FontSmall,FontMiddle,FontBig;//フォント
public:
	void Initialize(b2World *World,string CharaType,float Density,float Friction,int MaxHP);//初期化
	void Ctrl();//操作
	void StepSkillWindow();//スキルウィンドウの処理、描画
	bool GetSkillWindowVisible();//スキルウィンドウが有効かどうかを取得する
	void SetSkillWindowVisible(bool Visible);//スキルウィンドウの表示設定を変更する
	void InitAllSkillAvailableCount();//スキルのADをすべて初期化する
};

#endif