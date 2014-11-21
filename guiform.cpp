//guiform.cpp
//feyleaf game studio
//by Benjamin C Watt (@feyleafgames)
//August 19, 2014
////////////////////////

#include "globals.h"

void GUIFormClass::setBackground(int _type, int _tmp, coord _pixel)
{
	back.renderType=_type;
	back.templateIndex=_tmp;
	back.pixel=_pixel;
	back.buttonIndex = 0;
}

void GUIFormClass::addCell(int _type, int _tmp, coord _pixel)
{
	guiCell dummy;
	dummy.renderType=_type;
	dummy.hidden=false;
	dummy.templateIndex=_tmp;
	dummy.pixel=_pixel;
	dummy.buttonIndex = 0;
	cells.push_back(dummy);
}

void GUIFormClass::addHiddenCell(int _type, int _tmp, coord _pixel)
{
	guiCell dummy;
	dummy.renderType=_type;
	dummy.hidden=true;
	dummy.templateIndex=_tmp;
	dummy.pixel=_pixel;
	dummy.buttonIndex = 0;
	cells.push_back(dummy);
}

void GUIFormClass::addWords(const char* message, int _buttonID, coord _pixel)
{
	guiText dummy;
	strcpy_s(dummy.msg, message);
	dummy.pixel=_pixel;
	dummy.buttonIndex = _buttonID;
	words.push_back(dummy);
}