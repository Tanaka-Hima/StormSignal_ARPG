#include "Physics2DImage.h"

#ifndef _CHARACTER
#define _CHARACTER

class HitBox;

const int Skill_None_None = 0;
const int Skill_Sword_Front = 1;
const int Skill_Sword_Shockwave = 2;
const int Skill_None_Stan = 3;
const int Skill_None_Frontstep = 4;
const int Skill_None_Backstep = 5;
const int Skill_Sword_Knockup = 6;
const int Skill_Handgun_Fire = 7;
const int Skill_Sword_Smash = 8;
const int Skill_Sword_Spin = 9;
const int Skill_Sword_StrikeTop = 10;

const string SkillValueNames[] = {"Skill_None_None",
								"Skill_Sword_Front",
								"Skill_Sword_Shockwave",
								"Skill_None_Damage",
								"Skill_None_Frontstep",
								"Skill_None_Backstep",
								"Skill_Sword_Knockup",
								"Skill_Handgun_Fire",
								"Skill_Sword_Smash",
								"Skill_Sword_Spin",
								"Skill_Sword_StrikeTop"};

const int SkillAvailableCount[] = {0,1,1,0,1,1,1,8,1,1,1};
const int SkillConsumeEX[] = {0,0,0,0,0,0,0,0,50,0,0};

const string SkillNames[] = {"無し",
							"前方へ剣を振り下ろす(仮)",
							"前方へ衝撃波(仮)",
							"ダメージ",
							"フロントステップ",
							"バックステップ",
							"打ち上げ",
							"発砲",
							"前方へ吹き飛ばし攻撃(仮)",
							"剣を回転させる(仮)",
							"上昇しつつ打ち上げる(仮)"};

const string SkillInfo[] = {"スキルを設定しません。<>必要装備 : 無し AD : 無し",
							"前方へ剣を振り下ろします。<>必要装備 : 剣 AD : 1",
							"前方へ衝撃波を発生させます。<>必要装備 : 剣 AD : 1",
							"ダメージを受けます。<>必要装備 : 無し AD : 1",
							"前方へ踏み込みを行ないます。<>必要装備 : AD : 1",
							"後方へ飛び退きます。<>必要装備 : 無し AD : 1",
							"敵を空中へ打ち上げます。<>必要装備 : 剣 AD : 1",
							"指定方向へ弾丸を発射します。<>必要装備 : HG AD : 8",
							"前方へ強力な攻撃を行ないます。<>必要装備 : 剣 AD : 1",
							"自分の周りへ攻撃を行います。<>必要装備 : 剣 AD : 1",
							"空中に飛び上がりつつ敵を打ち上げます。<>必要装備 : 剣 AD : 1"};


const int Equipment_None_None = 0;
const int Equipment_Sword_Normal = 1;
const int Equipment_Sword_Flame = 2;
const int Equipment_Handgun_Normal = 3;

const string EquipmentValueNames[] = {"Equipment_None_None",
							"Equipment_Sword_Normal",
							"Equipment_Sword_Flame",
							"Equipment_Handgun_Normal"};

const string EquipmentNames[] = {"無し",
							"通常剣",
							"炎剣",
							"ハンドガン"};

const string EquipmentInfo[] = {"装備を使用しません。<>装備種類 : 無し",
							"通常剣を用いて攻撃を行います。<>装備種類 : 剣",
							"炎剣を用いて攻撃を行います。<>装備種類 : 剣",
							"ハンドガンを用いて攻撃を行います。<>装備種類 : HG"};

const int Effect_Sword_Shockwave = 0;
const int Effect_Handgun_Bullet = 1;
const int Effect_Other_Hit0 = 2;
const int Effect_Other_Hit1 = 3;
const int Effect_Other_StrikeGround = 4;

vector<int> GetSkillList(string EquipmentName);//装備に対応したスキルを取得
string GetEquipmentNameforSkill(int Skill);//スキル名から装備種類を取り出す
string GetEquipmentNameforEquipment(int Equipment);//装備名から装備種類を取り出す

class Character : public Physics2DImage
{
private:
	string CharacterType;//このCharacterの種類
	float ScrollDistance;//マップがどれだけスクロールしているか

	//効果音
	static int SE_Slash,
				SE_Splash,
				SE_SwingLight,
				SE_SwingHeavy,
				SE_Bomb;
public:
	int MaxHP;//最大HP
	int HP;//HP
	int State;//現在の状態
	int StateTime;//現在の状態が続く時間
	int BeforeStateTime;//前フレームでのStateTime
	int Time;//時間
	int Width,Height;
	b2Vec2 BeforeVect;//前フレームでの速度
	int AffectStanTime;//自分の攻撃で敵に与えたStanTime
	int ComboCount;//コンボ数
	int BeforeComboCount;//前フレームでのコンボ数
	static vector<Character*> CharacterList;//このプログラム上で生成されたすべてのCharacterクラス
	static vector<HitBox> HitBoxList;//Characterによって生成されたすべてのヒットボックス
	static vector<Image_2D> AnimeGraphs;//アニメーション用の画像
	static vector<Image_2D> EquipmentGraphs;//装備画像
	static vector<Image_2D> EffectGraphs;//エフェクト用画像
	static vector<Image_2D> Effects;//描画するエフェクト

	void InitChara(b2World *World,string CharaType,float Density,float Friction,int InputMaxHP);//Characterクラスの初期化
	virtual void Initialize(b2World *World,string CharaType,float Density,float Friction,int MaxHP) = 0;//初期化
	virtual bool UseSkill(int SkillNumber,int EquipmentNumber);//スキルの使用
	virtual bool JudgeSkillCancel();//現在使用しているスキルがギャンセルできるかを判定
	void Step();//毎フレームの処理
	virtual void Ctrl() = 0;//操作
	virtual Character* GetPlayer();//プレイヤーのポインターを取得
	void SetScrollDistance(float Distance);
	string GetCharaType();//このCharacterの種類を取得
	void DeleteCharacterList();//自信をCharacterListから削除
	virtual ~Character()
	{
		DeleteCharacterList();
		if(Graph.size() > 0)
		{
			b2World *World = GetBody()->GetWorld();
			World->DestroyBody(GetBody());
		}
	}
};

#endif