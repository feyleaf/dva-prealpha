/*
interpret.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 26, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
This class will run routines for parsing and interpreting text file information
to be used in the template registry as well as
the registered object save/load
*/

#ifndef INTERPRET_H
#define INTERPRET_H

//ICAT = interpreter category
#define ICAT_NONE 0
#define ICAT_COLORVARIANCE 1
#define ICAT_TILE 2
#define ICAT_NULLENTITY 3
#define ICAT_SEED 4
#define ICAT_DECORATION 5
#define ICAT_CREATURE 6
#define ICAT_INGREDIENT 7
#define ICAT_SUMMON 8
#define ICAT_TOOL 9
#define ICAT_VEGETATION 10
#define ICAT_TERRAIN 11
#define ICAT_ECOLOGY 12
#define ICAT_REGION 13
#define ICAT_BIOME 14

class ParserClass
{
protected:
	std::ifstream pFile;

public:
	ParserClass();
	~ParserClass();

	char line[255];
	ValueRegistryClass val;

	void openForReading(const char* filename);
	void closeFile();

	bool readLine();
	int getCategory();
	void getTokens(int category);

	void parseColorVariance(const char* line);
	void parseBiome(const char* line);
	void parseCreature(const char* line);
	void parseDecoration(const char* line);
	void parseEntity(const char* line);
	void parseTile(const char* line);
	void parseTerrain(const char* line);
	void parseRegion(const char* line);
	void parseTool(const char* line);
	void parseIngredient(const char* line);
	void parseSummon(const char* line);
	void parseSeed(const char* line);
	void parseEcology(const char* line);
	void parseVegetation(const char* line);


};



#endif//INTERPRET_H