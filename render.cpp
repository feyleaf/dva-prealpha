/*
render.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
August 10, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#include "globals.h"

void RenderManager::initialize(TemplateRegistryClass& tmp, const settingStruct set)
{
	loadGraphicsFiles(set);
	createTileSheet(tmp, set);
	createEntitySheet(tmp, set);
	createGuiSheet(tmp, set);
}
void RenderManager::createTileSheet(TemplateRegistryClass& tmp, const settingStruct set)
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
		tmp.container.tileList[i].origin=coord(0, row);
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
	spriteSheet.getTexture().copyToImage().saveToFile(set.tileSheetFile);
}

void RenderManager::createEntitySheet(TemplateRegistryClass& tmp, const settingStruct set)
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
		tmp.container.entityList[i].origin = coord(0,row);
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
	spriteSheet.getTexture().copyToImage().saveToFile(set.entitySheetFile);
}

void RenderManager::createGuiSheet(TemplateRegistryClass& tmp, const settingStruct set)
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
		tmp.container.buttonList[i].origin = coord(0,row);
		for(int c=0; c<tmp.container.buttonList[i].iconRange; c++)
		{
			col=c*tmp.container.buttonList[i].dimensions.x;
			tempTexture.loadFromFile("images/" + std::string(tmp.container.buttonList[i].spritefile), 
				sf::IntRect(c*tmp.container.buttonList[i].dimensions.x, 0, tmp.container.buttonList[i].dimensions.x, tmp.container.buttonList[i].dimensions.y));
			tempSprite.setTexture(tempTexture);
			tempSprite.setTextureRect(sf::IntRect(c*tmp.container.buttonList[i].dimensions.x, 0, tmp.container.buttonList[i].dimensions.x, tmp.container.buttonList[i].dimensions.y));
			//tempSprite.setColor(sf::Color(255,255,255));
			tempSprite.setPosition(float(col), float(row));
			spriteSheet.draw(tempSprite);			
		}
	}
	spriteSheet.display();
	spriteSheet.getTexture().copyToImage().saveToFile(set.guiSheetFile);
}

void RenderManager::loadGraphicsFiles(settingStruct set)
{
	mainfont.loadFromFile(set.mainFontFile);
	auxfont.loadFromFile(set.auxFontFile);
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

void RenderManager::ColorizeMiniMap(registeredTile* obj, coord place, sf::Color tint)
{
	coord o=obj->origin;
	int frameskip=obj->frame*obj->dimensions.x;
	obj->minimapColor = tileSheet.copyToImage().getPixel(o.x+frameskip, o.y);
}

void RenderManager::DrawTile(sf::RenderWindow& win, registeredTile* obj, coord place, sf::Color tint)
{
	currentSprite.setTexture(tileSheet);
	coord o=obj->origin;
	int frameskip=obj->frame*obj->dimensions.x;
	currentSprite.setTextureRect(sf::IntRect(o.x+frameskip, o.y, obj->dimensions.x, obj->dimensions.y));
	currentSprite.setColor(tint);

	currentSprite.setPosition(float((place.x*32)), float((place.y*32))+16.0f);
	
	win.draw(currentSprite);
}

void RenderManager::DrawQuickMap(sf::RenderWindow& win, GameObjectContainerClass& obj, coord worldCoord, coord centerPos, bool highlight)
{
	currentSprite.setTexture(obj.mapSheet);
	currentSprite.setTextureRect(sf::IntRect(0,0,24,18));
	//if we center on x,y then the rendered position is x+wpx, y+wpy
	sf::Vector2f outputPos = toVector(centerPos-worldCoord);
	outputPos.x*=32*24;
	outputPos.y*=32*18;
	currentSprite.setPosition(outputPos);
	currentSprite.setScale(32.0f, 32.0f);
	currentSprite.setColor(sf::Color::White);
	if(highlight)
		currentSprite.setColor(sf::Color::Red);
	win.setView(viewport);
	win.draw(currentSprite);
	currentSprite.setScale(1.0f, 1.0f);
	win.setView(win.getDefaultView());
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

void RenderManager::DrawRituals(sf::RenderWindow& win, const TemplateRegistryClass& tmp, RitualClass& theRitual)
{
	currentSprite.setTexture(entitySheet);
	for(int i=0; i<int(theRitual.cell.size()); i++)
	{
		currentSprite.setTextureRect(sf::IntRect(0,tmp.container.entityList[theRitual.cell[i].templateIndex].origin.y, tmp.container.entityList[theRitual.cell[i].templateIndex].dimensions.x, tmp.container.entityList[theRitual.cell[i].templateIndex].dimensions.y));
		currentSprite.setColor(sf::Color::White);
		currentSprite.setPosition(float((theRitual.cell[i].point.x*32)), float((theRitual.cell[i].point.y*32)+16));
	
		win.draw(currentSprite);
	}
}

void RenderManager::DrawGUIForm(sf::RenderWindow& win, const TemplateRegistryClass& tmp, GUIFormClass& form, coord _mouse_pixel)
{
	for(int i=1; i<int(form.cells.size()); i++)
	{
		switch(form.cells[i].renderType)
		{
			case RENDER_TILE:
				currentSprite.setTexture(tileSheet);
				currentSprite.setTextureRect(sf::IntRect(0,tmp.container.tileList[form.cells[i].templateIndex].origin.y, tmp.container.tileList[form.cells[i].templateIndex].dimensions.x, tmp.container.tileList[form.cells[i].templateIndex].dimensions.y));
				break;
			case RENDER_ENTITY:
				currentSprite.setTexture(entitySheet);
				currentSprite.setTextureRect(sf::IntRect(0,tmp.container.entityList[form.cells[i].templateIndex].origin.y, tmp.container.entityList[form.cells[i].templateIndex].dimensions.x, tmp.container.entityList[form.cells[i].templateIndex].dimensions.y));
				break;
			case RENDER_BUTTON:
			default:
				currentSprite.setTexture(guiSheet);
				currentSprite.setTextureRect(sf::IntRect(0,tmp.container.buttonList[form.cells[i].templateIndex].origin.y, tmp.container.buttonList[form.cells[i].templateIndex].dimensions.x, tmp.container.buttonList[form.cells[i].templateIndex].dimensions.y));
				break;
		}
		if(isCollision(toVector(_mouse_pixel), sf::IntRect(form.cells[i].pixel.x, form.cells[i].pixel.y, 32, 32)))
			currentSprite.setColor(sf::Color::Yellow);
		else currentSprite.setColor(sf::Color::White);
		currentSprite.setPosition(float(form.cells[i].pixel.x), float(form.cells[i].pixel.y));

		win.draw(currentSprite);
	}
}

void RenderManager::DrawInventory(sf::RenderWindow& win, InventoryClass& items, const buttonStruct* cell)
{
	char out[8];
	sf::Text quantityOut;
	quantityOut.setCharacterSize(16);
	quantityOut.setFont(auxfont);
	quantityOut.setColor(sf::Color::White);
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
			int index=((x-items.tl.x)+((y-items.tl.y)*items.dimensions.x));
	
			held=items.cellList[index].idx_item;
			if(items.reg.entities[held] != NULL)
			{
				o=items.reg.entities[held]->origin;
				currentSprite.setTextureRect(sf::IntRect(o.x, o.y, 32, 32));
				currentSprite.setPosition(float((x*32)), float((y*32))+16.0f);
				currentSprite.setColor(sf::Color::White);
	
				win.draw(currentSprite);

				if(items.getQuantityAt(index) > 1)
				{
					sprintf_s(out, "%5i", items.getQuantityAt(index));
					quantityOut.setString(out);
					quantityOut.setPosition(float((x*32)), float((y*32))+33.0f);
					win.draw(quantityOut);
				}
			}
		}
	}
}
