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
	regButtons.clear();
	regButtons.push_back(NULL);
	regMaps.clear();
	regMaps.push_back(NULL);
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
	return ret;
}

int GameObjectContainerClass::getEntityTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename)
{
	for(int i=1; i<int(tmp.container.entityList.size()); i++)
	{
		if(strcmp(tmp.container.entityList[i].cname, _codename)==0)
			return i;
	}
	return 0;
}

int GameObjectContainerClass::getActionTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename)
{
	for(int i=1; i<int(tmp.container.actionList.size()); i++)
	{
		if(strcmp(tmp.container.actionList[i].cname, _codename)==0)
			return i;
	}
	return 0;
}

void GameObjectContainerClass::fillVegDropList(const TemplateRegistryClass& tmp, int packIndex, const char* _codename)
{
	for(int i=1; i<int(tmp.container.valuesList.size()); i++)
	{
		if(strcmp(tmp.container.valuesList[i].cname, _codename)==0)
		{
			regVeg[packIndex]->dropList=i;
			break;
		}
	}
}

int GameObjectContainerClass::randomEntityFromList(const TemplateRegistryClass& tmp, const char* _codename)
{
	int listIndex=getListTemplateIndex(tmp, _codename);
	if(listIndex<1 || listIndex>int(tmp.container.valuesList.size())) return 0;
	if(tmp.container.valuesList[listIndex].list.empty()) return 0;
	int numberOfItems = int(tmp.container.valuesList[listIndex].list.size())-1;
	int selection = rand()%numberOfItems + 1;
	return getEntityTemplateIndex(tmp, tmp.container.valuesList[listIndex].list[selection].c_str());
}

int GameObjectContainerClass::randomTileFromList(const TemplateRegistryClass& tmp, const char* _codename)
{
	int listIndex=getListTemplateIndex(tmp, _codename);
	if(listIndex<1 || listIndex>int(tmp.container.valuesList.size())) return 0;
	if(tmp.container.valuesList[listIndex].list.size()<2) return 0;
	int numberOfItems = int(tmp.container.valuesList[listIndex].list.size())-1;
	int selection = rand()%numberOfItems + 1;
	return getTileTemplateIndex(tmp, tmp.container.valuesList[listIndex].list[selection].c_str());
}

std::string GameObjectContainerClass::randomShapeFromList(const TemplateRegistryClass& tmp, const char* _codename)
{
	int listIndex=getListTemplateIndex(tmp, _codename);
	if(listIndex<1 || listIndex>int(tmp.container.valuesList.size())) return "";
	if(tmp.container.valuesList[listIndex].list.empty()) return "";
	int numberOfItems = int(tmp.container.valuesList[listIndex].list.size())-1;
	int selection = rand()%numberOfItems + 1;
	return tmp.container.valuesList[listIndex].list[selection];
}

int GameObjectContainerClass::getTileTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename)
{
	for(int i=1; i<int(tmp.container.tileList.size()); i++)
	{
		if(strcmp(tmp.container.tileList[i].cname, _codename)==0)
			return i;
	}
	return 0;
}

int GameObjectContainerClass::getGuiTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename)
{
	for(int i=1; i<int(tmp.container.buttonList.size()); i++)
	{
		if(strcmp(tmp.container.buttonList[i].cname, _codename)==0)
			return i;
	}
	return 0;
}

int GameObjectContainerClass::getButtonForAction(const TemplateRegistryClass& tmp, const char* _codename)
{
	for(int i=1; i<int(regButtons.size()); i++)
	{
		if(strcmp(tmp.container.actionList[regButtons[i]->actionTemplateIndex].cname, _codename)==0)
			return i;
	}
	return 0;
}

int GameObjectContainerClass::getButtonLinkedEntity(const TemplateRegistryClass& tmp, const char* _codename)
{
	int ret=0;
	for(int i=1; i<int(regButtons.size()); i++)
	{
		if(strcmp(tmp.container.actionList[regButtons[i]->actionTemplateIndex].cname, _codename)==0)
		{
			if(regButtons[i]->active) ret=regButtons[i]->linkedEntityIndex;
		}
	}
	return ret;
}

void GameObjectContainerClass::activateEntityButtons(int entityIndex)
{
	for(int i=1; i<int(regButtons.size()); i++)
	{
		if(regButtons[i]->linkedEntityIndex == entityIndex)
			regButtons[i]->active=true;
	}
}

void GameObjectContainerClass::deactivateEntityButtons(int entityIndex)
{
	for(int i=1; i<int(regButtons.size()); i++)
	{
		if(regButtons[i]->linkedEntityIndex == entityIndex)
			regButtons[i]->active=false;
	}
}

int GameObjectContainerClass::getListTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename)
{
	for(int i=1; i<int(tmp.container.valuesList.size()); i++)
	{
		if(strcmp(tmp.container.valuesList[i].cname, _codename)==0)
			return i;
	}
	return 0;
}

int GameObjectContainerClass::getTerrainTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename)
{
	for(int i=1; i<int(tmp.container.terrainList.size()); i++)
	{
		if(strcmp(tmp.container.terrainList[i].cname, _codename)==0)
			return i;
	}
	return 0;
}

int GameObjectContainerClass::getEcologyTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename)
{
	for(int i=1; i<int(tmp.container.ecoList.size()); i++)
	{
		if(strcmp(tmp.container.ecoList[i].cname, _codename)==0)
			return i;
	}
	return 0;
}

int GameObjectContainerClass::getBiomeTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename)
{
	for(int i=1; i<int(tmp.container.biomeList.size()); i++)
	{
		if(strcmp(tmp.container.biomeList[i].cname, _codename)==0)
			return i;
	}
	return 0;
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

//wipes all registry items!
void GameObjectClass::clear(coord worldCursor)
{
	while(!objMap[worldCursor].regTiles.empty())
	{
		delete objMap[worldCursor].regTiles[0];
		objMap[worldCursor].regTiles.erase(objMap[worldCursor].regTiles.begin());
	}
	while(!objMap[worldCursor].regCreature.empty())
	{
		delete objMap[worldCursor].regCreature[0];
		objMap[worldCursor].regCreature.erase(objMap[worldCursor].regCreature.begin());
	}
	while(!objMap[worldCursor].regDeco.empty())
	{
		delete objMap[worldCursor].regDeco[0];
		objMap[worldCursor].regDeco.erase(objMap[worldCursor].regDeco.begin());
	}
	while(!objMap[worldCursor].regIng.empty())
	{
		delete objMap[worldCursor].regIng[0];
		objMap[worldCursor].regIng.erase(objMap[worldCursor].regIng.begin());
	}
	while(!objMap[worldCursor].regMaps.empty())
	{
		delete objMap[worldCursor].regMaps[0];
		objMap[worldCursor].regMaps.erase(objMap[worldCursor].regMaps.begin());
	}
	while(!objMap[worldCursor].regSeed.empty())
	{
		delete objMap[worldCursor].regSeed[0];
		objMap[worldCursor].regSeed.erase(objMap[worldCursor].regSeed.begin());
	}
	while(!objMap[worldCursor].regSummon.empty())
	{
		delete objMap[worldCursor].regSummon[0];
		objMap[worldCursor].regSummon.erase(objMap[worldCursor].regSummon.begin());
	}
	while(!objMap[worldCursor].regTool.empty())
	{
		delete objMap[worldCursor].regTool[0];
		objMap[worldCursor].regTool.erase(objMap[worldCursor].regTool.begin());
	}
	while(!objMap[worldCursor].regVeg.empty())
	{
		delete objMap[worldCursor].regVeg[0];
		objMap[worldCursor].regVeg.erase(objMap[worldCursor].regVeg.begin());
	}
	while(!objMap[worldCursor].regEntities.empty())
	{
		delete objMap[worldCursor].regEntities[0];
		objMap[worldCursor].regEntities.erase(objMap[worldCursor].regEntities.begin());
	}

	int ac=objMap[worldCursor].actions.size()-1;
	for(int i=0; i<ac; i++)
	{
		delete objMap[worldCursor].actions[ac-i];
		objMap[worldCursor].actions.erase(objMap[worldCursor].actions.begin()+(ac-i));
	}
	objMap[worldCursor].init();
}

bool GameObjectClass::createMapTerrain(const TemplateRegistryClass& tmp, const char* _biomename, coord worldCursor)
{
	coord _tl=coord(rand()%5,rand()%5);
	coord _br=coord(rand()%5+20,rand()%5+10);
	int biomeIndex=objMap[worldCursor].getBiomeTemplateIndex(tmp, _biomename);
	//match the terrain name with a name in the template registry, and store its index
	int terrainIndex=objMap[worldCursor].getTerrainTemplateIndex(tmp, tmp.container.biomeList[biomeIndex].terrainListName);
	int ecoIndex=objMap[worldCursor].getEcologyTemplateIndex(tmp, tmp.container.biomeList[biomeIndex].ecologyListName);
	mapGenStruct mapGen;
	if(terrainIndex==0)	return false;
	//match the tile list, then select a tile name from the list based on a random number
	int tileAlias=objMap[worldCursor].randomTileFromList(tmp, tmp.container.terrainList[terrainIndex].landListName);
	int accentAlias=objMap[worldCursor].randomTileFromList(tmp, tmp.container.terrainList[terrainIndex].accentListName);
	int wornAlias=objMap[worldCursor].randomTileFromList(tmp, tmp.container.terrainList[terrainIndex].wornListName);
	int decoAlias=objMap[worldCursor].randomEntityFromList(tmp, tmp.container.terrainList[terrainIndex].decoListName);
	int vegAlias=objMap[worldCursor].randomEntityFromList(tmp, tmp.container.ecoList[ecoIndex].plantListName);
	int creatureAlias=objMap[worldCursor].randomEntityFromList(tmp, tmp.container.ecoList[ecoIndex].creatureListName);

	mapGen.displaying=true;
	mapGen.active=true;
	mapGen.baseTiles=tileAlias;
	mapGen.worldCoords=coord(0,0);

	//now we create some shapes
	mapShapeStruct layer;
	mapSpreadStruct decoSpread;
	if(wornAlias>0)
	{
		std::string shapeString=objMap[worldCursor].randomShapeFromList(tmp, tmp.container.terrainList[terrainIndex].shapesListName);
		layer.tl=_tl;
		layer.br=_br;
		strncpy_s(layer.shapeName, 32, shapeString.c_str(), 32);
		layer.terrainTiles=wornAlias;
		mapGen.shapeLayer.push_back(new mapShapeStruct(layer));
	}
	if(accentAlias>0)
	{
		std::string shapeString=objMap[worldCursor].randomShapeFromList(tmp, tmp.container.terrainList[terrainIndex].shapesListName);
		layer.tl=_tl;
		layer.br=_br;
		strncpy_s(layer.shapeName, 32, shapeString.c_str(), 32);
		layer.terrainTiles=accentAlias;
		mapGen.shapeLayer.push_back(new mapShapeStruct(layer));
	}

	//then the decorations
	if(decoAlias>0)
	{
		_tl=coord(rand()%5,rand()%5);
		_br=coord(rand()%5+20,rand()%5+10);
		decoSpread.tl=_tl;
		decoSpread.br=_br;
		decoSpread.density=rand()%15+3;
		decoSpread.entityTemplateIndex=decoAlias;
		mapGen.decoLayer.push_back(new mapSpreadStruct(decoSpread));
	}

	if(vegAlias>0)
	{
		mapSpreadStruct vegSpread;
		vegSpread.entityTemplateIndex=vegAlias;
		vegSpread.density=rand()%10+3;
		vegSpread.tl=coord(0,0); vegSpread.br=coord(15,15);
		mapGen.vegLayer.push_back(new mapSpreadStruct(vegSpread));
	}

	if(creatureAlias>0)
	{
		mapSpreadStruct creatureSpread;
		creatureSpread.entityTemplateIndex=creatureAlias;
		creatureSpread.density=rand()%2+2;
		creatureSpread.tl=coord(0,0); creatureSpread.br=coord(15,15);
		mapGen.creatureLayer.push_back(new mapSpreadStruct(creatureSpread));
		objMap[worldCursor].regMaps.push_back(new mapGenStruct(mapGen));
	}
	return true;	
}

GameObjectClass::GameObjectClass()
{
}

bool GameObjectClass::createTile(const TemplateRegistryClass& tmp, const char* _name, coord _pos, int _con, long _seed, coord worldCoord)
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
			objMap[worldCoord].regTiles.push_back(new registeredTile(i, _orig, tmp.container.tileList[i].dimensions, _pos));
			if(tmp.container.tileList[i].variance>0)
				objMap[worldCoord].regTiles[int(objMap[worldCoord].regTiles.size()-1)]->distortionColor=getTileDistortion(tmp.container.varianceList[tmp.container.tileList[i].variance], _pos, _con, _seed);
			return true;
		}
	}

	//TODO: color varaince!! :)
	return ret;
}

void GameObjectClass::cloneToInventory(int entityIndex, coord worldCoord)
{
	//if(entityIndex<1 || entityIndex>int(objMap[worldCursor].regEntities.size())) return;
	//if(obj.regEntities[entityIndex] == NULL) return;
	objMap[worldCoord].regEntities[entityIndex]->plane=1;
	objMap[worldCoord].regEntities[entityIndex]->pos=coord(-1,-1);
}

void GameObjectClass::cloneFromInventory(int entityIndex, coord _pos, coord worldCoord)
{
	if(entityIndex<1 || entityIndex>int(objMap[worldCoord].regEntities.size())) return;
	if(objMap[worldCoord].regEntities[entityIndex] == NULL) return;
	objMap[worldCoord].regEntities[entityIndex]->plane=0;
	objMap[worldCoord].regEntities[entityIndex]->pos=_pos;
}

int GameObjectClass::createEntity(const TemplateRegistryClass& tmp, const char* _name, coord _pos, float time, coord worldCoord)
{
	bool ret=0;
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
					objMap[worldCoord].regCreature.push_back(new creaturePack(tmp.container.creaturePackList[i]));
					objMap[worldCoord].regEntities.push_back(new registeredEntity(p,_type,int(objMap[worldCoord].regCreature.size())-1, _orig, tmp.container.entityList[p].dimensions,bb,_pos));
					createAction(tmp, tmp.container.entityList[p].creation, int(objMap[worldCoord].regEntities.size()-1), 0, 0, time, worldCoord);
					return int(objMap[worldCoord].regEntities.size()-1);
				}
			}
			break;
		case ICAT_DECORATION:
			for(int i=1; i<int(tmp.container.decoPackList.size()); i++)
			{
				if(tmp.container.decoPackList[i].entityID==p)
				{
					objMap[worldCoord].regDeco.push_back(new decoPack(tmp.container.decoPackList[i]));
					objMap[worldCoord].regEntities.push_back(new registeredEntity(p,_type,int(objMap[worldCoord].regDeco.size())-1, _orig, tmp.container.entityList[p].dimensions,bb,_pos));
					return int(objMap[worldCoord].regEntities.size()-1);
				}
			}
			break;
		case ICAT_INGREDIENT:
			for(int i=1; i<int(tmp.container.ingredientPackList.size()); i++)
			{
				if(tmp.container.ingredientPackList[i].entityID==p)
				{
					objMap[worldCoord].regIng.push_back(new ingredientPack(tmp.container.ingredientPackList[i]));
					objMap[worldCoord].regEntities.push_back(new registeredEntity(p,_type,int(objMap[worldCoord].regIng.size())-1, _orig, tmp.container.entityList[p].dimensions,bb,_pos));
					return int(objMap[worldCoord].regEntities.size()-1);
				}
			}
			break;
		case ICAT_SEED:
			for(int i=1; i<int(tmp.container.seedPackList.size()); i++)
			{
				if(tmp.container.seedPackList[i].entityID==p)
				{
					int seedI=int(objMap[worldCoord].regSeed.size());
					objMap[worldCoord].regSeed.push_back(new seedPack(tmp.container.seedPackList[i]));
					objMap[worldCoord].regSeed[seedI]->usageProtocol=objMap[worldCoord].getActionTemplateIndex(tmp, tmp.container.seedPackList[i].usageProtocol);
					objMap[worldCoord].regSeed[seedI]->vegetationContained=objMap[worldCoord].getEntityTemplateIndex(tmp, tmp.container.seedPackList[i].plantSummon);
					objMap[worldCoord].regEntities.push_back(new registeredEntity(p,_type,seedI, _orig, tmp.container.entityList[p].dimensions,bb,_pos));
					return int(objMap[worldCoord].regEntities.size()-1);
				}
			}
			break;
		case ICAT_SUMMON:
			for(int i=1; i<int(tmp.container.summonPackList.size()); i++)
			{
				if(tmp.container.summonPackList[i].entityID==p)
				{
					int summonI=int(objMap[worldCoord].regSummon.size());
					objMap[worldCoord].regSummon.push_back(new summonPack(tmp.container.summonPackList[i]));
					objMap[worldCoord].regSummon[summonI]->usageProtocol=objMap[worldCoord].getActionTemplateIndex(tmp, tmp.container.summonPackList[i].usageProtocol);
					objMap[worldCoord].regEntities.push_back(new registeredEntity(p,_type,int(objMap[worldCoord].regSummon.size())-1, _orig, tmp.container.entityList[p].dimensions,bb,_pos));
					return int(objMap[worldCoord].regEntities.size()-1);
				}
			}
			break;
		case ICAT_TOOL:
			for(int i=1; i<int(tmp.container.toolPackList.size()); i++)
			{
				if(tmp.container.toolPackList[i].entityID==p)
				{
					objMap[worldCoord].regTool.push_back(new toolPack(tmp.container.toolPackList[i]));
					int toolIndex=int(objMap[worldCoord].regTool.size())-1;
					objMap[worldCoord].regTool[toolIndex]->usageProtocol = objMap[worldCoord].getActionTemplateIndex(tmp, tmp.container.toolPackList[i].usageProtocol);
					objMap[worldCoord].regEntities.push_back(new registeredEntity(p,_type,toolIndex, _orig, tmp.container.entityList[p].dimensions,bb,_pos));

					return int(objMap[worldCoord].regEntities.size()-1);
				}
			}
			break;
		case ICAT_VEGETATION:
			for(int i=1; i<int(tmp.container.vegPackList.size()); i++)
			{
				if(tmp.container.vegPackList[i].entityID==p)
				{
					int entI=int(objMap[worldCoord].regEntities.size());
					int vegI=int(objMap[worldCoord].regVeg.size());
					objMap[worldCoord].regVeg.push_back(new vegPack(time, tmp.container.vegPackList[i]));
					objMap[worldCoord].fillVegDropList(tmp, vegI, tmp.container.vegPackList[i].lootList);
					objMap[worldCoord].regEntities.push_back(new registeredEntity(p,_type,vegI, _orig, tmp.container.entityList[p].dimensions,bb,_pos));
					createAction(tmp, tmp.container.entityList[p].creation, entI, 0, 0, time+float((tmp.container.vegPackList[i].growthTicks)*0.2f), worldCoord);
					return entI;
				}
			}
			break;
		default:
			return 0;
			break;
	}
	return ret;
}

bool GameObjectClass::createButton(const TemplateRegistryClass& tmp, const char* _name, coord _pos, coord worldCoord, int linkedEntity, bool act)
{
	bool ret=false;
	coord _orig;
	int row=0;
	int p=0;
	sf::IntRect bb; //the bounding box around the entity sprite(for mouse selection, NOT for collision with other entities)
	//first search the entity template registry
	for(int i=1; i<int(tmp.container.buttonList.size()); i++)
	{
		row+=tmp.container.buttonList[i-1].dimensions.y;
		if(strcmp(tmp.container.buttonList[i].cname, _name)==0)
		{
			_orig = coord(0, row);
			p=i; break;
		}
	}
	if(p==0) return false;
	bb=tmp.container.buttonList[p].box; bb.left+=(_pos.x*32); bb.top+=(_pos.y*32);

	for(int i=1; i<int(tmp.container.actionList.size()); i++)
	{
		if(strcmp(tmp.container.actionList[i].cname, tmp.container.buttonList[p].actionName)==0)
		{
			objMap[worldCoord].regButtons.push_back(new buttonStruct(act, linkedEntity, i, _orig, tmp.container.buttonList[p].dimensions, bb, _pos));
			return true;
		}
	}
	return ret;
}

void GameObjectContainerClass::eraseEntity(int entityIndex)
{
	//this routine DOES NOT remove the entity from the list, only empties the data
	//first erase the associated pack
	if(regEntities[entityIndex] == NULL) return;
	int category=regEntities[entityIndex]->type;
	int catIndex=regEntities[entityIndex]->packIndex;
	switch(category)
	{
		case ICAT_CREATURE:
			if(regCreature[catIndex] != NULL)
			{
				regCreature[catIndex]->active=false;
			}
			break;
		case ICAT_DECORATION:
			if(regDeco[catIndex] != NULL)
			{
				regDeco[catIndex]->active=false;
			}
			break;
		case ICAT_INGREDIENT:
			if(regIng[catIndex] != NULL)
			{
				regIng[catIndex]->active=false;
			}
			break;
		case ICAT_SEED:
			if(regSeed[catIndex] != NULL)
			{
				regSeed[catIndex]->active=false;
			}
			break;
		case ICAT_SUMMON:
			if(regSummon[catIndex] != NULL)
			{
				regSummon[catIndex]->active=false;
			}
			break;
		case ICAT_TOOL:
			if(regTool[catIndex] != NULL)
			{
				regTool[catIndex]->active=false;
			}
			break;
		case ICAT_VEGETATION:
			if(regVeg[catIndex] != NULL)
			{
				regVeg[catIndex]->active=false;
			}
			break;
		default:
			break;
	}
	regEntities[entityIndex]->active=false;
}

bool GameObjectClass::createAction(const TemplateRegistryClass& tmp, const char* _name, int entitySrc, int entityTrg, int tileTrg, float time, coord worldCoord)
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
			for(int i=1; i<int(objMap[worldCoord].actions.size()); i++)
			{
				if(!objMap[worldCoord].actions[i]->active)
				{
					delete objMap[worldCoord].actions[i]; //deallocate old memory
					objMap[worldCoord].actions[i] = new actionStruct(act);	//allocate new memory
					return true;
				}
			}
			//create a new action is all the rest of the actions are active
			objMap[worldCoord].actions.push_back(new actionStruct(act));
			return true;
		}
	}
	return false;
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