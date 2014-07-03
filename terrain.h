#ifndef TERRAIN_H
#define TERRAIN_H

#include "globals.h"

struct terrainRecord
{
	int moveCost;
	int terrainRule;

	terrainRecord() {moveCost=1; terrainRule=0;}
};

class TerrainClass
{
protected:
	terrainRecord terrainMap[60][60];

public:
	void initialize();
	bool setMoveCostAt(coord point, int value);
	bool setTerrainRuleAt(coord point, int value);
	int getMoveCostAt(coord point);
	int getTerrainAt(coord point);

	TerrainClass() {initialize();}
	~TerrainClass() {}
};

#endif //TERRAIN_H