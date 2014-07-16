#include "globals.h"

InventoryClass::InventoryClass()
{
	tl=coord(24,7);
	dimensions=coord(5,5);
	capacity=dimensions.x*dimensions.y;
	cursor=1;
	for(int i=0; i<capacity; i++) cellList.push_back(cellStruct());
}

int InventoryClass::select(coord click)
{
	coord _pt=click-tl;
	if(_pt.x<0 || _pt.y<0 || _pt.x>dimensions.x || _pt.y>dimensions.y)
	{
		return cursor;
	}
	else
	{
		cursor=_pt.x+(_pt.y*dimensions.x);
		return cursor;
	}
}
bool InventoryClass::clearSlot(int plc)
{
	cellList[plc].tmp_idx=0;
	cellList[plc].idx_item=0;
	cellList[plc].quantity=0;
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
	temp.idx_item = cellList[plcA].idx_item;
	temp.quantity = cellList[plcA].quantity;

	cellList[plcA].tmp_idx = cellList[plcB].tmp_idx;
	cellList[plcA].idx_item = cellList[plcB].idx_item;
	cellList[plcA].quantity = cellList[plcB].quantity;

	cellList[plcB].tmp_idx = temp.tmp_idx;
	cellList[plcB].idx_item = temp.idx_item;
	cellList[plcB].quantity = temp.quantity;

	return true;
}


bool InventoryClass::isAllSlotsFilled()
{
	for(int i=0; i<capacity; i++)
	{
		if(cellList[i].quantity==0) return false;
	}
	return true;
}

bool InventoryClass::isInventoryEmpty()
{
	for(int i=0; i<capacity; i++)
	{
		if(cellList[i].quantity>0) return false;
	}
	return true;
}

bool InventoryClass::isSlotEmpty()
{
	return (cellList[cursor].quantity==0);
}

unsigned int InventoryClass::drop(int plc)
{
	if(cellList[plc].quantity==0)
	{
		cellList[plc].tmp_idx=0;
		cellList[plc].idx_item=0;
		return 0;
	}
	if(cellList[plc].quantity==1)
	{
		unsigned int temp=cellList[plc].idx_item;
		cellList[plc].idx_item=0;
		cellList[plc].quantity=0;
		return temp;
	}
	cellList[plc].quantity-=1;
	return cellList[plc].idx_item;
}

unsigned int InventoryClass::getItemAt(int plc)
{
	return cellList[plc].idx_item;
}

unsigned int InventoryClass::getItemAtCursor()
{
	return getItemAt(cursor);
}

short InventoryClass::getQuantityAt(int plc)
{
	if(plc>capacity || plc<0) return 0;
	return cellList[plc].quantity;
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
		if(cellList[i].quantity == 0)
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
				if(cellList[i].quantity<999)
				{
					ret=i;
					return ret;
				}
			}
		}
	}
	return ret;

}


bool InventoryClass::add(registeredEntity* ent, int entIndex, short q)
{
	//adds an item to the inventory
	//first check to see if the item is stackable, if not, each of q times seeks a different slot
	//also we skip the match algo if the item is not stackable
	int plc=-1;//seeking fail case is -1
	bool quantityAdded=true;

	for(int i=0; i<q; i++)
	{
		if(ent->type != ICAT_CREATURE && ent->type != ICAT_SUMMON) //add types that are not stackable
		{
			plc = getFirstMatch(ent->entityTemplateIndex);
			if(plc == -1) //if no first match is found, then look for the next empty slot
			{
				plc = getFirstEmpty();
				quantityAdded=false;
			}
		}
		else
		{
			plc = getFirstEmpty();
			quantityAdded=false;
		}
		//if there is no match and no free slot, we fail the addition
		if(plc == -1) return false;
		if(quantityAdded)
		{
			cellList[plc].quantity+=1;
		}
		else
		{
			cellList[plc].tmp_idx = ent->entityTemplateIndex;
			cellList[plc].idx_item = entIndex;
			cellList[plc].quantity = 1;
		}
		quantityAdded=true;
	}

	//if it's made it this far, there is no failure and we return success
	return true;
}

