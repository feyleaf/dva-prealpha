#include "globals.h"

InventoryClass::InventoryClass()
{
	tl=coord(24,7);
	dimensions=coord(5,5);
	capacity=dimensions.x*dimensions.y;
	cursor=1;
}

int InventoryClass::select(coord click)
{
	coord _pt=click-tl;
	if(_pt.x>dimensions.x || _pt.y>dimensions.y)
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
	if(cellList[plc].ptr_item != NULL) delete cellList[plc].ptr_item;
	cellList[plc].ptr_item=NULL;
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
	temp.ptr_item = cellList[plcA].ptr_item;
	temp.quantity = cellList[plcA].quantity;

	cellList[plcA].ptr_item = cellList[plcB].ptr_item;
	cellList[plcA].quantity = cellList[plcB].quantity;

	cellList[plcB].ptr_item = temp.ptr_item;
	cellList[plcB].quantity = temp.quantity;

	temp.ptr_item=NULL;
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

registeredEntity* InventoryClass::drop(int plc)
{
	if(cellList[plc].quantity==0)
	{
		cellList[plc].ptr_item=NULL;
		return NULL;
	}
	if(cellList[plc].quantity==1)
	{
		registeredEntity* temp=cellList[plc].ptr_item;
		cellList[plc].ptr_item=NULL;
		cellList[plc].quantity=0;
		return temp;
	}
	cellList[plc].quantity-=1;
	return cellList[plc].ptr_item;
}

registeredEntity* InventoryClass::getItemAt(int plc)
{
	return cellList[plc].ptr_item;
}

registeredEntity* InventoryClass::getItemAtCursor()
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
		if(cellList[i].ptr_item!=NULL)
		{		
			if(cellList[i].ptr_item->entityTemplateIndex == int(_id))
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


bool InventoryClass::add(registeredEntity* it, short q)
{
	//adds an item to the inventory
	//first check to see if the item is stackable, if not, each of q times seeks a different slot
	//also we skip the match algo if the item is not stackable
	int plc=-1;//seeking fail case is -1
	bool quantityAdded=true;

	for(int i=0; i<q; i++)
	{
		if(it->type != ICAT_CREATURE) //add types that are not stackable
		{
			plc = getFirstMatch(it->entityTemplateIndex);
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
			cellList[plc].ptr_item = it;
			cellList[plc].quantity = 1;
		}
		quantityAdded=true;
	}

	//if it's made it this far, there is no failure and we return success
	return true;
}

