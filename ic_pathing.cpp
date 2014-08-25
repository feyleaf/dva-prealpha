#include "globals.h"

void AStarClass::setStart(const coord& _pt)
{
	startPoint.point=coord(_pt);
	startPoint.g=0;
	startPoint.parent=coord(-1, -1);
}
void AStarClass::setTarget(const coord& _pt)
{
	targetPoint.point=coord(_pt);
	targetPoint.h=0;
	targetPoint.parent=coord(_pt);
}
int AStarClass::getHScore(const coord& _a, const coord& _b)
{
	//int ret=(abs(_a.x-_b.x)+abs(_a.y-_b.y));
	int ret=int(1+calcDist(toVector(_a), toVector(_b)));
	return ret;
}

void AStarClass::reset(pathNode& _check)
{
	_check.point=coord(-1,-1);
	_check.parent=coord(-1,-1);
	_check.g=0;
	_check.h=0;
	_check.f=0;
}
void AStarClass::initBounds(const settingStruct& set)
{
	bounds=set.mapGridDimensions;
}
void AStarClass::initialize(const coord& _start, const coord& _target)
{
	openList.clear();
	closedList.clear();
	pathList.clear();
	reset(startPoint);
	reset(targetPoint);
	reset(current);
	reset(checkNorth);
	reset(checkSouth);
	reset(checkEast);
	reset(checkWest);

	setStart(_start);
	setTarget(_target);
	startPoint.h=getHScore(startPoint.point, targetPoint.point);
}
void AStarClass::setParent(pathNode& _check)
{
	_check.parent=coord(current.point);
}

coord& AStarClass::getParent(pathNode& _check)
{
	return _check.parent;
}
bool AStarClass::isOpenListEmpty()
{
	return (openList.empty());
}
bool AStarClass::isInOpen(const pathNode& _check)
{
	for(int i=0; i<int(openList.size()); i++)
	{
		if(openList[i].point==_check.point)
			return true;
	}
	return false;
}
bool AStarClass::isInClosed(const pathNode& _check)
{
	for(int i=0; i<int(closedList.size()); i++)
	{
		if(closedList[i].point==_check.point)
			return true;
	}
	return false;
}

bool AStarClass::isTargetFound()
{
	return (current.point == targetPoint.point);
}
pathNode& AStarClass::selectLowestFscore()
{
	int lowIndex=0;
	int lowestF = 1000;
	checkNorth.f=checkNorth.g+checkNorth.h;
	checkSouth.f=checkSouth.g+checkSouth.h;
	checkEast.f=checkEast.g+checkEast.h;
	checkWest.f=checkWest.g+checkWest.h;
	current.f=current.g+current.h;

	for(int i=0; i<int(openList.size()); i++)
	{
		openList[i].f = openList[i].h+openList[i].g;
		if(openList[i].f<lowestF)
		{
			lowIndex=i;
			lowestF=openList[lowIndex].f;
		}
	}
	return openList[lowIndex];
}
void AStarClass::fillCheckList(TerrainClass& _tr)
{
	reset(checkNorth);
	reset(checkSouth);
	reset(checkEast);
	reset(checkWest);

	checkNorth.point.x=current.point.x;
	checkNorth.point.y=current.point.y-1;
	checkNorth.parent=coord(current.point);
	checkNorth.h=getHScore(checkNorth.point, targetPoint.point);
	if(checkNorth.point.y<0) validNorth=false;
	else if(_tr.getTerrainAt(checkNorth.point)>0) validNorth=false;
	else validNorth=true;
	checkSouth.point.x=current.point.x;
	checkSouth.point.y=current.point.y+1;
	checkSouth.parent=coord(current.point);
	checkSouth.h=getHScore(checkSouth.point, targetPoint.point);
	if(checkSouth.point.y>=bounds.y) validSouth=false;
	else if(_tr.getTerrainAt(checkSouth.point)>0) validSouth=false;
	else validSouth=true;
	checkEast.point.x=current.point.x+1;
	checkEast.point.y=current.point.y;
	checkEast.parent=coord(current.point);
	checkEast.h=getHScore(checkEast.point, targetPoint.point);
	if(checkEast.point.x>=bounds.x) validEast=false;
	else if(_tr.getTerrainAt(checkEast.point)>0) validEast=false;
	else validEast=true;
	checkWest.point.x=current.point.x-1;
	checkWest.point.y=current.point.y;
	checkWest.parent=coord(current.point);
	checkWest.h=getHScore(checkWest.point, targetPoint.point);
	if(checkWest.point.x<0) validWest=false;
	else if(_tr.getTerrainAt(checkWest.point)>0) validWest=false;
	else validWest=true;
}
void AStarClass::updateCost(pathNode& _check, int moveCost)
{
	if(_check.parent==coord(-1, -1)) _check.g=moveCost;
	else _check.g=current.g+moveCost;
}
bool AStarClass::worseThanOpen(const pathNode& _check)
{
	for(int i=0; i<int(openList.size()); i++)
	{
		if(openList[i].point==_check.point)
		{
			if(_check.f>openList[i].f)
				return true;
		}
	}
	return false;

}
bool AStarClass::worseThanClosed(const pathNode& _check)
{
	for(int i=0; i<int(closedList.size()); i++)
	{
		if(closedList[i].point==_check.point)
		{
			if(_check.f>closedList[i].f) return true;
		}
	}
	return false;

}
void AStarClass::removeFromOpen(const coord& _pt)
{
	for(int i=0; i<int(openList.size()); i++)
	{
		if(openList[i].point.x==_pt.x && openList[i].point.y==_pt.y)
		{
			openList.erase(openList.begin()+i);
		}
	}
}
void AStarClass::removeFromClosed(const coord& _pt)
{
	for(int i=0; i<int(closedList.size()); i++)
	{
		if(closedList[i].point.x==_pt.x && closedList[i].point.y==_pt.y)
		{
			closedList.erase(closedList.begin()+i);
		}
	}
}
void AStarClass::removeFromOpen(const pathNode& _check)
{
	for(int i=0; i<int(openList.size()); i++)
	{
		if(openList[i].point==_check.point && openList[i].parent==_check.parent)
		{
			openList.erase(openList.begin()+i);
		}
	}
}
void AStarClass::removeFromClosed(const pathNode& _check)
{
	for(int i=0; i<int(closedList.size()); i++)
	{
		if(closedList[i].point==_check.point && closedList[i].parent==_check.parent)
		{
			closedList.erase(closedList.begin()+i);
		}
	}
}
void AStarClass::addToOpen(const pathNode& _check)
{
	openList.push_back(_check);
}
void AStarClass::addToClosed(const pathNode& _check)
{
	closedList.push_back(_check);
}
AStarClass::AStarClass()
{
}
AStarClass::~AStarClass()
{
}

bool AStarClass::runPathing(TerrainClass& _tr, const coord& _start, const coord& _target)
{
	int index=0;
	initialize(_start, _target);
	addToOpen(startPoint);
	while(!isOpenListEmpty() && !isTargetFound())
	{
		current=selectLowestFscore();
		if(!isTargetFound())
		{
			fillCheckList(_tr);
			if(validNorth)
			{
				updateCost(checkNorth,_tr.getMoveCostAt(checkNorth.point));
				if(!(worseThanOpen(checkNorth) || isInClosed(checkNorth)))
				{
					removeFromOpen(checkNorth.point);
					removeFromClosed(checkNorth);
					setParent(checkNorth);
					checkNorth.h=getHScore(checkNorth.point, targetPoint.point);
					addToOpen(checkNorth);
				}
			}
			if(validSouth)
			{
				updateCost(checkSouth,_tr.getMoveCostAt(checkSouth.point));
				if(!(worseThanOpen(checkSouth) || isInClosed(checkSouth)))
				{
					removeFromOpen(checkSouth);
					removeFromClosed(checkSouth);
					setParent(checkSouth);
					checkSouth.h=getHScore(checkSouth.point, targetPoint.point);
					addToOpen(checkSouth);
				}
			} 
			if(validEast)
			{
				updateCost(checkEast,_tr.getMoveCostAt(checkEast.point));
				if(!(worseThanOpen(checkEast) || isInClosed(checkEast)))
				{
					removeFromOpen(checkEast);
					removeFromClosed(checkEast);
					setParent(checkEast);
					checkEast.h=getHScore(checkEast.point, targetPoint.point);
					addToOpen(checkEast);
				}
			}
			if(validWest)
			{
			updateCost(checkWest,_tr.getMoveCostAt(checkWest.point));
				if(!(worseThanOpen(checkWest) || isInClosed(checkWest)))
				{
					removeFromOpen(checkWest);
					removeFromClosed(checkWest);
					setParent(checkWest);
					checkWest.h=getHScore(checkWest.point, targetPoint.point);
					addToOpen(checkWest);
				}
			}

		}
		removeFromOpen(current.point);
		addToClosed(current);
	}
	pathList.clear();
	for(int i=0; i<int(closedList.size()); i++)
		if(closedList[i].point==_target) index=i;
	pathList.push_back(closedList[index].point);
	do
	{
		for(int i=0; i<int(closedList.size()); i++)
			if(closedList[i].point==closedList[index].parent)
				index=i;
			pathList.push_back(closedList[index].point);
//			pathList.push_back(closedList[i].point);
			
	} while (!(closedList[index].point == startPoint.point));
	//pathList.push_back(startPoint.point);

	return (pathList[0]!=pathList[1]);



}

coord AStarClass::getNextTile(TerrainClass& _tr, const coord& _start, const coord& _target)
{
	if(!runPathing(_tr, _start, _target)) return _start;
	int index=int(pathList.size());
	return pathList[index-2];
}