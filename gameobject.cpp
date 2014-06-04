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
	sf::IntRect _texRect;
	int c=0;
	//first search the template registry
	for(int i=1; i<int(tmp.container.tileList.size()); i++)
	{
		c+=tmp.container.tileList[i].iconRange;
		if(strcmp(tmp.container.tileList[i].cname, _name)==0)
		{
			_texRect = sf::IntRect((c%8)*32,(int(c/8))*32,32,32);
			regTiles.push_back(new registeredTile(i, _pos, _texRect));
			return true;
		}
	}

	//TODO: color varaince!! :)
	return ret;
}

bool GameObjectClass::createEntity(const TemplateRegistryClass& tmp, const char* _name, unsigned char _type, coord _pos)
{
	bool ret=false;
	sf::Texture _tex;
	int p=0;
	//first search the entity template registry
	for(int i=0; i<int(tmp.container.entityList.size()); i++)
	{
		if(strcmp(tmp.container.entityList[i].cname, _name)==0)
		{
			_tex.loadFromFile("images/" + std::string(tmp.container.entityList[i].spritefile),
				sf::IntRect(0,0,tmp.container.entityList[i].dimensions.x, tmp.container.entityList[i].dimensions.y));
			p=i; break;
		}
	}
	if(p==0) return false;

	switch(_type)
	{
		case ICAT_CREATURE:
			for(int i=0; i<int(tmp.container.creaturePackList.size()); i++)
			{
				if(tmp.container.creaturePackList[i].entityID==p)
				{
					regEntities.push_back(new registeredEntity(p,_type,i,_pos));
					return true;
				}
			}
			break;
		case ICAT_DECORATION:
			for(int i=0; i<int(tmp.container.decoPackList.size()); i++)
			{
				if(tmp.container.decoPackList[i].entityID==p)
				{
					regEntities.push_back(new registeredEntity(p,_type,i,_pos));
					return true;
				}
			}
			break;
		case ICAT_INGREDIENT:
			for(int i=0; i<int(tmp.container.ingredientPackList.size()); i++)
			{
				if(tmp.container.ingredientPackList[i].entityID==p)
				{
					regEntities.push_back(new registeredEntity(p,_type,i,_pos));
					return true;
				}
			}
			break;
		case ICAT_SEED:
			for(int i=0; i<int(tmp.container.seedPackList.size()); i++)
			{
				if(tmp.container.seedPackList[i].entityID==p)
				{
					regEntities.push_back(new registeredEntity(p,_type,i,_pos));
					return true;
				}
			}
			break;
		case ICAT_SUMMON:
			for(int i=0; i<int(tmp.container.summonPackList.size()); i++)
			{
				if(tmp.container.summonPackList[i].entityID==p)
				{
					regEntities.push_back(new registeredEntity(p,_type,i,_pos));
					return true;
				}
			}
			break;
		case ICAT_TOOL:
			for(int i=0; i<int(tmp.container.toolPackList.size()); i++)
			{
				if(tmp.container.toolPackList[i].entityID==p)
				{
					regEntities.push_back(new registeredEntity(p,_type,i,_pos));
					return true;
				}
			}
			break;
		case ICAT_VEGETATION:
			for(int i=0; i<int(tmp.container.vegPackList.size()); i++)
			{
				if(tmp.container.vegPackList[i].entityID==p)
				{
					regEntities.push_back(new registeredEntity(p,_type,i,_pos));
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


/*
i like the algorithm in this. pretty simple but i would rather not delete it because i shall use if again
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
}*/