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
	if(varianceList[ret.tmp.variance] != NULL)
	{
		ret.curColor=getTileDistortion(varianceList[ret.tmp.variance], ret.grid, con, 100);
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
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color(25,109,0), sf::Color(182, 255, 182), char(140), char(190))));
	//emerald grass
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color(0,120,19), sf::Color::Green, char(200), char(255))));
	//sand
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color(255,255,90), sf::Color(196,197,17), char(240), char(255))));
	//dirt
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color(81,68,45), sf::Color(40,40,40), char(144), char(255))));
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
	varianceList.push_back(new colorVarianceTemplate(setRange(sf::Color::White, sf::Color(243,240,255), char(240), char(255))));
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