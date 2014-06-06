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
	regTiles.clear();
	regEntities.clear();
	regTiles.push_back(NULL);
	regEntities.push_back(NULL);
}

bool GameObjectClass::createTile(const TemplateRegistryClass& tmp, const char* _name, coord _pos)
{
	bool ret=false;
	coord _orig;
	int row=0;
	//first search the template registry
	for(int i=1; i<int(tmp.container.tileList.size()); i++)
	{
		row+=tmp.container.tileList[i-1].dimensions.y;
		if(strcmp(tmp.container.tileList[i].cname, _name)==0)
		{
			_orig = coord(0, row);
			regTiles.push_back(new registeredTile(i, _orig, tmp.container.tileList[i].dimensions, _pos));
			if(tmp.container.tileList[i].variance>0)
				regTiles[int(regTiles.size()-1)]->distortionColor=getTileDistortion(tmp.container.varianceList[tmp.container.tileList[i].variance], _pos);
			return true;
		}
	}

	//TODO: color varaince!! :)
	return ret;
}

bool GameObjectClass::createEntity(const TemplateRegistryClass& tmp, const char* _name, coord _pos)
{
	bool ret=false;
	coord _orig;
	unsigned char _type = 0;
	int row=0;
	int p=0;
	//first search the entity template registry
	for(int i=1; i<int(tmp.container.entityList.size()); i++)
	{
		row+=tmp.container.entityList[i-1].dimensions.y;
		if(strcmp(tmp.container.entityList[i].cname, _name)==0)
		{
			_type=tmp.container.entityList[i].type;
			_orig = coord(0, row);
			p=i; break;
		}
	}
	if(p==0) return false;

	switch(_type)
	{
		case ICAT_CREATURE:
			for(int i=1; i<int(tmp.container.creaturePackList.size()); i++)
			{
				if(tmp.container.creaturePackList[i].entityID==p)
				{
					regEntities.push_back(new registeredEntity(p,_type,i, _orig, tmp.container.entityList[p].dimensions,_pos));
					return true;
				}
			}
			break;
		case ICAT_DECORATION:
			for(int i=1; i<int(tmp.container.decoPackList.size()); i++)
			{
				if(tmp.container.decoPackList[i].entityID==p)
				{
					regEntities.push_back(new registeredEntity(p,_type,i, _orig, tmp.container.entityList[p].dimensions,_pos));
					return true;
				}
			}
			break;
		case ICAT_INGREDIENT:
			for(int i=1; i<int(tmp.container.ingredientPackList.size()); i++)
			{
				if(tmp.container.ingredientPackList[i].entityID==p)
				{
					regEntities.push_back(new registeredEntity(p,_type,i, _orig, tmp.container.entityList[p].dimensions,_pos));
					return true;
				}
			}
			break;
		case ICAT_SEED:
			for(int i=1; i<int(tmp.container.seedPackList.size()); i++)
			{
				if(tmp.container.seedPackList[i].entityID==p)
				{
					regEntities.push_back(new registeredEntity(p,_type,i, _orig, tmp.container.entityList[p].dimensions,_pos));
					return true;
				}
			}
			break;
		case ICAT_SUMMON:
			for(int i=1; i<int(tmp.container.summonPackList.size()); i++)
			{
				if(tmp.container.summonPackList[i].entityID==p)
				{
					regEntities.push_back(new registeredEntity(p,_type,i, _orig, tmp.container.entityList[p].dimensions,_pos));
					return true;
				}
			}
			break;
		case ICAT_TOOL:
			for(int i=1; i<int(tmp.container.toolPackList.size()); i++)
			{
				if(tmp.container.toolPackList[i].entityID==p)
				{
					regEntities.push_back(new registeredEntity(p,_type,i, _orig, tmp.container.entityList[p].dimensions,_pos));
					return true;
				}
			}
			break;
		case ICAT_VEGETATION:
			for(int i=1; i<int(tmp.container.vegPackList.size()); i++)
			{
				if(tmp.container.vegPackList[i].entityID==p)
				{
					regEntities.push_back(new registeredEntity(p,_type,i, _orig, tmp.container.entityList[p].dimensions,_pos));
					return true;
				}
			}
			break;
		default:
			return false;
			break;
	}
	return ret;
}


sf::Color GameObjectClass::getTileDistortion(const colorVarianceTemplate& var, coord _pos)
{
	sf::Color ret;
	unsigned char value=0;
	unsigned char white = noiseyPixel(_pos, var.whiteBase, var.whiteRange, 16, 314159);
	ret.r=(noiseyPixel(_pos, var.redBase, var.redRange, 16, 314159));
	ret.g=(noiseyPixel(_pos, var.greenBase, var.greenRange, 16, 314159));
	ret.b=(noiseyPixel(_pos, var.blueBase, var.blueRange, 16, 314159));
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