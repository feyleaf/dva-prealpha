/*
gameobject.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
July 17, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#include "globals.h"

GameObjectContainerClass::GameObjectContainerClass()
{
	init();
}

GameObjectContainerClass::~GameObjectContainerClass()
{

	/*
	*/
	//init();
}

void GameObjectContainerClass::init()
{
	tiles.clear();		tiles.push_back(0);
	entities.clear();	entities.push_back(0);
	buttons.clear();	buttons.push_back(0);
	strings.clear();	strings.push_back(0);
	mapgenerator=0;
	mapMode = MAPMODE_NEUTRAL;
	walked=false;
	generated=false;
	biomeIndex=0;
}

int GameObjectContainerClass::tileIndexOnGrid(EtherRegistryClass& _ether, coord _grid)
{
	int ret=0;
	for(int i=1; i<int(tiles.size()); i++)
	{
		if(_ether.regTiles[tiles[i]] != NULL && _ether.regTiles[tiles[i]]->pos==_grid)
			ret=tiles[i];
	}
	return ret;
}

int GameObjectContainerClass::entityIndexOnGrid(EtherRegistryClass& _ether, coord _grid, int lower)
{
	int ret=0;
	for(int i=lower+1; i<int(entities.size()); i++)
	{
		if(_ether.regEntities[entities[i]]->pos==_grid)
			return entities[i];
	}
	return ret;
}

int GameObjectContainerClass::entityIndexAtPoint(EtherRegistryClass& _ether, coord _pixel)
{
	int ret=0;
	for(int i=1; i<int(entities.size()); i++)
	{
		if(_ether.regEntities[entities[i]]->box.contains(sf::Vector2i(_pixel.x, _pixel.y)))
			ret=entities[i];
	}
	return ret;
}

int GameObjectContainerClass::buttonIndexAtPoint(EtherRegistryClass& _ether, coord _pixel)
{
	int ret=0;
	for(int i=1; i<int(buttons.size()); i++)
	{
		if(_ether.regButtons[buttons[i]]->box.contains(sf::Vector2i(_pixel.x, _pixel.y)))
			ret=buttons[i];
	}
	return ret;
}

/*void GameObjectContainerClass::fillVegDropList(const TemplateRegistryClass& tmp, int packIndex, const char* _codename)
{
	for(int i=1; i<int(tmp.container.valuesList.size()); i++)
	{
		if(strcmp(tmp.container.valuesList[i].cname, _codename)==0)
		{
			regVeg[packIndex]->dropList=i;
			break;
		}
	}
}*/

void GameObjectContainerClass::releaseEntityList()
{
	entities.clear();
	entities.push_back(0);
}

void GameObjectContainerClass::releaseTileList()
{
	tiles.clear();
	tiles.push_back(0);
}

void GameObjectContainerClass::releaseButtonList()
{
	buttons.clear();
	buttons.push_back(0);
}

void GameObjectContainerClass::eraseEntity(int entityIndex)
{
	for(int i=1; i<int(entities.size()); i++)
	{
		if(entities[i]==entityIndex) entities.erase(entities.begin()+i);
	}
}
//wipes all registry items!
void GameObjectClass::clear(coord worldCursor)
{
	objMap[worldCursor].releaseEntityList();
	objMap[worldCursor].releaseTileList();
	objMap[worldCursor].releaseButtonList();
	objMap[worldCursor].init();
}

GameObjectClass::GameObjectClass()
{
}




sf::Color GameObjectClass::getTileDistortion(const colorVarianceTemplate& var, coord _pos, int con, long seed)
{
	sf::Color ret;
	unsigned char value=0;
	unsigned char white = noiseyPixel(_pos, var.whiteBase, var.whiteRange, con, seed);
	ret.r=(noiseyPixel(_pos, var.redBase, var.redRange, con, seed));
	ret.g=(noiseyPixel(_pos, var.greenBase, var.greenRange, con, seed));
	ret.b=(noiseyPixel(_pos, var.blueBase, var.blueRange, con, seed));
	value=unsigned char(max3(int(ret.r), int(ret.g), int(ret.b)));
	if(white>value)
	{
		for(int s=0; s<int(white-value); s++)
		{
			ret.r++; ret.g++; ret.b++;
		}
	}
	else if(value>white)
	{
		for(int s=0; s<int(value-white); s++)
		{
			if(ret.r>0) ret.r--;
			if(ret.g>0) ret.g--;
			if(ret.b>0) ret.b--;
		}
	}

	return ret;
}