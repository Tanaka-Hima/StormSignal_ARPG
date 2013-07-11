#include <vector>
#include <string>

using namespace std;

#ifndef _MAP
#define _MAP

const char Mapchip_Blank = '0';
const char Mapchip_Clay = '1';
const char Mapchip_ClayFloor = '2';
const char Mapchip_Woodbox = '3';

class Map
{
private:
	vector<char> MapData[14];
	vector<int> MapChips;
	b2BodyDef GroundBodyDef;
	b2Body* GroundBody;
	b2PolygonShape GroundBox;
	int Width;
public:
	void Initialize(void);
	void LoadMapData(string Pass);
	void CreateMap(b2World *World);
	void Draw(void);
};

#endif