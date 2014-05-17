#include "globals.h"

GameObjectClass::GameObjectClass()
{
	varianceList.clear();
	varianceList.push_back(NULL);
	initializeColorVariance();
	regTiles.clear();
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
	ret.curColor = sf::Color::White;
	if(varianceList[ret.tmp.variance] != NULL)
	{
		ret.curColor=getTileDistortion(varianceList[ret.tmp.variance], ret.grid, 100);
	}
	return ret;
}

void GameObjectClass::initializeColorVariance()
{
/*
#define VARIANCE_NONE	0
#define VARIANCE_GREENGRASS	1
#define VARIANCE_EMERALDGRASS	2
#define VARIANCE_SAND	3
#define VARIANCE_DIRT	4
#define VARIANCE_MARSHGRASS	5
#define VARIANCE_WATER	6
#define VARIANCE_REDBRICKS	7
#define VARIANCE_MARBLE	8
#define VARIANCE_BLUEBRICKS	9
#define VARIANCE_GREYBRICKS	10
#define VARIANCE_PLANKS	11
#define VARIANCE_CLAY	12
#define VARIANCE_STONEWALL	13
#define VARIANCE_SNOW	14
#define VARIANCE_OBSIDIAN	15
#define VARIANCE_ICE	16
*/
	//green grass
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color::Green, sf::Color::Cyan, char(144), char(255))));
	//emerald grass
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color::Green, sf::Color::Cyan, char(144), char(255))));
	//sand
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color::Green, sf::Color::Cyan, char(144), char(255))));
	//dirt
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color::Green, sf::Color::Cyan, char(144), char(255))));
	//marsh grass
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color::Green, sf::Color::Cyan, char(144), char(255))));
	//water
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color::Green, sf::Color::Cyan, char(144), char(255))));
	//red brick
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color::Green, sf::Color::Cyan, char(144), char(255))));
	//marble
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color::Green, sf::Color::Cyan, char(144), char(255))));
	//blue brick
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color::Green, sf::Color::Cyan, char(144), char(255))));
	//grey brick
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color::Green, sf::Color::Cyan, char(144), char(255))));
	//planks
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color::Green, sf::Color::Cyan, char(144), char(255))));
	//clay
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color::Green, sf::Color::Cyan, char(144), char(255))));
	//stone wall
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color::Green, sf::Color::Cyan, char(144), char(255))));
	//snow
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color::White, sf::Color(240,240,255), char(144), char(255))));
	//obsidian
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color::Green, sf::Color::Cyan, char(144), char(255))));
	//ice
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color::Green, sf::Color::Cyan, char(144), char(255))));

}

colorVarianceTemplate GameObjectClass::setRange(sf::Color low, sf::Color high, char dark, char light)
{
	colorVarianceTemplate ret(255,255,255,255,255,255,255,255);
	if(low.r<high.r)
	{
		ret.redBase=char(low.r);
		ret.redRange=char(high.r-low.r);
	}
	else
	{
		ret.redBase=char(high.r);
		ret.redRange=char(low.r-high.r);
	}
	if(low.g<high.g)
	{
		ret.greenBase=char(low.g);
		ret.greenRange=char(high.g-low.g);
	}
	else
	{
		ret.greenBase=char(high.g);
		ret.greenRange=char(low.g-high.g);
	}
	if(low.b<high.b)
	{
		ret.blueBase=char(low.b);
		ret.blueRange=char(high.b-low.b);
	}
	else
	{
		ret.blueBase=char(high.b);
		ret.blueRange=char(low.b-high.b);
	}
	if(dark<light)
	{
		ret.whiteBase=char(dark);
		ret.whiteRange=char(light-dark);
	}
	else
	{
		ret.whiteBase=light;
		ret.whiteRange=char(dark-light);
	}
	return ret;
}

sf::Color GameObjectClass::getTileDistortion(const colorVarianceTemplate* _var, coord _pos, long seed)
{
	sf::Color ret(noiseyPixel(_pos, _var->redBase, _var->redRange, 60, seed),
	noiseyPixel(_pos, _var->greenBase, _var->greenRange, 60, seed),
	noiseyPixel(_pos, _var->blueBase, _var->blueRange, 60, seed),
	255);
	return ret;
}