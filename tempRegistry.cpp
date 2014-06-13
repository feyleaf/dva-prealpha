#include "globals.h"

TemplateRegistryClass::TemplateRegistryClass()
{
}

void TemplateRegistryClass::parseFile(const char* filename)
{
	int cat=ICAT_NONE;
	int index=0;
	parser.openFile(filename);
	do{
		cat=parser.selectCategory();
		switch(cat)
		{
			case ICAT_COLORVARIANCE:
				container.varianceList.push_back(parser.parseColor());
				break;
			case ICAT_TILE:
				container.tileList.push_back(parser.parseTile());
				index=container.tileList.size()-1;
				if(!linkTileColors(index)) container.tileList.erase(container.tileList.begin()+index);
				break;
			case ICAT_PROTOCOL:
				container.actionList.push_back(parser.parseAction());
				break;
			case ICAT_DECORATION:
				container.entityList.push_back(parser.parseEntity(ICAT_DECORATION));
				index=container.entityList.size()-1;
				if(!linkEntityProtocol(index)) container.entityList.erase(container.entityList.begin()+index);
				else container.decoPackList.push_back(parser.parseDecoration(index));
				break;
			case ICAT_CREATURE:
				container.entityList.push_back(parser.parseEntity(ICAT_CREATURE));
				index=container.entityList.size()-1;
				if(!linkEntityProtocol(index)) container.entityList.erase(container.entityList.begin()+index);
				else container.creaturePackList.push_back(parser.parseCreature(index));
				break;
			case ICAT_INGREDIENT:
				container.entityList.push_back(parser.parseEntity(ICAT_INGREDIENT));
				index=container.entityList.size()-1;
				container.ingredientPackList.push_back(parser.parseIngredient(index));
				break;
			case ICAT_SEED:
				container.entityList.push_back(parser.parseEntity(ICAT_DECORATION));
				index=container.entityList.size()-1;
				if(!linkEntityProtocol(index)) container.entityList.erase(container.entityList.begin()+index);
				else container.seedPackList.push_back(parser.parseSeed(index));
				break;
			case ICAT_NULLENTITY:
				container.entityList.push_back(parser.parseEntity(ICAT_DECORATION));
				index=container.entityList.size()-1;
				if(!linkEntityProtocol(index)) container.entityList.erase(container.entityList.begin()+index);
				break;
			case ICAT_SUMMON:
				container.entityList.push_back(parser.parseEntity(ICAT_DECORATION));
				index=container.entityList.size()-1;
				if(!linkEntityProtocol(index)) container.entityList.erase(container.entityList.begin()+index);
				else container.summonPackList.push_back(parser.parseSummon(index));
				break;
			case ICAT_TOOL:
				container.entityList.push_back(parser.parseEntity(ICAT_DECORATION));
				index=container.entityList.size()-1;
				if(!linkEntityProtocol(index)) container.entityList.erase(container.entityList.begin()+index);
				else container.toolPackList.push_back(parser.parseTool(index));
				break;
			case ICAT_VEGETATION:
				container.entityList.push_back(parser.parseEntity(ICAT_VEGETATION));
				index=container.entityList.size()-1;
				if(!linkEntityProtocol(index)) container.entityList.erase(container.entityList.begin()+index);
				else container.vegPackList.push_back(parser.parseVegetation(index));
				break;
			default:
				parser.parseTool(0);
				break;
		}
	} while(cat != ICAT_NONE);
}

bool TemplateRegistryClass::linkTileColors(int tileIndex)
{
	int color=0;
	for(int i=0; i<int(container.varianceList.size()); i++)
	{
		if(strcmp(container.tileList[tileIndex].var_codename, container.varianceList[i].cname)==0)
		{
			color=i;
			break;
		}
	}				
	container.tileList[tileIndex].variance = color;
	return (color != 0);

}

bool TemplateRegistryClass::linkEntityProtocol(int entityIndex)
{
	int act=0;
	for(int i=1; i<int(container.entityList.size()); i++)
	{
		if(strcmp(container.entityList[entityIndex].creation, container.actionList[i].cname)==0)
		{
			act=i;
			break;
		}
	}				
	container.entityList[entityIndex].creationProtocol = act;
	return true;//(act != 0);

}