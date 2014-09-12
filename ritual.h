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
	int resultslot;
	std::vector<ritualCell> cell;
	coord placeToPoint(int p);
	bool addToRitual(int templateIndex);
	void addSlotToInventory(int p, const TemplateRegistryClass& tmp, InventoryClass& _inv);
	int templateFromSlot(int p);
	bool isThisRitual(RecipeManager& _recipes);
	int findRitual(RecipeManager& _recipes);
	void clear() {cell.clear(); slots=4; resultslot=3;}
	//more stuff later! :)
};


#endif//RITUAL_H