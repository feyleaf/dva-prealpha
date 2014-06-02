/*
render.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 19, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#include "globals.h"

void RenderManager::loadGraphicsFiles(settingStruct set)
{
	font.loadFromFile(set.mainFontFile);
	entitySheet.setSmooth(false);
	tileSheet.setSmooth(false);
	guiSheet.setSmooth(false);
	entitySheet.loadFromFile(set.entitySheetFile);
	tileSheet.loadFromFile(set.tileSheetFile);
	guiSheet.loadFromFile(set.guiSheetFile);
}

sf::IntRect RenderManager::rectFromOrigin(unsigned char _origin, int _wid, int _hig)
{
	coord subt;
	subt.x=_origin%8;
	subt.y=int(_origin/8);
	return sf::IntRect(subt.x*_wid, subt.y*_hig, _wid, _hig);

}

void RenderManager::DrawTile(sf::RenderWindow& win, const registeredTile* obj, coord place, sf::Color tint, int con, long sd)
{
	currentSprite.setTexture(obj->tx);
	currentSprite.setColor(sf::Color::White);//obj->curColor);

	currentSprite.setPosition(float((place.x*32)), float((place.y*32)));
	
	win.draw(currentSprite);
}

void RenderManager::DrawEntity(sf::RenderWindow& win, const registeredEntity* obj, coord place)
{
	currentSprite.setTexture(obj->tx);
	currentSprite.setColor(sf::Color::White);

	currentSprite.setPosition(float((place.x*32)), float((place.y*32)));
	
	win.draw(currentSprite);
}