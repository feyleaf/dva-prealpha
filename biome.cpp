//biome.cpp

#include "globals.h"

BiomeClass::BiomeClass()
{
	strcpy_s(name, "");
	maxVolume=8;
	maxReach=3;
	points.clear();
}

BiomeClass::~BiomeClass()
{
}

bool BiomeClass::addPoint(coord _pt)
{
	bool flag=false;
	for(int i=0; i<int(points.size()-1); i++)
	{
		if(points[i]==_pt) flag=true;
	}
	if(!flag)
	{
		points.push_back(_pt);
	}
	return flag;
}