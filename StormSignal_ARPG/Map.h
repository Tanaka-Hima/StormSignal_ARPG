#include <vector>
#include <string>
#include "Player.h"
#include "Enemy.h"
#include "Object.h"

using namespace std;

#ifndef _MAP
#define _MAP

const string Mapchip_Blank = "00";
const string Mapchip_Clay = "01";
const string Mapchip_ClayFloor = "02";
const string Mapchip_Woodbox = "03";
const string Mapchip_Switch1 = "04";
const string Mapchip_Switch2 = "05";

const string Mapchip_TrainingBag = "10";

const string Mapchip_Player = "99";

const string Trigger_Hit = "Hit";
const string Trigger_Hitted = "Hitted";
const string Trigger_Touch = "Touch";
const string Trigger_Flag = "Flag";
const string Trigger_Flagged = "Flagged";

const string Action_Redraw = "Redraw";
const string Action_Flag = "Flag";

class Map
{
private:
	vector<string> MapData[14];
	vector<vector<string>> ScriptData;
	vector<int> MapChips;
	Player PlayerData;
	vector<Enemy> EnemyData;
	vector<Object> RigidBodies;
	b2BodyDef GroundBodyDef;
	b2Body* GroundBody;
	b2PolygonShape GroundBox;
	int Width;
public:
	void Initialize(b2World *World);
	void LoadMapData(string Pass);
	void LoadScriptData(string Pass);
	void CreateMap(b2World *World);
	void Step();
	void Draw();
};

#endif