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
	return ret;
}

ingredientPackTemplate TemplateReaderClass::parseIngredient(int entityIndex)
{
	ingredientPackTemplate ret;
	std::string chunk;
	ret.entityID=entityIndex;

	std::getline(pFile, chunk, '\n');
	return ret;
}

summonPackTemplate TemplateReaderClass::parseSummon(int entityIndex)
{
	//TODO: don't forget to put a string containing the entity codename!
	summonPackTemplate ret;
	std::string chunk;
	ret.entityID=entityIndex;

	std::getline(pFile, chunk, '\n');
	return ret;
}

seedPackTemplate TemplateReaderClass::parseSeed(int entityIndex)
{
	//TODO: don't forget to put a string in for the vegetation codename!
	seedPackTemplate ret;
	std::string chunk;
	ret.entityID=entityIndex;

	std::getline(pFile, chunk, '\n');
	return ret;
}

toolPackTemplate TemplateReaderClass::parseTool(int entityIndex)
{
	toolPackTemplate ret;
	std::string chunk;
	ret.entityID=entityIndex;

	std::getline(pFile, chunk, '\n');
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