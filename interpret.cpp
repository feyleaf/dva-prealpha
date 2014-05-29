#include "globals.h"

ParserClass::ParserClass()
{
	pFile.clear();
	strcpy_s(line, "");
	openForReading("allreg-testing.txt");
}

ParserClass::~ParserClass()
{
	closeFile();
}
void ParserClass::openForReading(const char* filename)
{
	pFile.close();
	pFile.open(filename, std::ios_base::in); //opens a text file for input from the file
//	pFile.get(); //starts up the text getting at the first line
}

void ParserClass::closeFile()
{
	pFile.close();
}

bool ParserClass::readLine()
{
	std::string buffer;
	//reads one line, delimited by a '\n' newline
	std::getline(pFile, buffer, '\n');
	strcpy_s(line, 255, buffer.c_str());
	return !pFile.eof();
}

int ParserClass::getCategory()
{
	char* token;
	char* remainder;
	int ret=ICAT_NONE;
	if(!readLine()) return ICAT_NONE;
	if(line==NULL) return ICAT_NONE;
	token = strtok_s(line, ":", &remainder);
	if(strcmp(line, "VARIANCE")) ret=ICAT_COLORVARIANCE;
	if(strcmp(line, "BIOME")) ret=ICAT_BIOME;
	if(strcmp(line, "CREATURE")) ret=ICAT_CREATURE;
	if(strcmp(line, "DECORATION")) ret=ICAT_DECORATION;
	if(strcmp(line, "ECOLOGY")) ret=ICAT_ECOLOGY;
	if(strcmp(line, "INGREDIENT")) ret=ICAT_INGREDIENT;
	if(strcmp(line, "REGION")) ret=ICAT_REGION;
	if(strcmp(line, "SEED")) ret=ICAT_SEED;
	if(strcmp(line, "SUMMON")) ret=ICAT_SUMMON;
	if(strcmp(line, "TERRAIN")) ret=ICAT_TERRAIN;
	if(strcmp(line, "TILE")) ret=ICAT_TILE;
	if(strcmp(line, "TOOL")) ret=ICAT_TOOL;
	if(strcmp(line, "VEGETATION")) ret=ICAT_VEGETATION;
	if(strcmp(line, "ENTITY")) ret=ICAT_NULLENTITY;

	strcpy_s(line, remainder);
	return ret;
}

void ParserClass::getTokens(int category)
{
	switch(category)
	{
		case ICAT_COLORVARIANCE:
			//color variance reads the codename followed by red, green, blue base
			//then reads red, green blue range, finally the white value low and range
			parseColorVariance(line);
			break;
		case ICAT_BIOME:
			//biome reads the codename, then the printed name, followed by the terrain codename
			parseBiome(line);
			break;
		case ICAT_CREATURE:
			//creature reads the creature codename, followed by the default name, then
			//reads the width, height, sprite image file, number of frames, and the
			//hp, movement speed, etc
			parseCreature(line);
			break;
		case ICAT_DECORATION:
			//deco reads the deco codename, followed by the default name, then
			//reads the width, height, sprite image file, number of frames, and 
			//hp and special attributes
			parseDecoration(line);
			break;
		case ICAT_ECOLOGY:
			//ecology reads the ecology codename, followed by the printed name, then
			//reads each vegetation codename, each followed by a density value (parentesis if more than one)
			//reads each seed codename, each followed by a density value  (parenthesis if more than one)
			//then reads each creature codename, each followed by a density value  (parenthesis if more than one)
			parseEcology(line);
			break;
		case ICAT_INGREDIENT:
			//ingredient reads the ingredient codename, followed by the default name, then
			//reads the width, height, sprite image file, and number of frames
			parseIngredient(line);
			break;
		case ICAT_NULLENTITY:
			//entity reads the entity codename, followed by the default name, then
			//reads the width, height, sprite image file, number of frames
			parseEntity(line);
			break;
		case ICAT_REGION:
			//region reads a top, left, bottom, right rectangle vertices in grid form
			parseRegion(line);
			break;
		case ICAT_SEED:
			//seed reads the seed codename, followed by the default name, then
			//reads the width, height, sprite image file, and number of frames
			//then the vegetation entity codename that the seed becomes
			parseSeed(line);
			break;
		case ICAT_SUMMON:
			//summon reads the summon codename, followed by the default name, then
			//reads the width, height, sprite image file, and number of frames
			//then the creature codename that the summon charm contains
			parseSummon(line);
			break;
		case ICAT_TERRAIN:
			//terrain reads the terrain codename, followed by the printed name, then
			//reads each tile codename (parentesis if more than one)
			//reads each accent codenamt (parenthesis if more than one)
			//then reads each worn codename (parenthesis if more than one)
			//finally reads the list of shapes (parentesis again)
			parseTerrain(line);
			break;
		case ICAT_TILE:
			//tile reads the codename of the tile, followed by its printed name
			//then reads the width, height, sprite image file, and number of frames
			//then reads the color variance codename
			parseTile(line);
			break;
		case ICAT_TOOL:
			//tool reads the tool codename, followed by the default name, then
			//reads the width, height, sprite image file, and number of frames
			//then the process codename that it performs
			parseTool(line);
			break;
		case ICAT_VEGETATION:
			//vegetation reads the vegetation codename, followed by the default name, then
			//reads the width, height, sprite image file, and number of frames
			//then the entity codename list that it drops (enclosed in parenthesis if needed)
			parseVegetation(line);
			break;
		default:
			return;
	}
}

//color variance reads the codename followed by red, green, blue base
//then reads red, green blue range, finally the white value low and range
void ParserClass::parseColorVariance(const char* line)
{
	//this is a mess, i shall clean up the reading code
	colorVarianceTemplate b;
	char rem[255];
	strcpy_s(rem, line);
	char* token;
	char* context;
	token = strtok_s(rem, ",", &context); //the codename
	strcpy_s(b.cname, token); //put it into the variance template
	strcpy_s(rem, context);
	token = strtok_s(rem, ",", &context); //red base
	b.redBase = atoi(token); //put it into the variance template
	strcpy_s(rem, context);
	token = strtok_s(rem, ",", &context); //green base
	b.greenBase = atoi(token); //put it into the variance template
	strcpy_s(rem, context);
	token = strtok_s(rem, ",", &context); //blue base
	b.blueBase = atoi(token); //put it into the variance template
	strcpy_s(rem, context);
	token = strtok_s(rem, ",", &context); //red range
	b.redRange = atoi(token); //put it into the variance template
	strcpy_s(rem, context);
	token = strtok_s(rem, ",", &context); //green range
	b.greenRange = atoi(token); //put it into the variance template
	strcpy_s(rem, context);
	token = strtok_s(rem, ",", &context); //blue range
	b.blueRange = atoi(token); //put it into the variance template
	strcpy_s(rem, context);
	token = strtok_s(rem, ",", &context); //white base
	b.whiteBase = atoi(token); //put it into the variance template
	strcpy_s(rem, context);
	token = strtok_s(rem, ",", &context); //white range
	b.whiteRange = atoi(token); //put it into the variance template
	strcpy_s(rem, context);

	val.allColorVariances.push_back(new colorVarianceTemplate(b));
}
void ParserClass::parseBiome(const char* line)
{

}
void ParserClass::parseCreature(const char* line)
{

}
void ParserClass::parseDecoration(const char* line)
{

}
void ParserClass::parseEntity(const char* line)
{

}

//tile reads the codename of the tile, followed by its printed name
//then reads the width, height, sprite image file, and number of frames
//then reads the color variance codename
void ParserClass::parseTile(const char* line)
{
	tileTemplate b;
	char rem[255];
	strcpy_s(rem, line);
	char* token;
	char* context;
	token = strtok_s(rem, ",", &context); //the codename
	strcpy_s(b.cname, token); //put it into the tile template
	strcpy_s(rem, context);
	token = strtok_s(rem, ",", &context); //the print name
	strcpy_s(b.name, token); //put it into the tile template
	strcpy_s(rem, context);
	token = strtok_s(rem, ",", &context); //width
	b.dimensions.x = atoi(token); //put it into the tile template
	strcpy_s(rem, context);
	token = strtok_s(rem, ",", &context); //height
	b.dimensions.y = atoi(token); //put it into the tile template
	strcpy_s(rem, context);
	token = strtok_s(rem, ",", &context); //sprite file
	strcpy_s(b.spritefile, token); //put it into the tile template
	strcpy_s(rem, context);
	token = strtok_s(rem, ",", &context); //number of frames
	b.iconRange = atoi(token); //put it into the tile template
	strcpy_s(rem, context);
	token = strtok_s(rem, ",", &context); //color variance codename
	b.variance=-1;
	//now we look for the variance name. if it doesnt exist, we nix this entire tile registration
	for(int i=0; i<int(val.allColorVariances.size()-1); i++)
	{
		if(val.allColorVariances[i] != NULL)
		{
			if(strcmp(val.allColorVariances[i]->cname, token)==0)
			{
				b.variance = i;
				break;
			}
		}
	}
	if(b.variance==-1) return; //we dump this registry, it has an invalid variance codename

	//otehrwise, it gets registered :)
	val.allTiles.push_back(new tileTemplate(b));

}
void ParserClass::parseTerrain(const char* line)
{

}
void ParserClass::parseRegion(const char* line)
{

}
void ParserClass::parseTool(const char* line)
{

}
void ParserClass::parseIngredient(const char* line)
{

}
void ParserClass::parseSummon(const char* line)
{

}
void ParserClass::parseSeed(const char* line)
{

}
void ParserClass::parseEcology(const char* line)
{

}
void ParserClass::parseVegetation(const char* line)
{

}

