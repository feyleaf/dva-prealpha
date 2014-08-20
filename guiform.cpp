//guiform.cpp
//feyleaf game studio
//by Benjamin C Watt (@feyleafgames)
//August 19, 2014
////////////////////////

#include "globals.h"

void GUIFormClass::addCell(int _type, int _tmp, coord _grid)
{
	guiCell dummy;
	dummy.renderType=_type;
	dummy.templateIndex=_tmp;
	dummy.grid=_grid;
	cells.push_back(dummy);
}