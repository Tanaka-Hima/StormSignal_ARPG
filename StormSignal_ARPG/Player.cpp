#include "Player.h"
#include "Functions.h"
#include "ConstantValue.h"
#include <DxLib.h>

void Player::Initialize(b2World *World,string CharaType,float Density,float Friction,int MaxHP)
{
	InitChara(World,CharaType,Density,Friction,MaxHP);

	//フォント読み込み
	DeleteFontToHandle(FontSmall);
	FontSmall = CreateFontToHandle( "メイリオ" , 15 , 3 ,-1,-1,2) ;
	DeleteFontToHandle(FontMiddle);
	FontMiddle = CreateFontToHandle( "メイリオ" , 20 , 7 ,-1,-1,2) ;
	DeleteFontToHandle(FontBig);
	FontBig = CreateFontToHandle( "メイリオ" , 40 , 10 ,-1,-1,3) ;

	//スキルの初期化
	for(int i=0;i<3;i++)
	{
		Equipments[i] = 1;
		for(int j=0;j<3;j++)
		{
			SkillSet[i][j][0] = Skill_Sword_Front;
			SkillSet[i][j][1] = 0;
			SkillSet[i][j][2] = 0;
			SkillSet[i][j][3] = SkillAvailableCount[Skill_Sword_Front];
		}
	}

	SkillCursorPoint.x = SkillCursorPoint.y = 0;
	ChangeSkillPoint.x = ChangeSkillPoint.y = -1;
	SkillChangeFlag = false;

	GetShape()->SetAsBox(12/2/Box_Rate,24/2/Box_Rate);

	GetFixtureDef()->shape = GetShape();
	GetFixtureDef()->density = Density;
	GetFixtureDef()->friction = Friction;

	GetBody()->DestroyFixture(GetFixture());
	GetBody()->CreateFixture(GetFixtureDef());

	Center_x = 64;
	Center_y = 52;

	Width = 9;
	Height = 24;

	EX = 200;
	MaxEX = 400;

	//スキル設定ウィンドウ作成
	SkillWindow.Initialize(25,25,Screen_Width-50,Screen_Height-50,Black,LightBlack);
	SkillWindow.Visible = false;
	InfoPanel.Initialize(15,330,300,60,Black,LightBlack);
	InfoPanel2.Initialize(SkillWindow.GetWidth()/2,330,300,60,Black,LightBlack);

	SkillImages.clear();
	int Length = AnimeGraphs.size();
	for(int i=0;i<Length;i++)
	{
		SkillImages.push_back(AnimeGraphs[i]);
		SkillImages[SkillImages.size()-1].Initialize();
		SkillImages[SkillImages.size()-1].Anime_Speed = 500;
		
		Window Panel;
		SkillPanels.push_back(Panel);
		SkillPanels[SkillPanels.size()-1].Initialize(0,0,64,64,Black,LightBlack);
	}

	EquipmentImages.clear();
	Length = EquipmentGraphs.size();
	for(int i=0;i<Length;i++)
	{
		Image_2D Image;
		EquipmentImages.push_back(EquipmentGraphs[i]);
		EquipmentImages[EquipmentImages.size()-1].Initialize();
		EquipmentImages[EquipmentImages.size()-1].Anime_Speed = 500;
		
		Window Panel;
		EquipmentPanels.push_back(Panel);
		EquipmentPanels[EquipmentPanels.size()-1].Initialize(0,0,64,64,Black,LightBlack);
	}
}

void Player::Ctrl()
{
	//プレイヤーがマップから落ちた場合はHPを0にする
	if(y > Screen_Height + 30)HP = 0;

	//プレイヤーが地面に接触しているかを確認
	bool Flag = false;
	for(b2ContactEdge *i = GetBody()->GetContactList();i;i = i->next)
	{
		string Str = (char*)i->contact->GetFixtureB()->GetBody()->GetUserData();
		if(i->contact->GetManifold()->localNormal.y == 1 && Str == "Ground")
		{
			Flag = true;
		}
		
	}

	//プレイヤーのコンボが途切れていて、かつ地面にいる場合はADを初期化する
	if(ComboCount == 0 && Flag && JudgeSkillCancel())InitAllSkillAvailableCount();

	//コンボが繋がった場合、EXゲージを増やす
	if(ComboCount > BeforeComboCount)EX += ComboCount;
	if(EX > MaxEX)EX = MaxEX;

	//スタン中は行動できない
	if(State == Skill_None_Stan && StateTime > 0)return;

	//スキルウィンドウ表示中は行動できない
	if(SkillWindow.Visible)return;

	//スキル使用
	int Key[9] = {KEY_INPUT_Q,KEY_INPUT_W,KEY_INPUT_E,
					KEY_INPUT_A,KEY_INPUT_S,KEY_INPUT_D,
					KEY_INPUT_Z,KEY_INPUT_X,KEY_INPUT_C};
	for(int i=0;i<9;i++)
	{
		if(CheckKeyDown(Key[i]) && (SkillSet[i%3][(int)(i/3)][3] > 0 || SkillSet[i%3][(int)(i/3)][3] == -1))
		{
			if(SkillConsumeEX[SkillSet[i%3][(int)(i/3)][0]] <= EX)
			{
				if(UseSkill(SkillSet[i%3][(int)(i/3)][0],Equipments[0]))
				{//スキル使用成功時にはAD,EXを減らす
					EX -= SkillConsumeEX[SkillSet[i%3][(int)(i/3)][0]];
					SkillSet[i%3][(int)(i/3)][3]--;
				}
			}
		}
	}

	//スキル使用中には移動、ジャンプができない
	if(!JudgeSkillCancel())return;
	b2Vec2 Vect = GetBody()->GetLinearVelocity();

	//移動
	if(CheckHitKey(KEY_INPUT_LEFT))
	{
		Vect.x = -MoveSpeed;
		Direction = ImageDirection_Left;
	}
	if(CheckHitKey(KEY_INPUT_RIGHT))
	{
		Vect.x = MoveSpeed;
		Direction = ImageDirection_Right;
	}

	if(Flag && CheckKeyDown(KEY_INPUT_SPACE))
	{//ジャンプ(空中不可)
		Vect.y = -MoveSpeed*2.5;
	}

	//地面との摩擦
	if(Flag && Vect.x != 0)Vect.x -= (float32)(Vect.x / fabs(Vect.x) * 0.05);

	//ベクトルを適用
	GetBody()->SetLinearVelocity(Vect);
}

void Player::StepSkillWindow()
{
	//スキルウィンドウトグル
	if(CheckKeyDown(KEY_INPUT_LALT))SkillWindow.Visible = 1 - SkillWindow.Visible;

	//スキルウィンドウが有効であるかどうか確認
	if(!SkillWindow.Visible)return;

	for(int i=0;i<3;i++)
	{//装備中武器に合わせたスキルセットを取得
		EnableSkillList[i] = GetSkillList(GetEquipmentNameforEquipment(Equipments[i]));
	}

	//操作
	if(!SkillChangeFlag)
	{//カーソルが左にあり、変更操作をおこなっていない状態
		if(CheckKeyDown(KEY_INPUT_LEFT))SkillCursorPoint.x--;
		if(CheckKeyDown(KEY_INPUT_RIGHT))SkillCursorPoint.x++;
		if(CheckKeyDown(KEY_INPUT_UP))SkillCursorPoint.y--;
		if(CheckKeyDown(KEY_INPUT_DOWN))SkillCursorPoint.y++;
	
		if(SkillCursorPoint.x < 0)SkillCursorPoint.x = 2;
		else if(SkillCursorPoint.x > 2)SkillCursorPoint.x = 0;
		if(SkillCursorPoint.y < 0)SkillCursorPoint.y = 3;
		else if(SkillCursorPoint.y > 3)SkillCursorPoint.y = 0;
	}else
	{//スキル変更を行おうとしている状態
		if(CheckKeyDown(KEY_INPUT_LEFT))ChangeSkillPoint.x--;
		if(CheckKeyDown(KEY_INPUT_RIGHT))ChangeSkillPoint.x++;

		if(SkillCursorPoint.y != 0)
		{//スキル変更
			if(CheckKeyDown(KEY_INPUT_UP))
			{
				ChangeSkillPoint.x = 0;
				ChangeSkillPoint.y--;
			}
			if(CheckKeyDown(KEY_INPUT_DOWN))
			{
				ChangeSkillPoint.x = 0;
				ChangeSkillPoint.y++;
			}

			int Length = EnableSkillList[ChangeSkillPoint.y-1].size()-1;

			if(Length < ChangeSkillPoint.x)ChangeSkillPoint.x = 0;
			else if(0 > ChangeSkillPoint.x)ChangeSkillPoint.x = Length;

			if(ChangeSkillPoint.y < 1)ChangeSkillPoint.y = 3;
			else if(ChangeSkillPoint.y > 3)ChangeSkillPoint.y = 1;
		}else
		{//装備変更
			int Length = GetArrayLength(EquipmentNames)-1;
			if(ChangeSkillPoint.x < 0)ChangeSkillPoint.x = Length;
			else if(ChangeSkillPoint.x > Length)ChangeSkillPoint.x = 0;
		}
	}

	if(CheckKeyDown(KEY_INPUT_RETURN))
	{//変更決定
		if(!SkillChangeFlag)
		{
			ChangeSkillPoint.x = 0;
			ChangeSkillPoint.y = SkillCursorPoint.y;
		}else
		{
			if(SkillCursorPoint.y != 0)
			{
				SkillSet[SkillCursorPoint.x][SkillCursorPoint.y-1][0] = EnableSkillList[ChangeSkillPoint.y-1][ChangeSkillPoint.x];
				SkillSet[SkillCursorPoint.x][SkillCursorPoint.y-1][3] = 0;
			}else Equipments[SkillCursorPoint.x] = ChangeSkillPoint.x;

			for(int i=0;i<3;i++)
			{//装備中武器に合わせたスキルセットを取得
				EnableSkillList[i] = GetSkillList(GetEquipmentNameforEquipment(Equipments[i]));
			}

			#pragma region 使用できないスキルを検索し、無効にする
			vector<POINT> Points;

			int Skill[3][3];
			for(int i=0;i<3;i++)
			{
				for(int j=0;j<3;j++)
				{
					Skill[i][j] = 0;
				}
			}
			
			for(int i=0;i<3;i++)
			{
				int Length = EnableSkillList[i].size();
				for(int j=0;j<Length;j++)
				{
					for(int x=0;x<3;x++)
					{
						for(int y=0;y<3;y++)
						{
							if(SkillSet[x][y][0] == EnableSkillList[i][j])
							{
								Skill[x][y] = 1;
							}
						}
					}
				}
			}

			for(int i=0;i<3;i++)
			{
				for(int j=0;j<3;j++)
				{
					if(!Skill[i][j])
					{
						SkillSet[i][j][0] = 0;
					}
				}
			}
			#pragma endregion
		}

		if(SkillChangeFlag)ChangeSkillPoint.x = ChangeSkillPoint.y = -1;
		SkillChangeFlag = 1 - SkillChangeFlag;
	}

	//描画
	SkillWindow.ReWindow();
	SkillWindow.SetDrawThisWindow();

	//DrawBox(15,10,110,35,Black,false);
	
	//パネルの制作
	int Length = AnimeGraphs.size();
	for(int i=0;i<Length;i++)
	{
		SkillPanels[i].ReWindow();
		SkillPanels[i].SetDrawThisWindow();

		SkillImages[i].x = SkillPanels[i].GetWidth()/2;
		SkillImages[i].y = SkillPanels[i].GetHeight()/2+24;
		SkillImages[i].Draw(true);
	}

	Length = EquipmentGraphs.size();
	for(int i=0;i<Length;i++)
	{
		EquipmentPanels[i].ReWindow();
		EquipmentPanels[i].SetDrawThisWindow();

		EquipmentImages[i].x = EquipmentPanels[i].GetWidth()/2;
		EquipmentImages[i].y = EquipmentPanels[i].GetHeight()/2+24;
		EquipmentImages[i].Draw(true);
	}

	SkillWindow.SetDrawThisWindow();

	for(int i=0;i<4;i++)
	{
		for(int j=0;j<3;j++)
		{
			int Select1 = 0,Select2 = 0;
			if(SkillCursorPoint.x == j && SkillCursorPoint.y == i)Select1 = 10;
			if(ChangeSkillPoint.x == j && ChangeSkillPoint.y == i && SkillChangeFlag)Select2 = 10;
			if(i>0)
			{//スキル窓の描画
				//DrawRotaGraph2(15+j*70+32,50+i*70+32,64,40,1,0,AnimeGraphs[SkillSet[j][i-1][0]][0],true);
				if(i < 4)
				{//使用中スキル
					SkillPanels[SkillSet[j][i-1][0]].x = 15+j*70;
					SkillPanels[SkillSet[j][i-1][0]].y = 50+i*70-Select1;
					SkillPanels[SkillSet[j][i-1][0]].Draw();
				}
				/*
				if(EnableSkillList[i-1].size() > j)
				{
					SkillPanels[EnableSkillList[i-1][j]].x = 15+j*70 + SkillWindow.GetWidth()/2;
					SkillPanels[EnableSkillList[i-1][j]].y = 50+i*70-Select2;
					SkillPanels[EnableSkillList[i-1][j]].Draw();
				}
				*/
			}else
			{//装備窓の描画
				//使用中装備
				EquipmentPanels[Equipments[j]].x = 15+j*70;
				EquipmentPanels[Equipments[j]].y = 50+i*70-Select1;
				EquipmentPanels[Equipments[j]].Draw();

			}
			//DrawBox(15+j*70,50+i*70,15+j*70+64,50+i*70+64,Black,false);
		}
	}

	for(int i=0;i<4;i++)
	{
		for(int j=0;j<5;j++)
		{
			int Select2 = 0;
			if(i>0)
			{//スキル窓の描画
				int ShowSkill = j;
				if(ChangeSkillPoint.x >= 5 && ChangeSkillPoint.y == i)ShowSkill += ChangeSkillPoint.x - 4;
				if(ChangeSkillPoint.x == ShowSkill && ChangeSkillPoint.y == i && SkillChangeFlag)Select2 = 10;
				if(EnableSkillList[i-1].size() <= j)continue;
				SkillPanels[EnableSkillList[i-1][ShowSkill]].x = 15+j*70 + SkillWindow.GetWidth()/2;
				SkillPanels[EnableSkillList[i-1][ShowSkill]].y = 50+i*70-Select2;
				SkillPanels[EnableSkillList[i-1][ShowSkill]].Draw();
			}else
			{//装備窓の描画
				int ShowEquip = j;
				if(ChangeSkillPoint.x >= 5 && ChangeSkillPoint.y == i)ShowEquip += ChangeSkillPoint.x - 4;
				if(ChangeSkillPoint.x == ShowEquip && ChangeSkillPoint.y == i && SkillChangeFlag)Select2 = 10;
				if(GetArrayLength(EquipmentValueNames) > ShowEquip)
				{
					EquipmentPanels[ShowEquip].x = 15+j*70 + SkillWindow.GetWidth()/2;
					EquipmentPanels[ShowEquip].y = 50+i*70-Select2;
					EquipmentPanels[ShowEquip].Draw();
				}
			}
		}
	}

	InfoPanel.ReWindow();
	InfoPanel2.ReWindow();

	//装備、スキル説明の描画
	string StrInfo;
	if(SkillCursorPoint.y != 0)StrInfo = SkillInfo[SkillSet[SkillCursorPoint.x][SkillCursorPoint.y-1][0]];
	else StrInfo = EquipmentInfo[Equipments[SkillCursorPoint.x]];
	InfoPanel.DrawStringInWindow(5,5,DrawString_Left,StrInfo,FontSmall,White);
	SkillWindow.SetDrawThisWindow();
	InfoPanel.Draw();

	StrInfo = "";
	if(SkillChangeFlag)
	{
		if(SkillCursorPoint.y != 0)StrInfo = SkillInfo[EnableSkillList[ChangeSkillPoint.y-1][ChangeSkillPoint.x]];
		else StrInfo = EquipmentInfo[ChangeSkillPoint.x];
		InfoPanel2.DrawStringInWindow(5,5,DrawString_Left,StrInfo,FontSmall,White);
	}
	SkillWindow.SetDrawThisWindow();
	InfoPanel2.Draw();

	SetDrawScreen(DX_SCREEN_BACK);
	SkillWindow.Draw();
}

bool Player::GetSkillWindowVisible()
{
	return SkillWindow.Visible;
}

void Player::SetSkillWindowVisible(bool Visible)
{
	SkillWindow.Visible = Visible;
}

int Player::GetEXGauge()
{
	return EX;
}

void Player::InitAllSkillAvailableCount()
{
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			SkillSet[i][j][3] = SkillAvailableCount[SkillSet[i][j][0]];
		}
	}
}
