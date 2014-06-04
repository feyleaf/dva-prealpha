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
	int totalSprites=0;
	for(int i=1; i<int(tmp.container.tileList.size()); i++)
	{
		totalSprites+=tmp.container.tileList[i].iconRange;
	}

	int width=32*8;
	int height=32*int((totalSprites/8)+1);
	sf::RenderTexture spriteSheet;
	sf::Texture tempTexture;
	sf::Sprite tempSprite;

	spriteSheet.create(width, height);
	int count=0;
	int row=0; int col=0;
	for(int i=1; i<int(tmp.container.tileList.size()); i++)
	{
		for(int c=0; c<tmp.container.tileList[i].iconRange; c++)
		{
			count++;
			row=int(count/8);
			col=count%8;
			tempTexture.loadFromFile("images/" + std::string(tmp.container.tileList[i].spritefile), 
				sf::IntRect(c*32, 0, 32, 32));
			tempSprite.setTexture(tempTexture);
			tempSprite.setColor(sf::Color(255,255,255,255));
			tempSprite.setPosition(float(col*32), float(row*32));
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
	currentSprite.setTextureRect(obj->txRect);
	//currentSprite.setColor(sf::Color::White);//obj->curColor);

	currentSprite.setPosition(float((place.x*32)), float((place.y*32)));
	
	win.draw(currentSprite);
}

void RenderManager::DrawEntity(sf::RenderWindow& win, const registeredEntity* obj, coord place)
{
	currentSprite.setColor(sf::Color::White);

	currentSprite.setPosition(float((place.x*32)), float((place.y*32)));
	
	win.draw(currentSprite);
}