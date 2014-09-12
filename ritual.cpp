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
bool RitualClass::addToRitual(int templateIndex)
{
	if(templateIndex<1) return false;
	if(slots>int(cell.size()))
	{
		ritualCell dummy;
		dummy.point=placeToPoint(cell.size());
		dummy.templateIndex=templateIndex;
		cell.push_back(dummy);
		return true;
	}
	return false;
}

void RitualClass::addSlotToInventory(int plc, const TemplateRegistryClass& tmp, InventoryClass& _inv)
{
	_inv.add(tmp, cell[plc].templateIndex);
}

bool RitualClass::isThisRitual(RecipeManager& _recipes)
{
	return (findRitual(_recipes)>0);
}

int RitualClass::findRitual(RecipeManager& _recipes)
{
	int a, b, c;
	if(cell.size()<1) return 0;
	else if(cell.size()<2) {a=cell[0].templateIndex; b=0; c=0;}
	else if(cell.size()<3) {a=cell[0].templateIndex; b=cell[1].templateIndex; c=0;}
	else {a=cell[0].templateIndex; b=cell[1].templateIndex; c=cell[2].templateIndex;}
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

int RitualClass::templateFromSlot(int p)
{
	if(p>=0 && p<=slots && p<int(cell.size()))
		return cell[p].templateIndex;
	else return 0;
}

RitualClass::RitualClass()
{
	cell.clear();
	slots=4;
}

RitualClass::~RitualClass()
{
}