#ifndef INVENTORY_H
#define INVENTORY_H

struct cellStruct
{
	unsigned int tmp_idx; //template index
	unsigned int idx_item; //registry index
	unsigned int quantity; //how many are held
	cellStruct() {tmp_idx=0; idx_item=0; quantity=0;}

};

class InventoryRegistryClass
{
public:
	InventoryRegistryClass();
	~InventoryRegistryClass();

	std::vector<registeredEntity*> entities;
	std::vector<vegPack*> vegs;
	std::vector<decoPack*> decos;
	std::vector<seedPack*> seeds;
	std::vector<summonPack*> summons;
	std::vector<toolPack*> tools;
	std::vector<creaturePack*> creatures;
	std::vector<ingredientPack*> ings;

	int registerFromGameObject(const GameObjectContainerClass& obj, int entityIndex);
	int registerFromTemplate(const TemplateRegistryClass& tmp, int entityIndex);
};

class InventoryClass
{
public:
	InventoryClass();
	~InventoryClass() {}

	InventoryRegistryClass reg;

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
	bool add(const GameObjectContainerClass& obj, int entIndex, short q=1);
	bool add(const TemplateRegistryClass& tmp, int tmpIndex, short q=1);
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
