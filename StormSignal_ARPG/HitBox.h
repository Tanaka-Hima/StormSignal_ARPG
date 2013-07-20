#include <Box2D\Box2D.h>

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
	bool Suicide;//攻撃した本人にダメージは入るか？
	b2Vec2 HitVect;//ヒット時に相手へ与えるベクトル
	int Damage;//ダメージ
	int HitCount;//同じ相手へ何回ヒットするか(-1で∞)
	int Duration;//持続時間
	int Time;
	int StanTime;//ヒット時行動不能時間
	bool Follow;//キャラが移動した時にヒットポックスも移動させるか？

	vector<b2PolygonShape*> HittedChara;

public:
	void Initialize(b2PolygonShape InputShape,b2Transform InputTransform,Character* InputAttacker,bool SuicideFlag,b2Vec2 InputHitVect,int InputDamage,int InputHitCount,int InputDuration,int InputStanTime,bool FollowFlag);
	bool HitTestShape(Character* Target,b2PolygonShape* TargetShape,b2Transform TargetTrans);
	bool Step();
	Character* GetCharacter();
	b2Vec2 GetHitVect();
	int GetDamage();
	int GetStanTime();

	void Draw();

};

#endif