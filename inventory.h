#ifndef INVENTORY_H
#define INVENTORY_H

struct cellStruct
{
	registeredEntity* ptr_item;
	unsigned int quantity;
};

class InventoryClass
{
public:
	InventoryClass();
	~InventoryClass() {}

	coord tl;
	coord dimensions;
	int capacity;
	std::vector<cellStruct> cellList;

	int cursor;
	int select(coord click);

	int getFirstEmpty();
	int getFirstMatch(unsigned char _id);
	bool isAllSlotsFilled();
	bool isInventoryEmpty();
	bool isSlotEmpty();

	bool clearSlot(int plc);
	bool clearAll();
	bool swap(int plcA, int plcB);
	bool add(registeredEntity* it, short q=1);
	registeredEntity* drop(int plc);
	registeredEntity* getItemAt(int plc);
	registeredEntity* getItemAtCursor();
	short getQuantityAt(int plc);
	short getQuantityAtCursor();

	void sortByID();
	void sortByType();
	void sortByName();

};
#endif// INVENTORY_H
