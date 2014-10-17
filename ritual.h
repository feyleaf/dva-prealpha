//ritual.h
//feyleaf game studio
//by Benjamin C Watt (@feyleafgames)
//August 17, 2014
///////////////////////////////////

#ifndef RITUAL_H
#define RITUAL_H

struct ritualCell
{
	int entityIndex;
	coord point;
	ritualCell() {entityIndex=0; point=coord(0,0);}
};

class RitualClass
{
public:
	RitualClass();
	~RitualClass();
	int slots;
	int cursor;
	int resultslot;
	std::vector<ritualCell> cell;
	coord placeToPoint(int p);
	bool addToRitual(int entityIndex);
	void addSlotToInventory(int p, const EtherRegistryClass& _eth, InventoryClass& _inv);
	int templateFromSlot(const EtherRegistryClass& _eth, int p);
	bool isThisRitual(const EtherRegistryClass& _eth, RecipeManager& _recipes);
	int findRitual(const EtherRegistryClass& _eth, RecipeManager& _recipes);
	void clear() {cell.clear(); cursor=0; slots=4; resultslot=3; while(int(cell.size())<slots) cell.push_back(ritualCell());}
	//more stuff later! :)
};


#endif//RITUAL_H