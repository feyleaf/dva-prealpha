#include "globals.h"

int TemplateReaderClass::selectCategory()
{
	int ret=ICAT_NONE;
	std::string str;
	if(pFile.eof()) return ret;
	lineNumber++;
	std::getline(pFile, str, ':');
	if(str=="VARIANCE") ret = ICAT_COLORVARIANCE;
	if(str=="BIOME") ret = ICAT_BIOME;
	if(str=="CREATURE") ret = ICAT_CREATURE;
	if(str=="DECORATION") ret = ICAT_DECORATION;
	if(str=="ARBOR") ret = ICAT_DECORATION2;
	if(str=="ECOLOGY") ret = ICAT_ECOLOGY;
	if(str=="INGREDIENT") ret = ICAT_INGREDIENT;
	if(str=="ENTITY") ret = ICAT_NULLENTITY;
	if(str=="REGION") ret = ICAT_REGION;
	if(str=="SEED") ret = ICAT_SEED;
	if(str=="SUMMON") ret = ICAT_SUMMON;
	if(str=="TERRAIN") ret = ICAT_TERRAIN;
	if(str=="TILE") ret = ICAT_TILE;
	if(str=="TOOL") ret = ICAT_TOOL;
	if(str=="VEGETATION") ret = ICAT_VEGETATION;
	if(str=="ACTION") ret = ICAT_ACTION;
	if(str=="BUTTON") ret = ICAT_BUTTON;
	if(str=="FORM") ret = ICAT_FORM;
	if(str=="PROTOCOL") ret = ICAT_PROTOCOL;
	if(str=="LIST") ret = ICAT_STRINGLIST;
	if(str=="COMMENT") std::getline(pFile, str, '\n');

	return ret;
}

colorVarianceTemplate TemplateReaderClass::parseColor()
{
	colorVarianceTemplate ret;
	//we read the codename,red,green,blue bases,red,green,blue ranges,white,whiterange
	std::string chunk;
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.cname, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, ',');
	ret.redBase = unsigned char(atoi(chunk.c_str()));
	std::getline(pFile, chunk, ',');
	ret.greenBase = unsigned char(atoi(chunk.c_str()));
	std::getline(pFile, chunk, ',');
	ret.blueBase = unsigned char(atoi(chunk.c_str()));
	std::getline(pFile, chunk, ',');
	ret.redRange = unsigned char(atoi(chunk.c_str()));
	std::getline(pFile, chunk, ',');
	ret.greenRange = unsigned char(atoi(chunk.c_str()));
	std::getline(pFile, chunk, ',');
	ret.blueRange = unsigned char(atoi(chunk.c_str()));
	std::getline(pFile, chunk, ',');
	ret.whiteBase = unsigned char(atoi(chunk.c_str()));
	std::getline(pFile, chunk, '\n');
	ret.whiteRange = unsigned char(atoi(chunk.c_str()));
	return ret;
}

tileTemplate TemplateReaderClass::parseTile()
{
	tileTemplate ret;
	//we read the codename,printed name,width,height,sprite file,icon range(1 for singles),and color variance codename
	std::string chunk;
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.cname, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.name, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, ',');
	ret.dimensions.x = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	ret.dimensions.y = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.spritefile, 40, chunk.c_str(), 40);
	std::getline(pFile, chunk, ',');
	ret.iconRange = unsigned char(atoi(chunk.c_str()));
	std::getline(pFile, chunk, '\n');
	strncpy_s(ret.var_codename, 32, chunk.c_str(), 32);

	return ret;
}

entityTemplate TemplateReaderClass::parseEntity(unsigned char category)
{
	entityTemplate ret;
	//we read the codename,printed name,width,height,sprite file,icon range(1 for singles),and color variance codename
	std::string chunk;
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.cname, 32, chunk.c_str(), 32);
	ret.type = category;
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.name, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, ',');
	ret.dimensions.x = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	ret.dimensions.y = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	ret.cog.x = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	ret.cog.y = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	ret.box.left = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	ret.box.top = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	ret.box.width = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	ret.box.height = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.spritefile, 40, chunk.c_str(), 40);
	std::getline(pFile, chunk, ',');
	ret.iconRange = unsigned char(atoi(chunk.c_str()));
	std::getline(pFile, chunk, ',');					//this reads to comma because we are going to
														//parse more directly afterward
	strncpy_s(ret.creation, 32, chunk.c_str(), 32);

	return ret;
}

decoPackTemplate TemplateReaderClass::parseDecoration(int entityIndex)
{
	decoPackTemplate ret;
	std::string chunk;
	ret.entityID = entityIndex;
	std::getline(pFile, chunk, ',');
	ret.maxHP = unsigned int(atoi(chunk.c_str()));
	std::getline(pFile, chunk, ',');
	ret.mapBonus = unsigned char(atoi(chunk.c_str()));
	std::getline(pFile, chunk, ',');
	ret.defense = unsigned char(atoi(chunk.c_str()));
	std::getline(pFile, chunk, ',');
	ret.element = unsigned char(atoi(chunk.c_str()));	//this shall be a string value linking to a
														//list of elements in the future
	std::getline(pFile, chunk, '\n');		//TODO: lootList here
	return ret;
}

creaturePackTemplate TemplateReaderClass::parseCreature(int entityIndex)
{
	creaturePackTemplate ret;
	std::string chunk;
	ret.entityID = entityIndex;
	std::getline(pFile, chunk, ',');
	ret.maxHP = unsigned int(atoi(chunk.c_str()));
	std::getline(pFile, chunk, ',');
	ret.attack = unsigned char(atoi(chunk.c_str()));
	std::getline(pFile, chunk, ',');
	ret.defense = unsigned char(atoi(chunk.c_str()));
	std::getline(pFile, chunk, ',');
	ret.agility = unsigned char(atoi(chunk.c_str()));
	std::getline(pFile, chunk, ',');
	ret.moveSpeed = unsigned char(atoi(chunk.c_str()));	
	std::getline(pFile, chunk, '\n');		//TODO: actionList here??
	return ret;
}

vegetationPackTemplate TemplateReaderClass::parseVegetation(int entityIndex)
{
	vegetationPackTemplate ret;
	std::string chunk;
	ret.entityID=entityIndex;
	std::getline(pFile, chunk, ',');
	ret.growthTicks = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	ret.maxGrowthStages = unsigned char(atoi(chunk.c_str()));
	std::getline(pFile, chunk, ',');
	ret.mapBonus = unsigned char(atoi(chunk.c_str()));

	std::getline(pFile, chunk, '\n');
	strncpy_s(ret.lootList, chunk.c_str(), 32);
	return ret;
}

ingredientPackTemplate TemplateReaderClass::parseIngredient(int entityIndex)
{
	ingredientPackTemplate ret;
	std::string chunk;
	ret.entityID=entityIndex;

	std::getline(pFile, chunk, '\n');
	strncpy_s(ret.usageProtocol, 32, chunk.c_str(), 32);
	return ret;
}

summonPackTemplate TemplateReaderClass::parseSummon(int entityIndex)
{
	//TODO: don't forget to put a string containing the entity codename!
	summonPackTemplate ret;
	std::string chunk;
	ret.entityID=entityIndex;
	//we'll skip the registered entity index in parsing, because it requires a unique value
	ret.regEntityIndex=0;
	std::getline(pFile, chunk, '\n');
	strncpy_s(ret.usageProtocol, 32, chunk.c_str(), 32);
	return ret;
}

seedPackTemplate TemplateReaderClass::parseSeed(int entityIndex)
{
	//TODO: don't forget to put a string in for the vegetation codename!
	seedPackTemplate ret;
	std::string chunk;
	ret.entityID=entityIndex;

	std::getline(pFile, chunk, ',');
	strncpy_s(ret.plantSummon, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, '\n');
	strncpy_s(ret.usageProtocol, 32, chunk.c_str(), 32);
	return ret;
}

toolPackTemplate TemplateReaderClass::parseTool(int entityIndex)
{
	toolPackTemplate ret;
	std::string chunk;
	ret.entityID=entityIndex;
	std::getline(pFile, chunk, ',');
	ret.maxUses=atoi(chunk.c_str());
	std::getline(pFile, chunk, '\n');
	strncpy_s(ret.usageProtocol, 32, chunk.c_str(), 32);
	return ret;
}


actionTemplate TemplateReaderClass::parseAction()
{
	actionTemplate ret;
	//we read the codename,printed name,width,height,sprite file,icon range(1 for singles),and color variance codename
	std::string chunk;
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.cname, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, ',');
	ret.category = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	ret.coolDownTicks = int(atoi(chunk.c_str())*0.20f); //read as frame updates, output as float seconds
	std::getline(pFile, chunk, ',');
	ret.priority = atoi(chunk.c_str());
	std::getline(pFile, chunk, '\n');
	ret.restrictions = atoi(chunk.c_str());	

	return ret;
}

guiButtonTemplate TemplateReaderClass::parseButton()
{
	guiButtonTemplate ret;
	//we read the codename,printed name,width,height,sprite file,icon range(1 for singles),and color variance codename
	std::string chunk;
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.cname, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.name, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, ',');
	ret.dimensions.x = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	ret.dimensions.y = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	ret.box.left = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	ret.box.top = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	ret.box.width = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	ret.box.height = atoi(chunk.c_str());
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.spritefile, 40, chunk.c_str(), 40);
	std::getline(pFile, chunk, ',');
	ret.iconRange = unsigned char(atoi(chunk.c_str()));
	std::getline(pFile, chunk, ',');					//this reads to comma because we are going to
														//parse more directly afterward
	strncpy_s(ret.actionName, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, '\n');					//this reads to comma because we are going to
														//parse more directly afterward
	return ret;
}

terrainPoolTemplate TemplateReaderClass::parseTerrain()
{
	//this could become more efficient with a parser-built-in index finder for tiles and decorations
	terrainPoolTemplate ret;
	std::string chunk;
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.cname, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.name, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.landListName, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.wornListName, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.accentListName, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.decoListName, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, '\n');
	strncpy_s(ret.shapesListName, 32, chunk.c_str(), 32);
	return ret;
}

ecoPoolTemplate TemplateReaderClass::parseEcology()
{
	//this could become more efficient with a parser-built-in index finder for tiles and decorations
	ecoPoolTemplate ret;
	std::string chunk;
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.cname, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.name, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.plantListName, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, '\n');
	strncpy_s(ret.creatureListName, 32, chunk.c_str(), 32);
	return ret;
}

biomeInformationTemplate TemplateReaderClass::parseBiome()
{
	biomeInformationTemplate ret;
	std::string chunk;
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.cname, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.name, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, ',');
	strncpy_s(ret.terrainListName, 32, chunk.c_str(), 32);
	std::getline(pFile, chunk, '\n');
	strncpy_s(ret.ecologyListName, 32, chunk.c_str(), 32);
	return ret;
}

stringList TemplateReaderClass::parseList()
{
	stringList ret;
	std::string chunk;

	std::getline(pFile, chunk, ',');
	strncpy_s(ret.cname, 32, chunk.c_str(), 32);

	std::getline(pFile, chunk, ',');
	while(strcmp(chunk.c_str(), "END")!=0)
	{
		ret.list.push_back(chunk);
		std::getline(pFile, chunk, ',');
	}
	std::getline(pFile, chunk, '\n'); //i'm pretty sure this works with a trailing comma on the list :/

	return ret;
}

guiFormsTemplate TemplateReaderClass::parseForms()
{
	guiFormsTemplate ret;
	std::string chunk;

	std::getline(pFile, chunk, ',');
	strncpy_s(ret.cname, 40, chunk.c_str(), 40);

	std::getline(pFile, chunk, ',');
	strncpy_s(ret.name, 32, chunk.c_str(), 32);

	std::getline(pFile, chunk, ',');
	ret.dimensions.x=unsigned char(atoi(chunk.c_str()));
	std::getline(pFile, chunk, ',');
	ret.dimensions.y=unsigned char(atoi(chunk.c_str()));

	std::getline(pFile, chunk, '\n'); //i'm pretty sure this works with a trailing comma on the list :/

	return ret;
}

void TemplateReaderClass::skipLine()
{
	std::string chunk;
	std::getline(pFile, chunk, '\n');
}