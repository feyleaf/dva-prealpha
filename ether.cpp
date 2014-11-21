/*
ether.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
September 11, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#include "globals.h"

EtherRegistryClass::EtherRegistryClass()
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
	regButtons.clear();
	regButtons.push_back(NULL);
	regMaps.clear();
	regMaps.push_back(NULL);
	regText.clear();
	regText.push_back(NULL);
}

EtherRegistryClass::~EtherRegistryClass()
{
	int max=int(regButtons.size())-1;
	for(int i=max; i>0; i--)
	{
		delete regButtons[i];
		regButtons.erase(regButtons.begin()+i);
	}
	max=int(regCreature.size())-1;
	for(int i=max; i>0; i--)
	{
		delete regCreature[i];
		regCreature.erase(regCreature.begin()+i);
	}
	max=int(regDeco.size())-1;
	for(int i=max; i>0; i--)
	{
		delete regDeco[i];
		regDeco.erase(regDeco.begin()+i);
	}
	max=int(regIng.size())-1;
	for(int i=max; i>0; i--)
	{
		delete regIng[i];
		regIng.erase(regIng.begin()+i);
	}
	max=int(regSeed.size())-1;
	for(int i=max; i>0; i--)
	{
		delete regSeed[i];
		regSeed.erase(regSeed.begin()+i);
	}
	max=int(regSummon.size())-1;
	for(int i=max; i>0; i--)
	{
		delete regSummon[i];
		regSummon.erase(regSummon.begin()+i);
	}
	max=int(regTool.size())-1;
	for(int i=max; i>0; i--)
	{
		delete regTool[i];
		regTool.erase(regTool.begin()+i);
	}
	max=int(regVeg.size())-1;
	for(int i=max; i>0; i--)
	{
		delete regVeg[i];
		regVeg.erase(regVeg.begin()+i);
	}
	max=int(regEntities.size())-1;
	for(int i=max; i>0; i--)
	{
		delete regEntities[i];
		regEntities.erase(regEntities.begin()+i);
	}
	max=int(regTiles.size())-1;
	for(int i=max; i>0; i--)
	{
		delete regTiles[i];
		regTiles.erase(regTiles.begin()+i);
	}
	max=int(regMaps.size())-1;
	for(int i=max; i>0; i--)
	{
		delete regMaps[i];
		regMaps.erase(regMaps.begin()+i);
	}
}

int EtherRegistryClass::createTile(const TemplateRegistryClass& tmp, const char* _codename)
{
	coord _orig;
	int row=0;
	//first search the template registry
	for(int i=1; i<int(tmp.container.tileList.size()); i++)
	{
		row+=tmp.container.tileList[i-1].dimensions.y;
		if(strcmp(tmp.container.tileList[i].cname, _codename)==0)
		{
			_orig = coord(0, row);
			regTiles.push_back(new registeredTile(i, _orig, tmp.container.tileList[i].dimensions, coord(0,0)));
			
			return int(regTiles.size()-1);
		}
	}

	return 0;
}

int EtherRegistryClass::createEntity(const TemplateRegistryClass& tmp, const char* _codename)
{
	coord _orig;
	coord _dim;
	unsigned char _type = 0;
	int row=0;
	int p=0;
	sf::IntRect bb; //the bounding box around the entity sprite(for mouse selection, NOT for collision with other entities)
	//first search the entity template registry
	for(int i=1; i<int(tmp.container.entityList.size()); i++)
	{
		row+=tmp.container.entityList[i-1].dimensions.y;
		if(strcmp(tmp.container.entityList[i].cname, _codename)==0)
		{
			_type=tmp.container.entityList[i].type;
			_orig = coord(0, row);
			_dim=tmp.container.entityList[i].dimensions;
			bb=tmp.container.entityList[i].box;
			p=i; break;
		}
	}
	if(p==0) return 0;

	switch(_type)
	{
		case ICAT_CREATURE:
			for(int i=1; i<int(tmp.container.creaturePackList.size()); i++)
			{
				if(tmp.container.creaturePackList[i].entityID==p)
				{
					regCreature.push_back(new creaturePack(tmp.container.creaturePackList[i]));
					regEntities.push_back(new registeredEntity(p,_type,int(regCreature.size())-1, _orig, tmp.container.entityList[p].dimensions,tmp.container.entityList[p].cog,bb,coord(0,0)));
					return int(regEntities.size()-1);
				}
			}
			break;
		case ICAT_DECORATION:
			for(int i=1; i<int(tmp.container.decoPackList.size()); i++)
			{
				if(tmp.container.decoPackList[i].entityID==p)
				{
					regDeco.push_back(new decoPack(tmp.container.decoPackList[i]));
					regEntities.push_back(new registeredEntity(p,_type,int(regDeco.size())-1, _orig, _dim,tmp.container.entityList[p].cog,bb,coord(0,0)));
					return int(regEntities.size()-1);
				}
			}
			break;
		case ICAT_INGREDIENT:
			for(int i=1; i<int(tmp.container.ingredientPackList.size()); i++)
			{
				if(tmp.container.ingredientPackList[i].entityID==p)
				{
					regIng.push_back(new ingredientPack(tmp.container.ingredientPackList[i]));
					regEntities.push_back(new registeredEntity(p,_type,int(regIng.size())-1, _orig, tmp.container.entityList[p].dimensions,tmp.container.entityList[p].cog,bb,coord(0,0)));
					return int(regEntities.size()-1);
				}
			}
			break;
		case ICAT_SEED:
			for(int i=1; i<int(tmp.container.seedPackList.size()); i++)
			{
				if(tmp.container.seedPackList[i].entityID==p)
				{
					int seedI=int(regSeed.size());
					regSeed.push_back(new seedPack(tmp.container.seedPackList[i]));
					regSeed[seedI]->usageProtocol=0;
					regSeed[seedI]->vegetationContained=0;
					for(int j=1; j<int(tmp.container.actionList.size()); j++)
					{
						if(strcmp(tmp.container.actionList[j].cname, tmp.container.seedPackList[i].usageProtocol)==0)
							regSeed[seedI]->usageProtocol=j;
					}
					for(int j=1; j<int(tmp.container.actionList.size()); j++)
					{
						if(strcmp(tmp.container.actionList[j].cname, tmp.container.seedPackList[i].plantSummon)==0)
							regSeed[seedI]->vegetationContained=j;
					}
					regEntities.push_back(new registeredEntity(p,_type,seedI, _orig, tmp.container.entityList[p].dimensions,tmp.container.entityList[p].cog,bb,coord(0,0)));
					return int(regEntities.size()-1);
				}
			}
			break;
		case ICAT_SUMMON:
			for(int i=1; i<int(tmp.container.summonPackList.size()); i++)
			{
				if(tmp.container.summonPackList[i].entityID==p)
				{
					int summonI=int(regSummon.size());
					regSummon.push_back(new summonPack(tmp.container.summonPackList[i]));
					regSummon[summonI]->usageProtocol=0;
					for(int j=1; j<int(tmp.container.actionList.size()); j++)
					{
						if(strcmp(tmp.container.actionList[j].cname, tmp.container.summonPackList[i].usageProtocol)==0)
							regSummon[summonI]->usageProtocol=j;
					}
					regEntities.push_back(new registeredEntity(p,_type,summonI, _orig, tmp.container.entityList[p].dimensions,tmp.container.entityList[p].cog,bb,coord(0,0)));
					return int(regEntities.size()-1);
				}
			}
			break;
		case ICAT_TOOL:
			for(int i=1; i<int(tmp.container.toolPackList.size()); i++)
			{
				if(tmp.container.toolPackList[i].entityID==p)
				{
					regTool.push_back(new toolPack(tmp.container.toolPackList[i]));
					int toolIndex=int(regTool.size())-1;
					regTool[toolIndex]->usageProtocol=0;
					for(int j=1; j<int(tmp.container.actionList.size()); j++)
					{
						if(strcmp(tmp.container.actionList[j].cname, tmp.container.toolPackList[i].usageProtocol)==0)
							regTool[toolIndex]->usageProtocol=j;
					}
					regEntities.push_back(new registeredEntity(p,_type,toolIndex, _orig, tmp.container.entityList[p].dimensions,tmp.container.entityList[p].cog,bb,coord(0,0)));
					return int(regEntities.size()-1);
				}
			}
			break;
		case ICAT_VEGETATION:
			for(int i=1; i<int(tmp.container.vegPackList.size()); i++)
			{
				if(tmp.container.vegPackList[i].entityID==p)
				{
					int entI=int(regEntities.size());
					int vegI=int(regVeg.size());
					regVeg.push_back(new vegPack(JAN1_2014, tmp.container.vegPackList[i]));
					for(int j=1; j<int(tmp.container.valuesList.size()); j++)
					{
						if(strcmp(tmp.container.valuesList[j].cname, tmp.container.vegPackList[i].lootList)==0)
						{
							regVeg[vegI]->dropList=j;
							break;
						}
					}
					regEntities.push_back(new registeredEntity(p,_type,vegI, _orig, tmp.container.entityList[p].dimensions,tmp.container.entityList[p].cog,bb,coord(0,0)));
					return entI;
				}
			}
			break;
		default:
			return 0;
			break;
	}
	return 0;
}

int EtherRegistryClass::createButton(const TemplateRegistryClass& tmp, const char* _codename, int linkedEntity)
{
	coord _orig;
	int row=0;
	int p=0;
	sf::IntRect bb; //the bounding box around the button, for mouse selection
	//first search the entity template registry
	for(int i=1; i<int(tmp.container.buttonList.size()); i++)
	{
		row+=tmp.container.buttonList[i-1].dimensions.y;
		if(strcmp(tmp.container.buttonList[i].cname, _codename)==0)
		{
			_orig = coord(0, row);
			p=i; break;
		}
	}
	if(p==0) return 0;
	bb=tmp.container.buttonList[p].box;

	for(int i=1; i<int(tmp.container.actionList.size()); i++)
	{
		if(strcmp(tmp.container.actionList[i].cname, tmp.container.buttonList[p].actionName)==0)
		{
			regButtons.push_back(new buttonStruct(false, linkedEntity, i, _orig, tmp.container.buttonList[p].dimensions, bb, coord(0,0)));
			return int(regButtons.size()-1);
		}
	}
	regButtons.push_back(new buttonStruct(false, linkedEntity, 0, _orig, tmp.container.buttonList[p].dimensions, bb, coord(0,0)));
	return int(regButtons.size()-1);
}

int EtherRegistryClass::createString(const char* string, sf::Color& _color, sf::Font& _font, int pType, int _alphamax)
{
	textStruct makeText;
	makeText.active=true;
	makeText.printType=pType;
	makeText.msg.setFont(_font);
	if(string==NULL) 	makeText.msg.setString("");
	else makeText.msg.setString(sf::String(std::string(string)));
	makeText.msg.setColor(_color);
	makeText.alphamax=_alphamax;
	makeText.alphacur=_alphamax;
	regText.push_back(new textStruct(makeText));
	return int(regText.size()-1);
}


int EtherRegistryClass::createMapTerrain(const TemplateRegistryClass& tmp, const char* _biomeName)
{
	coord _tl=coord(rand()%5,rand()%5);
	coord _br=coord(rand()%5+20,rand()%5+10);

	int biomeIndex=0;
	for(int i=1; i<int(tmp.container.biomeList.size()); i++)
	{
		if(strcmp(tmp.container.biomeList[i].cname, _biomeName)==0)
			biomeIndex=i;
	}

	//match the terrain name with a name in the template registry, and store its index
	int terrainIndex=0;
	for(int i=1; i<int(tmp.container.terrainList.size()); i++)
	{
		if(strcmp(tmp.container.terrainList[i].cname, tmp.container.biomeList[biomeIndex].terrainListName)==0)
			terrainIndex=i;
	}
	int ecoIndex=0;
	for(int i=1; i<int(tmp.container.ecoList.size()); i++)
	{
		if(strcmp(tmp.container.ecoList[i].cname, tmp.container.biomeList[biomeIndex].ecologyListName)==0)
			ecoIndex=i;
	}
	int numberOfDecoTypes=0;
	int decoListIndex=0;
	for(int i=1; i<int(tmp.container.valuesList.size()); i++)
	{
		if(strcmp(tmp.container.valuesList[i].cname, tmp.container.terrainList[terrainIndex].decoListName)==0)
		{
			decoListIndex=i;
			numberOfDecoTypes=int(tmp.container.valuesList[i].list.size());
		}
	}

	mapGenStruct mapGen;
	if(terrainIndex==0)	return false;
	//match the tile list, then select a tile name from the list based on a random number
	int tileAlias=randomTileFromList(tmp, tmp.container.terrainList[terrainIndex].landListName);
	int accentAlias=randomTileFromList(tmp, tmp.container.terrainList[terrainIndex].accentListName);
	int wornAlias=randomTileFromList(tmp, tmp.container.terrainList[terrainIndex].wornListName);
	int vegAlias=randomEntityFromList(tmp, tmp.container.ecoList[ecoIndex].plantListName);
	int creatureAlias=randomEntityFromList(tmp, tmp.container.ecoList[ecoIndex].creatureListName);

	mapGen.displaying=true;
	mapGen.active=true;
	mapGen.baseTiles=tileAlias;
	mapGen.worldCoords=coord(0,0);

	//now we create some shapes
	mapShapeStruct layer;
	mapSpreadStruct decoSpread;
	if(wornAlias>0)
	{
		std::string shapeString=randomShapeFromList(tmp, tmp.container.terrainList[terrainIndex].shapesListName);
		layer.tl=_tl;
		layer.br=_br;
		strncpy_s(layer.shapeName, 32, shapeString.c_str(), 32);
		layer.terrainTiles=wornAlias;
		mapGen.shapeLayer.push_back(new mapShapeStruct(layer));
	}
	if(accentAlias>0)
	{
		std::string shapeString=randomShapeFromList(tmp, tmp.container.terrainList[terrainIndex].shapesListName);
		layer.tl=_tl;
		layer.br=_br;
		strncpy_s(layer.shapeName, 32, shapeString.c_str(), 32);
		layer.terrainTiles=accentAlias;
		mapGen.shapeLayer.push_back(new mapShapeStruct(layer));
	}

	//then the decorations
	int decoAlias=0;
	for(int t=0; t<numberOfDecoTypes; t++)
	{
		decoAlias=randomEntityFromList(tmp, tmp.container.terrainList[terrainIndex].decoListName);
		for(int i=1; i<int(tmp.container.entityList.size()); i++)
		{
			if(strcmp(tmp.container.entityList[i].cname, tmp.container.valuesList[decoListIndex].list[t].c_str())==0)
			{
				decoAlias=i;
				break;
			}
		}
		if(decoAlias>0)
		{
			_tl=coord(rand()%5,rand()%5);
			_br=coord(rand()%5+20,rand()%5+10);
			decoSpread.tl=_tl;
			decoSpread.br=_br;
			decoSpread.density=rand()%3+3;
			decoSpread.entityTemplateIndex=decoAlias;
			mapGen.decoLayer.push_back(new mapSpreadStruct(decoSpread));
		}
	}

	if(vegAlias>0)
	{
		mapSpreadStruct vegSpread;
		vegSpread.entityTemplateIndex=vegAlias;
		vegSpread.density=rand()%3+3;
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
	}

	regMaps.push_back(new mapGenStruct(mapGen));
	return int(regMaps.size()-1);
}

int EtherRegistryClass::randomEntityFromList(const TemplateRegistryClass& tmp, const char* _codename)
{
	int listIndex=0;
	for(int i=1; i<int(tmp.container.valuesList.size()); i++)
	{
		if(strcmp(tmp.container.valuesList[i].cname, _codename)==0)
			listIndex=i;
	}
	if(listIndex<1 || listIndex>int(tmp.container.valuesList.size())) return 0;
	if(tmp.container.valuesList[listIndex].list.empty()) return 0;
	int numberOfItems = int(tmp.container.valuesList[listIndex].list.size())-1;
	if(numberOfItems<1) return 0;
	int selection = rand()%numberOfItems + 1;
	for(int i=1; i<int(tmp.container.entityList.size()); i++)
	{
		if(strcmp(tmp.container.entityList[i].cname, tmp.container.valuesList[listIndex].list[selection].c_str())==0)
			return i;
	}
	return 0;
}

int EtherRegistryClass::randomTileFromList(const TemplateRegistryClass& tmp, const char* _codename)
{
	int listIndex=0;
	for(int i=1; i<int(tmp.container.valuesList.size()); i++)
	{
		if(strcmp(tmp.container.valuesList[i].cname, _codename)==0)
			listIndex=i;
	}
	if(listIndex<1 || listIndex>int(tmp.container.valuesList.size())) return 0;
	if(tmp.container.valuesList[listIndex].list.size()<2) return 0;
	int numberOfItems = int(tmp.container.valuesList[listIndex].list.size())-1;
	int selection = rand()%numberOfItems + 1;
	for(int i=1; i<int(tmp.container.tileList.size()); i++)
	{
		if(strcmp(tmp.container.tileList[i].cname, tmp.container.valuesList[listIndex].list[selection].c_str())==0)
			return i;
	}
	return 0;
}

std::string EtherRegistryClass::randomShapeFromList(const TemplateRegistryClass& tmp, const char* _codename)
{
	int listIndex=0;
	for(int i=1; i<int(tmp.container.valuesList.size()); i++)
	{
		if(strcmp(tmp.container.valuesList[i].cname, _codename)==0)
			listIndex=i;
	}
	if(listIndex<1 || listIndex>int(tmp.container.valuesList.size())) return "";
	if(tmp.container.valuesList[listIndex].list.empty()) return "";
	int numberOfItems = int(tmp.container.valuesList[listIndex].list.size())-1;
	int selection = rand()%numberOfItems + 1;
	return tmp.container.valuesList[listIndex].list[selection];
}

