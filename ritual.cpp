//ritual.cpp
//feyleaf game studio
//by Benjamin C Watt (@feyleafgames)
//August 17, 2014
////////////////////////

#include "globals.h"

coord RitualClass::placeToPoint(int p)
{
	if(p==0) return coord(7,4);
	if(p==1) return coord(8,6);
	if(p==2) return coord(6,6);
	if(p==3) return coord(7,5);
	return coord(0,0);
}
bool RitualClass::addToRitual(int entityIndex)
{
	if(entityIndex<1) return false;
	if(cursor<slots)
	{
		cell[cursor].point=placeToPoint(cursor);
		cell[cursor].entityIndex=entityIndex;
		cursor++;
		return true;
	}
	return false;
}

void RitualClass::addSlotToInventory(int plc, const EtherRegistryClass& _eth, InventoryClass& _inv)
{
	//if(cell[plc].entityIndex<1) return;
	_inv.addFromIndex(_eth, cell[plc].entityIndex);
}

bool RitualClass::isThisRitual(const EtherRegistryClass& _eth, RecipeManager& _recipes)
{
	return (findRitual(_eth, _recipes)>0);
}

int RitualClass::findRitual(const EtherRegistryClass& _eth, RecipeManager& _recipes)
{
	int a, b, c;
	if(cursor<1) return 0;
	a=templateFromSlot(_eth, 0);
	b=templateFromSlot(_eth, 1);
	c=templateFromSlot(_eth, 2);
	recipeStruct t;

	for(int i=0; i<int(_recipes.cookbook.size()); i++)
	{
		t=_recipes.getRecipe(i);
		if(t.result>0)
		{
			if(
				(a==t.one && b==t.two && c==t.three) ||
				(a==t.one && b==t.three && c==t.two) ||
				(a==t.two && b==t.one && c==t.three) ||
				(a==t.two && b==t.three && c==t.one) ||
				(a==t.three && b==t.two && c==t.one) ||
				(a==t.three && b==t.one && c==t.two))
				return t.result;			
		}
	}

	return 0;
}

int RitualClass::templateFromSlot(const EtherRegistryClass& _eth, int p)
{
	if(p<slots && cell[p].entityIndex>0 && _eth.regEntities[cell[p].entityIndex] != NULL)
		return _eth.regEntities[cell[p].entityIndex]->entityTemplateIndex;
	else return 0;
}

RitualClass::RitualClass()
{
	clear();
}

RitualClass::~RitualClass()
{
	//cell.clear();
}