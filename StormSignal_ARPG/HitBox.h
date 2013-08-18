#include <Box2D\Box2D.h>
#include "Image_2D.h"

#ifndef _HITBOX
#define _HITBOX

#include "Character.h"

class HitBox
{
private:
	b2PolygonShape Shape;
	b2Transform Transform;
	Character* Attacker;
	b2Transform AttackerBeforeTrans;
	b2Vec2 MoveVect;
	bool MoveFlag;
	Image_2D Image;
	bool DrawFlag;
	Image_2D Effect;
	bool EffectFlag;
	bool Suicide;//攻撃した本人にダメージは入るか？
	b2Vec2 HitVect;//ヒット時に相手へ与えるベクトル
	int Damage;//ダメージ
	int HitCount;//同じ相手へ何回ヒットするか(-1で∞)
	int Duration;//持続時間
	int Time;
	int BeforeTime;
	int StanTime;//ヒット時行動不能時間
	bool Follow;//キャラが移動した時にヒットポックスも移動させるか？
	bool AutoAngle;//移動方向にあわせて方向を変化させるか否か

	vector<b2PolygonShape*> HittedChara;

public:
	static vector<Image_2D> Effects;

	void Initialize(b2PolygonShape InputShape,//ヒットボックスの形状
					b2Transform InputTransform,//ヒットボックスの位置、向き
					Character* InputAttacker,//ヒットボックスを生成したCharacter
					bool SuicideFlag,//生成したCharacterに攻撃がヒットするかどうか
					b2Vec2 InputHitVect,//ヒット時に対象へ与えるベクトル
					int InputDamage,//対象へ与えるダメージ
					int InputHitCount,//1対象へヒットできる回数
					int InputDuration,//ヒットボックスが持続する時間
					int InputStanTime,//対象が行動不能になる時間
					bool FollowFlag//ヒットボックスを生成したCharacterを追いかけるかどうか
					);
	void SetMoveFlag(b2Vec2 InputMoveVect,bool InputAutoAngle = false);//この関数を呼び出した場合、1秒に指定分だけHitBoxが移動する
	void SetGraph(Image_2D Image);//この関数を呼び出した場合、ヒットボックスが存在する位置に画像が描画される
	void SetEffect(Image_2D Image);//この関数を呼び出した場合、ヒット時にエフェクトが描画される
	bool HitTestShape(Character* Target,b2PolygonShape* TargetShape,b2Transform TargetTrans);//Characterとの当たり判定
	bool Step();//毎フレームの処理
	Character* GetCharacter();//ヒットボックスを生成したCharacterを取得
	b2Vec2 GetHitVect();//ヒット時に対象へ与えるベクトルを取得
	int GetDamage();//対象へ与えるダメージを取得
	int GetStanTime();//対象が行動不能になる時間を取得

	void Draw();//描画

};

#endif