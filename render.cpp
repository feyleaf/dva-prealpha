/*
render.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
July 17, 2014
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

void RenderManager::createEntitySheet(const TemplateRegistryClass& tmp)
{
	int iconTotalWidth=0;
	int rowTotalHeight=0;
	for(int i=1; i<int(tmp.container.entityList.size()); i++)
	{
		rowTotalHeight+=tmp.container.entityList[i].dimensions.y;
		if(iconTotalWidth<(tmp.container.entityList[i].iconRange*tmp.container.entityList[i].dimensions.x))
			iconTotalWidth=tmp.container.entityList[i].iconRange*tmp.container.entityList[i].dimensions.x;
	}

	int width=iconTotalWidth;
	int height=rowTotalHeight;
	sf::RenderTexture spriteSheet;
	sf::Texture tempTexture;
	sf::Sprite tempSprite;

	spriteSheet.create(width, height);
	spriteSheet.clear(sf::Color(255,255,255,0));
	int count=0;
	int row=0; int col=0;
	for(int i=1; i<int(tmp.container.entityList.size()); i++)
	{
		row+=tmp.container.entityList[i-1].dimensions.y;
		for(int c=0; c<tmp.container.entityList[i].iconRange; c++)
		{
			col=c*tmp.container.entityList[i].dimensions.x;
			tempTexture.loadFromFile("images/" + std::string(tmp.container.entityList[i].spritefile), 
				sf::IntRect(c*tmp.container.entityList[i].dimensions.x, 0, tmp.container.entityList[i].dimensions.x, tmp.container.entityList[i].dimensions.y));
			tempSprite.setTexture(tempTexture);
			//tempSprite.setColor(sf::Color(255,255,255));
			tempSprite.setPosition(float(col), float(row));
			spriteSheet.draw(tempSprite);			
		}
	}
	spriteSheet.display();
	spriteSheet.getTexture().copyToImage().saveToFile("entities_temp.png");
}

void RenderManager::createGuiSheet(const TemplateRegistryClass& tmp)
{
	int iconTotalWidth=0;
	int rowTotalHeight=0;
	for(int i=1; i<int(tmp.container.buttonList.size()); i++)
	{
		rowTotalHeight+=tmp.container.buttonList[i].dimensions.y;
		if(iconTotalWidth<(tmp.container.buttonList[i].iconRange*tmp.container.buttonList[i].dimensions.x))
			iconTotalWidth=tmp.container.buttonList[i].iconRange*tmp.container.buttonList[i].dimensions.x;
	}

	int width=iconTotalWidth;
	int height=rowTotalHeight;
	sf::RenderTexture spriteSheet;
	sf::Texture tempTexture;
	sf::Sprite tempSprite;

	spriteSheet.create(width, height);
	spriteSheet.clear(sf::Color(255,255,255,0));
	int count=0;
	int row=0; int col=0;
	for(int i=1; i<int(tmp.container.buttonList.size()); i++)
	{
		row+=tmp.container.buttonList[i-1].dimensions.y;
		for(int c=0; c<tmp.container.buttonList[i].iconRange; c++)
		{
			col=c*tmp.container.buttonList[i].dimensions.x;
			tempTexture.loadFromFile("images/" + std::string(tmp.container.buttonList[i].spritefile), 
				sf::IntRect(c*tmp.container.buttonList[i].dimensions.x, 0, tmp.container.buttonList[i].dimensions.x, tmp.container.buttonList[i].dimensions.y));
			tempSprite.setTexture(tempTexture);
			//tempSprite.setColor(sf::Color(255,255,255));
			tempSprite.setPosition(float(col), float(row));
			spriteSheet.draw(tempSprite);			
		}
	}
	spriteSheet.display();
	spriteSheet.getTexture().copyToImage().saveToFile("gui_temp.png");
}

void RenderManager::loadGraphicsFiles(settingStruct set)
{
	font.loadFromFile(set.mainFontFile);
	entitySheet.setSmooth(false);
	tileSheet.setSmooth(false);
	guiSheet.setSmooth(false);
	entitySheet.loadFromFile("entities_temp.png");
	tileSheet.loadFromFile("tiles_temp.png");
	guiSheet.loadFromFile("gui_temp.png");
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
	currentSprite.setColor(tint);

	currentSprite.setPosition(float((place.x*32)), float((place.y*32))+16.0f);
	
	win.draw(currentSprite);
}

void RenderManager::DrawEntity(sf::RenderWindow& win, const registeredEntity* obj, coord worldpixel, bool highlight)
{
	if(obj==NULL || obj->plane>0) return;
	currentSprite.setTexture(entitySheet);
	coord o=obj->origin;
	int frameskip=obj->frame*obj->dimensions.x;
	currentSprite.setTextureRect(sf::IntRect(o.x+frameskip, o.y, obj->dimensions.x, obj->dimensions.y));
	if(highlight)
	{
		if(obj->isEnemy)
			currentSprite.setColor(sf::Color::Cyan);
		else
			currentSprite.setColor(sf::Color::Magenta);
	}
	else currentSprite.setColor(sf::Color::White);
	currentSprite.setPosition(float((worldpixel.x)), float((worldpixel.y)));
	
	win.draw(currentSprite);
}

void RenderManager::DrawGui(sf::RenderWindow& win, const buttonStruct* obj, coord place, bool hover)
{
	currentSprite.setTexture(guiSheet);
	coord o=obj->origin;
	int frameskip=obj->frame*obj->dimensions.x;
	currentSprite.setTextureRect(sf::IntRect(o.x+frameskip, o.y, obj->dimensions.x, obj->dimensions.y));
	currentSprite.setPosition(float((place.x*32)), float((place.y*32))+16.0f);
	if(hover)
		currentSprite.setColor(sf::Color::Yellow);
	else
		currentSprite.setColor(sf::Color::White);

	
	if(obj->active) win.draw(currentSprite);
}

void RenderManager::DrawInventory(sf::RenderWindow& win, const GameObjectContainerClass& _obj, const InventoryClass& items, const buttonStruct* cell)
{
	int held=0;
	currentSprite.setTexture(guiSheet);
	coord o=cell->origin;
	currentSprite.setTextureRect(sf::IntRect(o.x, o.y, cell->dimensions.x, cell->dimensions.y));
	for(int y=items.tl.y; y<items.tl.y+items.dimensions.y; y++)
	{
		for(int x=items.tl.x; x<items.tl.x+items.dimensions.x; x++)
		{
			currentSprite.setPosition(float((x*32)), float((y*32))+16.0f);
			if(items.cursor==((x-items.tl.x)+((y-items.tl.y)*items.dimensions.x))) currentSprite.setColor(sf::Color::Green);
			else currentSprite.setColor(sf::Color::White);
	
			win.draw(currentSprite);
		}
	}
	currentSprite.setTexture(entitySheet);
	for(int y=items.tl.y; y<items.tl.y+items.dimensions.y; y++)
	{
		for(int x=items.tl.x; x<items.tl.x+items.dimensions.x; x++)
		{
	
			held=items.cellList[((x-items.tl.x)+((y-items.tl.y)*items.dimensions.x))].idx_item;
			if((_obj.regEntities[held] != NULL) && _obj.regEntities[held]->plane==1)
			{
				o=_obj.regEntities[held]->origin;
				currentSprite.setTextureRect(sf::IntRect(o.x, o.y, 32, 32));
				currentSprite.setPosition(float((x*32)), float((y*32))+16.0f);
				currentSprite.setColor(sf::Color::White);
	
				win.draw(currentSprite);
			}
		}
	}
<<<<<<< HEAD
}
=======
}
>>>>>>> parent of fc3a278... Map Scaling
