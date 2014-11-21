#include "globals.h"

InventoryRegistryClass::InventoryRegistryClass()
{
	items.clear();
}

InventoryRegistryClass::~InventoryRegistryClass()
{
}

int InventoryRegistryClass::registerFromEtherIndex(const EtherRegistryClass& _eth, int index)
{
	if(_eth.regEntities[index] == NULL) return 0;
	items.push_back(index);
	int realIndex=int(items.size()-1);
	return realIndex;
}

InventoryClass::InventoryClass()
{
	tl=coord(24,7);
	dimensions=coord(5,5);
	capacity=dimensions.x*dimensions.y;
	cursor=1;
	cellList.clear();
	for(int i=0; i<capacity; i++) cellList.push_back(cellStruct());
}

int InventoryClass::select(coord click)
{
	coord _pt=click;
	if(_pt.x<0 || _pt.y<0 || _pt.x>dimensions.x || _pt.y>dimensions.y)
	{
		return cursor;
	}
	else
	{
		cursor=(_pt.x+(_pt.y*dimensions.x));
		if(cursor>capacity) cursor=capacity;
		return cursor;
	}
}
bool InventoryClass::clearSlot(int plc)
{
	cellList[plc].tmp_idx=0;
	while(!cellList[plc].l.items.empty())
		cellList[plc].l.items.pop_back();
	return true;
}

bool InventoryClass::clearAll()
{
	for(int i=0; i<capacity; i++)
	{
		if(!clearSlot(i)) return false;
	}
	return true;
}

bool InventoryClass::swap(int plcA, int plcB)
{
	if(plcA==plcB) return true;
	if(plcA<0 || plcA>capacity) return false;
	if(plcB<0 || plcB>capacity) return false;
	cellStruct temp;

	temp.tmp_idx = cellList[plcA].tmp_idx;
	for(int i=0; i<int(cellList[plcA].l.items.size()); i++)
		temp.l.items.push_back(cellList[plcA].l.items[i]);

	cellList[plcA].l.items.clear();
	cellList[plcA].tmp_idx = cellList[plcB].tmp_idx;
	for(int i=0; i<int(cellList[plcB].l.items.size()); i++)
		cellList[plcA].l.items.push_back(cellList[plcB].l.items[i]);

	cellList[plcB].l.items.clear();
	cellList[plcB].tmp_idx = temp.tmp_idx;
	for(int i=0; i<int(temp.l.items.size()); i++)
		cellList[plcB].l.items.push_back(temp.l.items[i]);

	return true;
}


bool InventoryClass::isAllSlotsFilled()
{
	for(int i=0; i<capacity; i++)
	{
		if(cellList[i].l.items.empty()) return false;
	}
	return true;
}

bool InventoryClass::isInventoryEmpty()
{
	for(int i=0; i<capacity; i++)
	{
		if(!cellList[i].l.items.empty()) return false;
	}
	return true;
}

bool InventoryClass::isSlotEmpty()
{
	return (cellList[cursor].l.items.empty());
}

unsigned int InventoryClass::drop(int plc)
{
	//if(cellList[plc].l.items.empty())
	//{
	//	cellList[plc].tmp_idx=0;
	//	return 0;
	//}
	unsigned int ret=cellList[plc].l.items[cellList[plc].l.items.size()-1];
	cellList[plc].l.items.pop_back();
	if(cellList[plc].l.items.empty()) cellList[plc].tmp_idx=0;
	return ret;
}

unsigned int InventoryClass::getItemAt(int plc)
{
	if(cellList[plc].l.items.empty()) return 0;
	return cellList[plc].l.items[cellList[plc].l.items.size()-1];
}

unsigned int InventoryClass::getItemAtCursor()
{
	return getItemAt(cursor);
}

short InventoryClass::getQuantityAt(int plc)
{
	return short(cellList[plc].l.items.size());
}

short InventoryClass::getQuantityAtCursor()
{
	return getQuantityAt(cursor);
}

int InventoryClass::getFirstEmpty()
{
	int ret=-1;
	for(int i=0; i<capacity; i++)
	{
		if(cellList[i].l.items.empty())
		{
			ret=i;
			return ret;
		}
	}
	return ret;

}

int InventoryClass::getFirstMatch(unsigned char _id)
{
	int ret=-1;
	for(int i=0; i<capacity; i++)
	{
		if(cellList[i].tmp_idx!=0)
		{		
			if(cellList[i].tmp_idx == int(_id))
			{
				if(getQuantityAt(i)<999)
				{
					ret=i;
					return ret;
				}
			}
		}
	}
	return ret;

}


bool InventoryClass::addFromIndex(const EtherRegistryClass& _eth, int entIndex)
{
	//adds an item to the inventory
	//first check to see if the item is stackable, if not, each of q times seeks a different slot
	//also we skip the match algo if the item is not stackable

	int plc=-1;//seeking fail case is -1
	bool quantityAdded=true;

	if(entIndex==0) return false;
	if(_eth.regEntities[entIndex]->type != ICAT_SUMMON) //add types that are not stackable
	{
		plc = getFirstMatch(_eth.regEntities[entIndex]->entityTemplateIndex);
		if(plc == -1) //if no first match is found, then look for the next empty slot
		{
			plc = getFirstEmpty();
			cellList[plc].tmp_idx = _eth.regEntities[entIndex]->entityTemplateIndex;
			quantityAdded=false;
		}
	}
	else
	{
		plc = getFirstEmpty();
		cellList[plc].tmp_idx = _eth.regEntities[entIndex]->entityTemplateIndex;
		quantityAdded=false;
	}
	//if there is no match and no free slot, we fail the addition
	if(plc<0) return false;
	cellList[plc].tmp_idx = _eth.regEntities[entIndex]->entityTemplateIndex;
	cellList[plc].l.items.push_back(entIndex);
	quantityAdded=true;

	//if it's made it this far, there is no failure and we return success
	return true;
}

bool InventoryClass::addEntityFromTemplate(const TemplateRegistryClass& _tmp, EtherRegistryClass& _eth, const char* _name)
{
	int newItem=_eth.createEntity(_tmp, _name);
	return addFromIndex(_eth, newItem);
}
bool InventoryClass::addSummonFromTemplate(const TemplateRegistryClass& _tmp, EtherRegistryClass& _eth, const char* _summon, const char* _contained)
{
	int newItem=_eth.createEntity(_tmp, _summon);
	if(_eth.regEntities[newItem]->type != ICAT_SUMMON) return false;
	int held=_eth.createEntity(_tmp, _contained);
	int summonPack = _eth.regEntities[newItem]->packIndex;
	_eth.regSummon[summonPack]->creatureContained=held;

	return addFromIndex(_eth, newItem);
}
