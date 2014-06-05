/*
render.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 19, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#include "globals.h"

void RenderManager::createTileSheet(const TemplateRegistryClass& tmp)
{
	int iconTotalWidth=0;
	int rowTotalHeight=0;
	for(int i=1; i<int(tmp.container.tileList.size()); i++)
	{
		rowTotalHeight+=tmp.container.tileList[i].dimensions.y;
		if(iconTotalWidth<(tmp.container.tileList[i].iconRange*tmp.container.tileList[i].dimensions.x))
			iconTotalWidth=tmp.container.tileList[i].iconRange*tmp.container.tileList[i].dimensions.x;
	}

	int width=iconTotalWidth;
	int height=rowTotalHeight;
	sf::RenderTexture spriteSheet;
	sf::Texture tempTexture;
	sf::Sprite tempSprite;

	spriteSheet.create(width, height);
	int count=0;
	int row=0; int col=0;
	for(int i=1; i<int(tmp.container.tileList.size()); i++)
	{
		row+=tmp.container.tileList[i-1].dimensions.y;
		for(int c=0; c<tmp.container.tileList[i].iconRange; c++)
		{
			col=c*tmp.container.tileList[i].dimensions.x;
			tempTexture.loadFromFile("images/" + std::string(tmp.container.tileList[i].spritefile), 
				sf::IntRect(c*tmp.container.tileList[i].dimensions.x, 0, tmp.container.tileList[i].dimensions.x, tmp.container.tileList[i].dimensions.y));
			tempSprite.setTexture(tempTexture);
			tempSprite.setColor(sf::Color(255,255,255,255));
			tempSprite.setPosition(float(col), float(row));
			spriteSheet.draw(tempSprite);			
		}
	}
	spriteSheet.display();
	spriteSheet.getTexture().copyToImage().saveToFile("tiles_temp.png");
}

void RenderManager::loadGraphicsFiles(settingStruct set)
{
	font.loadFromFile(set.mainFontFile);
	entitySheet.setSmooth(false);
	tileSheet.setSmooth(false);
	guiSheet.setSmooth(false);
	entitySheet.loadFromFile(set.entitySheetFile);
	tileSheet.loadFromFile("tiles_temp.png");
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
	currentSprite.setTexture(tileSheet);
	sf::Vector2i o=sf::Vector2i(toVector(obj->origin));
	int frameskip=obj->frame*obj->dimensions.x;
	currentSprite.setTextureRect(sf::IntRect(o.x+frameskip, o.y, obj->dimensions.x, obj->dimensions.y));
	currentSprite.setColor(obj->distortionColor);

	currentSprite.setPosition(float((place.x*32)), float((place.y*32)));
	
	win.draw(currentSprite);
}

void RenderManager::DrawEntity(sf::RenderWindow& win, const registeredEntity* obj, coord place)
{
	currentSprite.setColor(sf::Color::White);

	currentSprite.setPosition(float((place.x*32)), float((place.y*32)));
	
	win.draw(currentSprite);
}