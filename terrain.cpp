#include "globals.h"

void TerrainClass::initialize()
{
	terrainRecord dummy;
	for(int yy=0; yy<60; yy++)
	{
		for(int xx=0; xx<60; xx++)
		{
			terrainMap[xx][yy].moveCost=1;
			terrainMap[xx][yy].terrainRule=0;
		}
	}
}

bool TerrainClass::setMoveCostAt(coord point, int value)
{
	terrainMap[point.x][point.y].moveCost=value;
	return true;
}

bool TerrainClass::setTerrainRuleAt(coord point, int value)
{
	terrainMap[point.x][point.y].terrainRule=value;
	return true;
}

int TerrainClass::getMoveCostAt(coord point)
{
	return terrainMap[point.x][point.y].moveCost;
}

int TerrainClass::getTerrainAt(coord point)
{
	return terrainMap[point.x][point.y].terrainRule;
}