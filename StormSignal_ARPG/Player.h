#include "Character.h"
#include "Window.h"

#ifndef _PLAYER
#define _PLAYER

const float MoveSpeed = 5.0f;

class Player : public Character
{
private:
	int SkillSet[3][3][4];//x,y,{í—Ş,ƒŒƒxƒ‹,ŒoŒ±’l,CD}
	vector<int*> SkillList;
	Window SkillWindow;
	vector<Window> SkillPanels;
	vector<Image_2D> SkillImages;
	vector<Window> EquipmentPanels;
	vector<Image_2D> EquipmentImages;
	int Equipments[3];
public:
	void Initialize(b2World *World,void* UserData,float Density,float Friction,int MaxHP);
	void Ctrl(void);//‘€ì
	void StepSkillWindow(void);
};

#endif