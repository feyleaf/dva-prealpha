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
				sf::IntRect(col, 0, tmp.container.entityList[i].dimensions.x, tmp.container.entityList[i].dimensions.y));
			tempSprite.setTexture(tempTexture);
			tempSprite.setTextureRect(sf::IntRect(0, 0, tmp.container.entityList[i].dimensions.x, tmp.container.entityList[i].dimensions.y));
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
	header1.loadFromFile(set.auxFontFile);
	header2.loadFromFile(set.auxFontFile);
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

void RenderManager::DrawEntity(sf::RenderWindow& win, const registeredEntity* obj, coord worldpixel, bool highlight, bool ghost)
{
	if(obj==NULL || obj->plane>0) return;
	currentSprite.setTexture(entitySheet);
	coord o=obj->origin;
	int frameskip=obj->frame*obj->dimensions.x;
	currentSprite.setTextureRect(sf::IntRect(o.x+frameskip, o.y, obj->dimensions.x, obj->dimensions.y));
	currentSprite.setColor(sf::Color::White);
	if(highlight || ghost)
	{
		if(highlight)
		{
			if(obj->isEnemy)
				currentSprite.setColor(sf::Color::Cyan);
			else
				currentSprite.setColor(sf::Color::Magenta);
		}
		if(ghost)
		{
			sf::Color ghostColor = sf::Color(currentSprite.getColor().r, currentSprite.getColor().g, currentSprite.getColor().b, 80);
			currentSprite.setColor(ghostColor);
		}
	}
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

	
	if(obj->active) 
		win.draw(currentSprite);
}

void RenderManager::DrawRituals(sf::RenderWindow& win, const TemplateRegistryClass& tmp, RitualClass& theRitual)
{
	currentSprite.setTexture(entitySheet);
	for(int i=0; i<int(theRitual.cell.size()); i++)
	{
		currentSprite.setTextureRect(sf::IntRect(0,tmp.container.entityList[theRitual.cell[i].entityIndex].origin.y, tmp.container.entityList[theRitual.cell[i].entityIndex].dimensions.x, tmp.container.entityList[theRitual.cell[i].entityIndex].dimensions.y));
		currentSprite.setColor(sf::Color::White);
		currentSprite.setPosition(float((theRitual.cell[i].point.x*32)), float((theRitual.cell[i].point.y*32)+16));
	
		win.draw(currentSprite);
	}
}

void RenderManager::DrawGUIForm(sf::RenderWindow& win, const TemplateRegistryClass& tmp, GUIFormClass& form, coord _mouse_pixel)
{
	if(form.back.templateIndex>0)
	{
		currentSprite.setTexture(guiSheet);
		currentSprite.setTextureRect(sf::IntRect(0,tmp.container.buttonList[form.back.templateIndex].origin.y, tmp.container.buttonList[form.back.templateIndex].dimensions.x, tmp.container.buttonList[form.back.templateIndex].dimensions.y));
		currentSprite.setColor(sf::Color::White);
		currentSprite.setPosition(float(form.back.pixel.x), float(form.back.pixel.y));

		win.draw(currentSprite);
	}
	for(int i=1; i<int(form.cells.size()); i++)
	{
		if(!form.cells[i].hidden)
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
}

void RenderManager::DrawInventory(sf::RenderWindow& win, const TemplateRegistryClass& tmp, InventoryClass& items, GUIFormClass& form)
{
	if(!form.active) return;
	int highlight=-1;
	int held=-1;
	bool skipDraw=false;
	char out[8];
	sf::Text quantityOut;
	quantityOut.setCharacterSize(16);
	quantityOut.setFont(auxfont);
	quantityOut.setColor(sf::Color::White);
	sf::Text guiText;
	guiText.setCharacterSize(24);
	guiText.setFont(mainfont);
	guiText.setColor(sf::Color::White);
	for(int i=1; i<int(form.cells.size()); i++)
	{
		switch(form.cells[i].renderType)
		{
			case RENDER_TILE:
				currentSprite.setTexture(tileSheet);
				currentSprite.setTextureRect(sf::IntRect(0,tmp.container.tileList[form.cells[i].templateIndex].origin.y, tmp.container.tileList[form.cells[i].templateIndex].dimensions.x, tmp.container.tileList[form.cells[i].templateIndex].dimensions.y));
				break;
			case RENDER_ENTITY:
				held++;
				skipDraw=true;
				if(items.getQuantityAt(held) > 0)
				{
					skipDraw=false;
					currentSprite.setTexture(entitySheet);
					currentSprite.setTextureRect(sf::IntRect(0,tmp.container.entityList[form.cells[i].templateIndex].origin.y, tmp.container.entityList[form.cells[i].templateIndex].dimensions.x, tmp.container.entityList[form.cells[i].templateIndex].dimensions.y));
				}
				break;
			case RENDER_BUTTON:
				highlight++;
			default:
				currentSprite.setTexture(guiSheet);
				currentSprite.setTextureRect(sf::IntRect(0,tmp.container.buttonList[form.cells[i].templateIndex].origin.y, tmp.container.buttonList[form.cells[i].templateIndex].dimensions.x, tmp.container.buttonList[form.cells[i].templateIndex].dimensions.y));
				break;
		}
		if(items.cursor==highlight)
			currentSprite.setColor(sf::Color::Green);
		else currentSprite.setColor(sf::Color::White);
		currentSprite.setPosition(float(form.cells[i].pixel.x), float(form.cells[i].pixel.y)+16);

		if(!skipDraw) win.draw(currentSprite);
		skipDraw=false;
	}
	for(int j=0; j<items.capacity; j++)
	{
		if(items.getQuantityAt(j) > 1)
		{
			sprintf_s(out, "%5i", items.getQuantityAt(j));
			quantityOut.setString(out);
			sf::Vector2f defaultpos(float(j%5)*32.0f, float((int(j/5))*32.0f)+31.0f);
			sf::Vector2f offset(toVector(coord(24*32, 12*32)));
			quantityOut.setPosition(defaultpos+offset);
			win.draw(quantityOut);
		}
	}
	for(int j=0; j<int(form.words.size()); j++)
	{
		guiText.setString(form.words[j].msg);
		sf::Vector2f defaultpos(float(form.words[j].pixel.x), float(form.words[j].pixel.y));
		sf::Vector2f offset(toVector(coord(24*32, 12*32)));
		guiText.setPosition(defaultpos+offset);
		win.draw(guiText);
	}
}

void RenderManager::DrawRewards(sf::RenderWindow& win, const TemplateRegistryClass& tmp, InventoryClass& items, GUIFormClass& form)
{
	if(!form.active) return;
	int highlight=-1;
	int held=-1;
	bool skipDraw=false;
	char out[8];
	sf::Text quantityOut;
	quantityOut.setCharacterSize(16);
	quantityOut.setFont(auxfont);
	quantityOut.setColor(sf::Color::White);
	sf::Text guiText;
	guiText.setCharacterSize(24);
	guiText.setFont(mainfont);
	guiText.setColor(sf::Color::White);
	if(form.back.templateIndex>0)
	{
		currentSprite.setTexture(guiSheet);
		currentSprite.setTextureRect(sf::IntRect(0,tmp.container.buttonList[form.back.templateIndex].origin.y, tmp.container.buttonList[form.back.templateIndex].dimensions.x, tmp.container.buttonList[form.back.templateIndex].dimensions.y));
		currentSprite.setColor(sf::Color::White);
		currentSprite.setPosition(float(form.back.pixel.x), float(form.back.pixel.y));

		win.draw(currentSprite);
	}
	for(int i=1; i<int(form.cells.size()); i++)
	{
		currentSprite.setColor(sf::Color::White);
		switch(form.cells[i].renderType)
		{
			case RENDER_TILE:
				currentSprite.setTexture(tileSheet);
				currentSprite.setTextureRect(sf::IntRect(0,tmp.container.tileList[form.cells[i].templateIndex].origin.y, tmp.container.tileList[form.cells[i].templateIndex].dimensions.x, tmp.container.tileList[form.cells[i].templateIndex].dimensions.y));
				break;
			case RENDER_ENTITY:
				held++;
				skipDraw=true;
				if(items.getQuantityAt(held) > 0)
				{
					skipDraw=false;
					currentSprite.setTexture(entitySheet);
					currentSprite.setTextureRect(sf::IntRect(0,tmp.container.entityList[form.cells[i].templateIndex].origin.y, tmp.container.entityList[form.cells[i].templateIndex].dimensions.x, tmp.container.entityList[form.cells[i].templateIndex].dimensions.y));
				}
				break;
			case RENDER_BUTTON:
				currentSprite.setColor(sf::Color::Green);
				highlight++;
			default:
				currentSprite.setTexture(guiSheet);
				currentSprite.setTextureRect(sf::IntRect(0,tmp.container.buttonList[form.cells[i].templateIndex].origin.y, tmp.container.buttonList[form.cells[i].templateIndex].dimensions.x, tmp.container.buttonList[form.cells[i].templateIndex].dimensions.y));
				break;
		}
		currentSprite.setPosition(float(form.cells[i].pixel.x), float(form.cells[i].pixel.y)+16);

		if(!skipDraw) win.draw(currentSprite);
		skipDraw=false;
	}
	for(int j=0; j<items.capacity; j++)
	{
		if(items.getQuantityAt(j) > 1)
		{
			sprintf_s(out, "x%i", items.getQuantityAt(j));
			quantityOut.setString(out);
			quantityOut.setPosition(float(form.cells[j+1].pixel.x)+22.0f, float(form.cells[j+1].pixel.y)+30.0f);
			win.draw(quantityOut);
		}
	}
	for(int j=0; j<int(form.words.size()); j++)
	{
		guiText.setString(form.words[j].msg);
		guiText.setPosition(float(form.words[j].pixel.x), float(form.words[j].pixel.y));
		win.draw(guiText);
	}

}

void RenderManager::DrawStrings(sf::RenderWindow& win, GameObjectContainerClass& obj, const EtherRegistryClass& eth)
{
	for(int i=1; i<int(obj.strings.size()); i++)
	{
		if(eth.regText[obj.strings[i]] != NULL && eth.regText[obj.strings[i]]->active)
		{
			if(eth.regText[obj.strings[i]]->printType==PRINT_PANIN ||
				eth.regText[obj.strings[i]]->printType==PRINT_FADESTILL)
			{
				eth.regText[obj.strings[i]]->msg.setCharacterSize(90);
				eth.regText[obj.strings[i]]->msg.setFont(mainfont);
			}
			eth.regText[obj.strings[i]]->msg.setColor(eth.regText[obj.strings[i]]->color);
			win.draw(eth.regText[obj.strings[i]]->msg);
		}
	}
}