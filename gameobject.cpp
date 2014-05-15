#include "globals.h"

GameObjectClass::GameObjectClass()
{
	regTiles.push_back(NULL);
}

//returns the index from the template registry matching the search ID of the tile
int GameObjectClass::cloneTile(const unsigned int tileID, coord _grid)
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
				regTiles.push_back(new tileObjectStruct(newTile(*templateRegistry.allTiles[i], _grid)));
				return int(regTiles.size()-1);
			}
		}
	}
	return 0;
}

tileObjectStruct GameObjectClass::newTile(tileTemplate _t, coord _grid)
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
	ret.curColor = sf::Color(noiseyPixel(_grid.x, _grid.y, 5, 5), noiseyPixel(_grid.x, _grid.y, 90, 9), noiseyPixel(_grid.x, _grid.y, 100, 100));
	switch(ret.tmp.variance)
	{
	case VARIANCE_NONE: //basic variance in testing
	default:
		ret.curColor.r = noiseyPixel(_grid.x, _grid.y, 6, 6);
		break;
	}
	return ret;
}

