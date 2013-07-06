#include "DxLib.h"
#include "Box2D\Box2D.h"
#include "ConstantValue.h"

class DebugDraw :
	public b2Draw
{
public:
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	void DrawTransform(const b2Transform& xf);
};

void DrawCircle_Dxlib(int x,int y,int r,int Color,bool FillFlag)
{
	DrawCircle(x,y,r,Color,FillFlag);
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{

	int mode;//一時BLEND記録領域
	int parm;//一時BLENDパラメータ記録領域
	GetDrawBlendMode(&mode,&parm);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,128);

	//DrawString(0,0,"DrawPolygon",GetColor(255,0,0));

	int i = 1;

	for(; i < vertexCount; i++)
	{
		DrawLine(vertices[i-1].x*Box_Rate,vertices[i-1].y*Box_Rate,
		vertices[i].x*Box_Rate,vertices[i].y*Box_Rate,
		GetColor(255*color.r,255*color.g,255*color.b));
	}

	DrawLine(vertices[0].x*Box_Rate, vertices[0].y*Box_Rate,
	vertices[i-1].x*Box_Rate, vertices[i-1].y*Box_Rate,
	GetColor(255*color.r,255*color.g,255*color.b));

	SetDrawBlendMode(mode,parm);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	int mode;//一時BLEND記録領域
	int parm;//一時BLENDパラメータ記録領域

	GetDrawBlendMode(&mode,&parm);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,128);

	//DrawString(0,0,"DrawSolidPolygon",GetColor(255,0,0));

	//半透明化
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);

	for(int i=2; i < vertexCount; i++)
	{
		DrawTriangle(vertices[0].x*Box_Rate,vertices[0].y*Box_Rate,
		vertices[i-1].x*Box_Rate,vertices[i-1].y*Box_Rate,
		vertices[i].x*Box_Rate,vertices[i].y*Box_Rate,
		GetColor( 255*color.r, 255*color.g, 255*color.b),true);
	}

	SetDrawBlendMode(mode,parm);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	int mode;//BLEND記録領域
	int parm;//BLENDパラメータ記録領域
	GetDrawBlendMode(&mode,&parm);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	//DrawString(0,0,"DrawCircle",GetColor(255,0,0));
	DrawCircle_Dxlib(center.x*Box_Rate,center.y*Box_Rate,radius*Box_Rate,GetColor(255*color.r, 255*color.g,255*color.b),false);

	SetDrawBlendMode(mode,parm);
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	int mode;//BLEND記録領域
	int parm;//BLENDパラメータ記録領域
	GetDrawBlendMode(&mode,&parm);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	//DrawString(0,0,"DrawCircle",GetColor(255,0,0));
	DrawCircle_Dxlib(center.x*Box_Rate,center.y*Box_Rate,radius*Box_Rate,GetColor(255*color.r,255*color.g,255*color.b),true);

	SetDrawBlendMode(mode,parm);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	int mode;//BLEND記録領域
	int parm;//BLENDパラメータ記録領域

	GetDrawBlendMode(&mode,&parm);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);

	//DrawString(0,0,"DrawSegment",GetColor(255,0,0));

	DrawLine(p1.x*Box_Rate,p1.y*Box_Rate,p2.x*Box_Rate,p2.y*Box_Rate,GetColor(255*color.r,255*color.g,255*color.b));

	SetDrawBlendMode(mode,parm);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
 //DrawString(0,0,"DrawXForm",GetColor(255,0,0));
 /*
 b2Vec2 p1 = xf.position, p2;
 const float32 k_axisScale = 0.4f;
 glBegin(GL_LINES);
 
 glColor3f(1.0f, 0.0f, 0.0f);
 glVertex2f(p1.x, p1.y);
 p2 = p1 + k_axisScale * xf.R.col1;
 glVertex2f(p2.x, p2.y);

 glColor3f(0.0f, 1.0f, 0.0f);
 glVertex2f(p1.x, p1.y);
 p2 = p1 + k_axisScale * xf.R.col2;
 glVertex2f(p2.x, p2.y);

 glEnd();
 */
}
