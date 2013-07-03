#include "Player.h"
#include "Functions.h"
#include "ConstantValue.h"
#include <DxLib.h>

void Player::Initialize(b2World *World,void* UserData,float Density,float Friction,int MaxHP)
{
	InitChara(World,UserData,Density,Friction,MaxHP);

	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			SkillSet[i][j][0] = Skill_Sword_Front;
			SkillSet[i][j][1] = 0;
			SkillSet[i][j][2] = 0;
			SkillSet[i][j][3] = 0;
		}
	}

	GetShape()->SetAsBox(12/2/Box_Rate,24/2/Box_Rate);

	GetFixtureDef()->shape = GetShape();
	GetFixtureDef()->density = Density;
	GetFixtureDef()->friction = Friction;

	GetBody()->DestroyFixture(GetFixture());
	GetBody()->CreateFixture(GetFixtureDef());

	Center_x = 64;
	Center_y = 52;
}

void Player::Ctrl(void)
{
	b2Vec2 Vect = GetBody()->GetLinearVelocity();

	//移動関連
	if(CheckHitKey(KEY_INPUT_LEFT))
	{
		Vect.x = -MoveSpeed;
		Direction = -1;
	}
	if(CheckHitKey(KEY_INPUT_RIGHT))
	{
		Vect.x = MoveSpeed;
		Direction = 1;
	}
	if(CheckKeyDown(KEY_INPUT_SPACE))Vect.y = -MoveSpeed*3;

	//スキル使用
	int Key[9] = {KEY_INPUT_Q,KEY_INPUT_W,KEY_INPUT_E,
					KEY_INPUT_A,KEY_INPUT_S,KEY_INPUT_D,
					KEY_INPUT_Z,KEY_INPUT_X,KEY_INPUT_C};

	for(int i=0;i<9;i++)
	{
		if(CheckKeyDown(Key[i]))
		{
			UseSkill(SkillSet[i%3][(int)(i/3)][0]);
			SkillSet[i%3][(int)(i/3)][3] = GetNowCount();
		}
	}

	GetBody()->SetLinearVelocity(Vect);
}