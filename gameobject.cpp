/*
gameobject.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 19, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#include "globals.h"

GameObjectClass::GameObjectClass()
{
	varianceList.clear();
	varianceList.push_back(NULL);
	regTiles.clear();
	regTiles.push_back(NULL);
	regEntities.clear();
	regEntities.push_back(NULL);
}

//returns the index from the template registry matching the search ID of the tile
int GameObjectClass::cloneTile(const unsigned int tileID, coord _grid, int con)
{
	for(int i=0; i<int(regTiles.size()); i++)
	{
		if(regTiles[i] != NULL)
		{
			if(regTiles[i]->tmp.id == tileID && regTiles[i]->grid == _grid) //if there is already a match in the registry, we'll return that one
				return i;
		}
	}
	//if we found no match, then we create a new registered tile
	for(int i=0; i<int(templateRegistry.allTiles.size()); i++)
	{
		if(templateRegistry.allTiles[i] != NULL)
		{
			if(templateRegistry.allTiles[i]->id == tileID) //denotes a match from the queried tile
			{
				//we clone the selected tile with index i
				regTiles.push_back(new tileObjectStruct(newTile(*templateRegistry.allTiles[i], _grid, con)));
				return int(regTiles.size()-1);
			}
		}
	}
	return 0;
}

int GameObjectClass::cloneEntity(const unsigned int entityID, coord _grid)
{
	for(int i=0; i<int(regEntities.size()); i++)
	{
		if(regEntities[i] != NULL)
		{
			if(regEntities[i]->tmp.id == entityID && regEntities[i]->grid == _grid) //if there is already a match in the registry, we'll return that one
				return i;
		}
	}
	//if we found no match, then we create a new registered tile
	for(int i=0; i<int(templateRegistry.allEntities.size()); i++)
	{
		if(templateRegistry.allEntities[i] != NULL)
		{
			if(templateRegistry.allEntities[i]->id == entityID) //denotes a match from the queried tile
			{
				//we clone the selected tile with index i
				regEntities.push_back(new entityObjectStruct(newEntity(*templateRegistry.allEntities[i], _grid)));
				return int(regEntities.size()-1);
			}
		}
	}
	return 0;
}

tileObjectStruct GameObjectClass::newTile(tileTemplate _t, coord _grid, int con)
{
	tileObjectStruct ret;
	ret.tmp = tileTemplate(_t);
	ret.tmp.id = _t.id;
	strncpy_s(ret.tmp.name, 16, _t.name, 16);
	ret.tmp.sheet = _t.sheet;
	ret.tmp.sheetOrigin = _t.sheetOrigin;
	ret.tmp.iconRange = _t.iconRange;
	ret.tmp.variance = _t.variance;
	ret.grid = _grid;
	ret.curColor = sf::Color::White;
	for(int i=0; i<int(templateRegistry.allColorVariances.size()); i++)
	{
		if(templateRegistry.allColorVariances[i] != NULL)
		{
			if(templateRegistry.allColorVariances[i]->id == ret.tmp.variance) //denotes a match from the queried tile
			{
				ret.curColor=getTileDistortion(templateRegistry.allColorVariances[i], ret.grid, con, 100);
			}
		}
	}
	return ret;
}

entityObjectStruct GameObjectClass::newEntity(entityTemplate _t, coord _grid)
{
	entityObjectStruct ret;
	ret.tmp = entityTemplate(_t);
	ret.tmp.id = _t.id;
	ret.tmp.type = _t.type;
	strncpy_s(ret.tmp.name, 16, _t.name, 16);
	ret.tmp.sheet = _t.sheet;
	ret.tmp.sheetOrigin = _t.sheetOrigin;
	ret.tmp.iconRange = _t.iconRange;
	ret.tmp.creationProtocol = _t.creationProtocol;
	ret.grid = _grid;
	return ret;
}

sf::Color GameObjectClass::getTileDistortion(const colorVarianceTemplate* _var, coord _pos, int con, long seed)
{
	sf::Color ret;
	unsigned char value=0;
	unsigned char white = noiseyPixel(_pos, _var->whiteBase, _var->whiteRange, con, seed);
	ret.r=(noiseyPixel(_pos, _var->redBase, _var->redRange, con, seed));
	ret.g=(noiseyPixel(_pos, _var->greenBase, _var->greenRange, con, seed));
	ret.b=(noiseyPixel(_pos, _var->blueBase, _var->blueRange, con, seed));
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