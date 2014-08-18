//ritual.h
//feyleaf game studio
//by Benjamin C Watt (@feyleafgames)
//August 17, 2014
///////////////////////////////////

#ifndef RITUAL_H
#define RITUAL_H

struct ritualCell
{
	int templateIndex;
	coord point;
};

class RitualClass
{
public:
	RitualClass();
	~RitualClass();
	int slots;
	std::vector<ritualCell> cell;
	coord placeToPoint(int p);
	bool addToRitual(int templateIndex);
	void addSlotToInventory(int p, InventoryClass& _inv);
	int templateFromSlot(int p);
	bool isThisRitual();
	//more stuff later! :)
};


#endif//RITUAL_H