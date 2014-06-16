/*
gameobject.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 19, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#include "globals.h"

GameObjectContainerClass::GameObjectContainerClass()
{
	regTiles.clear();
	regEntities.clear();
	regTiles.push_back(NULL);
	regEntities.push_back(NULL);
	regCreature.clear();
	regCreature.push_back(NULL);
	regDeco.clear();
	regDeco.push_back(NULL);
	regIng.clear();
	regIng.push_back(NULL);
	regSeed.clear();
	regSeed.push_back(NULL);
	regSummon.clear();
	regSummon.push_back(NULL);
	regTool.clear();
	regTool.push_back(NULL);
	regVeg.clear();
	regVeg.push_back(NULL);
	actions.clear();
	actions.push_back(NULL);
}

int GameObjectContainerClass::numberOfTilesOnGrid(coord _grid)
{
	int ret=0;
	for(int i=1; i<int(regTiles.size()); i++)
	{
		if(regTiles[i]->pos == _grid)
		{
			ret++;
		}
	}
	return ret;
}

int GameObjectContainerClass::numberOfEntitiesOnGrid(coord _grid)
{
	int ret=0;
	for(int i=1; i<int(regEntities.size()); i++)
	{
		if(regEntities[i]->pos == _grid)
		{
			ret++;
		}
	}
}

int GameObjectContainerClass::getEntityTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename)
{
	for(int i=1; i<int(tmp.container.entityList.size()); i++)
	{
		if(strcmp(tmp.container.entityList[i].cname, _codename)==0)
			return i;
	}
}

int GameObjectContainerClass::getTileTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename)
{
	for(int i=1; i<int(tmp.container.tileList.size()); i++)
	{
		if(strcmp(tmp.container.tileList[i].cname, _codename)==0)
			return i;
	}

}

void GameObjectContainerClass::handleIndexesOnGrid(coord _grid)
{
	listHandler.clear();
	for(int i=1; i<int(regEntities.size()); i++)
	{
		if(regEntities[i]->pos == _grid && regEntities[i]->active)
		{
			listHandler.push_back(i);
		}
	}
}

void GameObjectContainerClass::handleEntitiesList(int _index)
{
	listHandler.clear();
	for(int i=1; i<int(regEntities.size()); i++)
	{
		if(regEntities[i] != NULL)
		{
			if(regEntities[i]->entityTemplateIndex==_index && regEntities[i]->active)
			{
				listHandler.push_back(i);
			}
		}
	}
}

void GameObjectContainerClass::handleTilesList(int _index)
{
	listHandler.clear();
	for(int i=1; i<int(regTiles.size()); i++)
	{
		if(regTiles[i] != NULL)
		{
			if(regTiles[i]->tileTemplateIndex==_index)
			{
				listHandler.push_back(i);
			}
		}
	}
}

void GameObjectContainerClass::handleTypesList(int _catType)
{
	listHandler.clear();
	for(int i=1; i<int(regEntities.size()); i++)
	{
		if(regEntities[i]->type == _catType && regEntities[i]->active)
		{
			listHandler.push_back(i);
		}
	}
}

GameObjectClass::GameObjectClass()
{
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
			obj.regTiles.push_back(new registeredTile(i, _orig, tmp.container.tileList[i].dimensions, _pos));
			if(tmp.container.tileList[i].variance>0)
				obj.regTiles[int(obj.regTiles.size()-1)]->distortionColor=getTileDistortion(tmp.container.varianceList[tmp.container.tileList[i].variance], _pos);
			return true;
		}
	}

	//TODO: color varaince!! :)
	return ret;
}

bool GameObjectClass::createEntity(const TemplateRegistryClass& tmp, const char* _name, coord _pos, float time)
{
	bool ret=false;
	coord _orig;
	unsigned char _type = 0;
	int row=0;
	int p=0;
	sf::IntRect bb; //the bounding box around the entity sprite(for mouse selection, NOT for collision with other entities)
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
	bb=tmp.container.entityList[p].box; bb.left+=(_pos.x*32); bb.top+=(_pos.y*32);

	switch(_type)
	{
		case ICAT_CREATURE:
			for(int i=1; i<int(tmp.container.creaturePackList.size()); i++)
			{
				if(tmp.container.creaturePackList[i].entityID==p)
				{
					obj.regCreature.push_back(new creaturePack(tmp.container.creaturePackList[i]));
					obj.regEntities.push_back(new registeredEntity(p,_type,int(obj.regCreature.size())-1, _orig, tmp.container.entityList[p].dimensions,bb,_pos));
					return true;
				}
			}
			break;
		case ICAT_DECORATION:
			for(int i=1; i<int(tmp.container.decoPackList.size()); i++)
			{
				if(tmp.container.decoPackList[i].entityID==p)
				{
					obj.regDeco.push_back(new decoPack(tmp.container.decoPackList[i]));
					obj.regEntities.push_back(new registeredEntity(p,_type,int(obj.regDeco.size())-1, _orig, tmp.container.entityList[p].dimensions,bb,_pos));
					return true;
				}
			}
			break;
		case ICAT_INGREDIENT:
			for(int i=1; i<int(tmp.container.ingredientPackList.size()); i++)
			{
				if(tmp.container.ingredientPackList[i].entityID==p)
				{
					obj.regIng.push_back(new ingredientPack(tmp.container.ingredientPackList[i]));
					obj.regEntities.push_back(new registeredEntity(p,_type,int(obj.regIng.size())-1, _orig, tmp.container.entityList[p].dimensions,bb,_pos));
					return true;
				}
			}
			break;
		case ICAT_SEED:
			for(int i=1; i<int(tmp.container.seedPackList.size()); i++)
			{
				if(tmp.container.seedPackList[i].entityID==p)
				{
					obj.regSeed.push_back(new seedPack());
					obj.regEntities.push_back(new registeredEntity(p,_type,int(obj.regSeed.size())-1, _orig, tmp.container.entityList[p].dimensions,bb,_pos));
					return true;
				}
			}
			break;
		case ICAT_SUMMON:
			for(int i=1; i<int(tmp.container.summonPackList.size()); i++)
			{
				if(tmp.container.summonPackList[i].entityID==p)
				{
					obj.regSummon.push_back(new summonPack());
					obj.regEntities.push_back(new registeredEntity(p,_type,int(obj.regSummon.size())-1, _orig, tmp.container.entityList[p].dimensions,bb,_pos));
					return true;
				}
			}
			break;
		case ICAT_TOOL:
			for(int i=1; i<int(tmp.container.toolPackList.size()); i++)
			{
				if(tmp.container.toolPackList[i].entityID==p)
				{
					obj.regTool.push_back(new toolPack());
					obj.regEntities.push_back(new registeredEntity(p,_type,int(obj.regTool.size())-1, _orig, tmp.container.entityList[p].dimensions,bb,_pos));
					return true;
				}
			}
			break;
		case ICAT_VEGETATION:
			for(int i=1; i<int(tmp.container.vegPackList.size()); i++)
			{
				if(tmp.container.vegPackList[i].entityID==p)
				{
					obj.regVeg.push_back(new vegPack(time, tmp.container.vegPackList[i]));
					obj.regEntities.push_back(new registeredEntity(p,_type,int(obj.regVeg.size())-1, _orig, tmp.container.entityList[p].dimensions,bb,_pos));
					createAction(tmp, tmp.container.entityList[p].creation, int(obj.regEntities.size()-1), 0, 0, time+float((tmp.container.vegPackList[i].growthTicks)*0.2f));
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

void GameObjectClass::eraseEntity(int entityIndex)
{
	//this routine DOES NOT remove the entity from the list, only empties the data
	//first erase the associated pack
	if(obj.regEntities[entityIndex] == NULL) return;
	int category=obj.regEntities[entityIndex]->type;
	int catIndex=obj.regEntities[entityIndex]->packIndex;
	switch(category)
	{
		case ICAT_CREATURE:
			if(obj.regCreature[catIndex] != NULL)
			{
				obj.regCreature[catIndex]->active=false;
			}
			break;
		case ICAT_DECORATION:
			if(obj.regDeco[catIndex] != NULL)
			{
				obj.regDeco[catIndex]->active=false;
			}
			break;
		case ICAT_INGREDIENT:
			if(obj.regIng[catIndex] != NULL)
			{
				obj.regIng[catIndex]->active=false;
			}
			break;
		case ICAT_SEED:
			if(obj.regSeed[catIndex] != NULL)
			{
				obj.regSeed[catIndex]->active=false;
			}
			break;
		case ICAT_SUMMON:
			if(obj.regSummon[catIndex] != NULL)
			{
				obj.regSummon[catIndex]->active=false;
			}
			break;
		case ICAT_TOOL:
			if(obj.regTool[catIndex] != NULL)
			{
				obj.regTool[catIndex]->active=false;
			}
			break;
		case ICAT_VEGETATION:
			if(obj.regVeg[catIndex] != NULL)
			{
				obj.regVeg[catIndex]->active=false;
			}
			break;
		default:
			break;
	}
	obj.regEntities[entityIndex]->active=false;
}

bool GameObjectClass::createAction(const TemplateRegistryClass& tmp, const char* _name, int entitySrc, int entityTrg, int tileTrg, float time)
{
	//first search the action template registry
	for(int i=1; i<int(tmp.container.actionList.size()); i++)
	{
		if(strcmp(tmp.container.actionList[i].cname, _name)==0)
		{
			actionStruct act;
			act.active=true;
			act.actionTemplateIndex=i;
			act.priority=1;
			act.queued=true;
			act.entityIndexSource=entitySrc;
			act.entityIndexTarget=entityTrg;
			act.tileIndexTarget=tileTrg;
			act.timeToActivate=time+float((tmp.container.actionList[i].coolDownTicks)*0.2f);
			//we can recycle inactive actions at this point
			//SIGNIFICANT decrease in vector size :)
			for(int i=1; i<int(obj.actions.size()); i++)
			{
				if(!obj.actions[i]->active)
				{
					delete obj.actions[i]; //deallocate old memory
					obj.actions[i] = new actionStruct(act);	//allocate new memory
					return true;
				}
			}
			//create a new action is all the rest of the actions are active
			obj.actions.push_back(new actionStruct(act));
			return true;
		}
	}
	return false;
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