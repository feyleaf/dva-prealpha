#ifndef INVENTORY_H
#define INVENTORY_H

class InventoryRegistryClass
{
public:
	InventoryRegistryClass();
	~InventoryRegistryClass();

	std::vector<int> items;

	int registerFromEtherIndex(const EtherRegistryClass& _eth, int entityIndex);
};

struct cellStruct
{
	unsigned int tmp_idx; //template index
	InventoryRegistryClass l; //ether registry index
	cellStruct() {tmp_idx=0; l.items.clear();}

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
	bool addFromIndex(const EtherRegistryClass& _eth, int entIndex);
	bool addEntityFromTemplate(const TemplateRegistryClass& _tmp, EtherRegistryClass& _eth, const char* _name);
	bool addSummonFromTemplate(const TemplateRegistryClass& _tmp, EtherRegistryClass& _eth, const char* _summon, const char* _contained);
	unsigned int drop(int plc);
	unsigned int getItemAt(int plc);
	unsigned int getItemAtCursor();
	short getQuantityAt(int plc);
	short getQuantityAtCursor();

	void sortByID();
	void sortByType();
	void sortByName();

};
#endif// INVENTORY_H
