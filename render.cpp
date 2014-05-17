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
void RenderManager::DrawTile(sf::RenderWindow& win, const tileObjectStruct* obj, coord place, sf::Color tint, int con, long sd)
{
	currentSprite.setTexture(tileSheet);
	currentSprite.setColor(obj->curColor);

	currentSprite.setTextureRect(rectFromOrigin(obj->tmp.sheetOrigin, 32, 32)); //TODO: fix this with settings
	currentSprite.setPosition(float((place.x*32)), float((place.y*32)));
	
	win.draw(currentSprite);
}