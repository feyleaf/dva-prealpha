//ritual.cpp
//feyleaf game studio
//by Benjamin C Watt (@feyleafgames)
//August 17, 2014
////////////////////////

#include "globals.h"

coord RitualClass::placeToPoint(int p)
{
	if(p==0) return coord(7,4);
	if(p==1) return coord(8,6);
	if(p==2) return coord(6,6);
	if(p==3) return coord(7,5);
	return coord(0,0);
}
bool RitualClass::addToRitual(int templateIndex)
{
	if(slots>int(cell.size()))
	{
		ritualCell dummy;
		dummy.point=placeToPoint(cell.size());
		dummy.templateIndex=templateIndex;
		cell.push_back(dummy);
		return true;
	}
	return false;
}

bool RitualClass::isThisRitual()
{
	return false;
}

int RitualClass::templateFromSlot(int p)
{
	if(p>0 && p<=slots && p<int(cell.size()))
		return cell[p].templateIndex;
	else return 0;
}

RitualClass::RitualClass()
{
	cell.clear();
	slots=4;
}

RitualClass::~RitualClass()
{
}