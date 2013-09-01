#include "HitBox.h"
#include "ConstantValue.h"
#include "SE.h"
#include <DxLib.h>

vector<Image_2D> HitBox::Effects;

void HitBox::Initialize(b2PolygonShape InputShape,b2Transform InputTransform,Character* InputAttacker,bool SuicideFlag,b2Vec2 InputHitVect,int InputDamage,int InputHitCount,int InputDuration,int InputStanTime,bool FollowFlag)
{
	Shape = InputShape;
	Transform = InputTransform;
	Attacker = InputAttacker;
	AttackerBeforeTrans = Attacker->GetBody()->GetTransform();
	MoveFlag = false;
	DrawFlag = false;
	EffectFlag = false;
	SEHandle = -1;
	Suicide = SuicideFlag;
	HitVect = InputHitVect;
	Damage = InputDamage;
	HitCount = InputHitCount;
	Duration = InputDuration;
	Follow = FollowFlag;

	BeforeTime = Time = GetNowCount();
	StanTime = InputStanTime;

	AutoAngle = false;
}

void HitBox::SetMoveFlag(b2Vec2 InputMoveVect,bool InputAutoAngle)
{
	MoveFlag = true;
	MoveVect = InputMoveVect;
	AutoAngle = InputAutoAngle;
}

void HitBox::SetGraph(Image_2D InputImage)
{
	DrawFlag = true;
	Image = InputImage;
}

void HitBox::SetEffect(Image_2D InputImage)
{
	EffectFlag = true;
	Effect = InputImage;
}

void HitBox::SetSE(int Handle)
{
	SEHandle = Handle;
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
	if(Count >= HitCount && HitCount > 0)return false;

	b2Manifold Manifold;

	b2CollidePolygons(&Manifold,&Shape,Transform,TargetShape,TargetTrans);
	if(Manifold.pointCount > 0)
	{
		if(EffectFlag)
		{
			Effect.x = ((Manifold.points[0].localPoint.x + Manifold.points[1].localPoint.x)/2.f + TargetTrans.p.x) * Box_Rate;
			Effect.y = ((Manifold.points[0].localPoint.y + Manifold.points[1].localPoint.y)/2.f + TargetTrans.p.y) * Box_Rate;
			Effects.push_back(Effect);
		}
		HittedChara.push_back(TargetShape);

		Attacker->AffectStanTime = StanTime;
		Attacker->ComboCount++;

		SEManager.Play(SEHandle);

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
		if(AutoAngle)
		{
			Image.Angle = atan2(MoveVect.y,MoveVect.x);
		}
		Image.Draw(true);
	}

	#ifdef _DEBUG
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
	#endif
}