#include "HitBox.h"
#include "ConstantValue.h"
#include <DxLib.h>

void HitBox::Initialize(b2PolygonShape InputShape,b2Transform InputTransform,Character* InputAttacker,bool SuicideFlag,b2Vec2 InputHitVect,int InputDamage,int InputHitCount,int InputDuration,int InputStanTime,bool FollowFlag)
{
	Shape = InputShape;
	Transform = InputTransform;
	Attacker = InputAttacker;
	AttackerBeforeTrans = Attacker->GetBody()->GetTransform();
	MoveFlag = false;
	DrawFlag = false;
	Suicide = SuicideFlag;
	HitVect = InputHitVect;
	Damage = InputDamage;
	HitCount = InputHitCount;
	Duration = InputDuration;
	Follow = FollowFlag;

	BeforeTime = Time = GetNowCount();
	StanTime = InputStanTime;
}

void HitBox::SetMoveFlag(b2Vec2 InputMoveVect)
{
	MoveFlag = true;
	MoveVect = InputMoveVect;
}

void HitBox::SetGraph(Image_2D InputImage)
{
	DrawFlag = true;
	Image = InputImage;
}

bool HitBox::HitTestShape(Character* Target,b2PolygonShape *TargetShape,b2Transform TargetTrans)
{
	if(Target == Attacker && !Suicide)return false;

	int Length = HittedChara.size();
	int Count = 0;
	for(int i=0;i<Length;i++)
	{
		if(HittedChara[i] == TargetShape)Count++;
	}
	if(Count >= HitCount)return false;

	b2Manifold Manifold;

	b2CollidePolygons(&Manifold,&Shape,Transform,TargetShape,TargetTrans);
	if(Manifold.pointCount > 0)
	{
		HittedChara.push_back(TargetShape);
		return true;
	}else return false;
}

bool HitBox::Step()
{
	//Characterを追尾する
	if(Follow)
	{
		b2Transform Trans = Attacker->GetBody()->GetTransform();
		b2Vec2 Vect = Trans.p - AttackerBeforeTrans.p;
		Transform.p += Vect;
		AttackerBeforeTrans = Trans;
	}

	//HitBoxの移動
	if(MoveFlag)
	{
		int Now = GetNowCount();
		Transform.p.x += MoveVect.x * ((Now - BeforeTime) / 1000.0);
		Transform.p.y += MoveVect.y * ((Now - BeforeTime) / 1000.0);
		BeforeTime = Now;
	}

	if(GetNowCount() - Time > Duration)
	{//持続時間を過ぎていれば削除
		return true;
	}
	return false;
}

Character* HitBox::GetCharacter()
{
	return Attacker;
}

b2Vec2 HitBox::GetHitVect()
{
	return HitVect;
}

int HitBox::GetDamage()
{
	return Damage;
}

int HitBox::GetStanTime()
{
	return StanTime;
}

void HitBox::Draw()
{
	if(DrawFlag)
	{
		Image.x = Transform.p.x * Box_Rate;
		Image.y = Transform.p.y * Box_Rate;
		Image.Draw(true);
	}

	const b2Color& color = b2Color(1,0,0);

	int mode;//一時BLEND記録領域
	int parm;//一時BLENDパラメータ記録領域
	GetDrawBlendMode(&mode,&parm);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,128);

	//DrawString(0,0,"DrawPolygon",GetColor(255,0,0));

	int i = 1;

	for(; i < Shape.GetVertexCount(); i++)
	{
		DrawLine((Shape.GetVertex(i-1).x + Transform.p.x)*Box_Rate,(Shape.GetVertex(i-1).y + Transform.p.y)*Box_Rate,
		(Shape.GetVertex(i).x + Transform.p.x)*Box_Rate,(Shape.GetVertex(i).y + Transform.p.y)*Box_Rate,
		GetColor(255*color.r,255*color.g,255*color.b));
	}

	DrawLine((Shape.GetVertex(0).x + Transform.p.x)*Box_Rate, (Shape.GetVertex(0).y + Transform.p.y)*Box_Rate,
	(Shape.GetVertex(i-1).x + Transform.p.x)*Box_Rate, (Shape.GetVertex(i-1).y + Transform.p.y)*Box_Rate,
	GetColor(255*color.r,255*color.g,255*color.b));

	SetDrawBlendMode(mode,parm);
}