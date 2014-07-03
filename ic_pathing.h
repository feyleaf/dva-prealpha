#ifndef IC_PATHING_H
#define IC_PATHING_H

//Iclea Project A* Pathing Algorithm
#include "globals.h"

struct pathNode
{
	coord point;
	coord parent;
	int h;
	int g;
	int f;

	pathNode() {point=coord(0,0); h=0;g=0;f=0; parent=coord(-1,-1);}
	pathNode(const pathNode& src) { point=src.point; h=src.h; g=src.g; f=src.f; parent=src.parent;}
};

class AStarClass
{
protected:
	std::vector<pathNode> openList;
	std::vector<pathNode> closedList;
	pathNode startPoint;
	pathNode targetPoint;
	pathNode current;
	pathNode checkNorth;
	pathNode checkEast;
	pathNode checkSouth;
	pathNode checkWest;
	bool validNorth;
	bool validEast;
	bool validSouth;
	bool validWest;
	coord bounds;

	bool foundIt;
public:
	std::vector<coord> pathList;
	AStarClass();
	~AStarClass();
	void erase() {openList.clear(); closedList.clear(); pathList.clear();}
	void setStart(const coord& _pt); //make startPoint a pathNode
	coord getStart() {return startPoint.point;}
	void setTarget(const coord& _pt);
	void initBounds(const settingStruct& set);
	void initialize(const coord& _start, const coord& _target);
	int getHScore(const coord& _a, const coord& _b);

	bool isOpenListEmpty();
	bool isTargetFound();
	pathNode& selectLowestFscore();
	void fillCheckList(TerrainClass& _tr);
	void updateCost(pathNode& _check, int moveCost=1);
	bool worseThanOpen(const pathNode& _check);
	bool worseThanClosed(const pathNode& _check);
	bool isInOpen(const pathNode& _check);
	bool isInClosed(const pathNode& _check);
	void removeFromOpen(const coord& _pt);
	void removeFromClosed(const coord& _pt);
	void removeFromOpen(const pathNode& _check);
	void removeFromClosed(const pathNode& _check);
	void addToOpen(const pathNode& _check);
	void addToClosed(const pathNode& _check);
	void setParent(pathNode& _check);
	coord& getParent(pathNode& _check);
	void reset(pathNode& _check);

	bool runPathing(TerrainClass& _tr, const coord& _start, const coord& _target);
	coord getNextTile(TerrainClass& _tr, const coord& _start, const coord& _target);



};
#endif

