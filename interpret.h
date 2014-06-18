/*
interpret.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 31, 2014
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
#define ICAT_PROTOCOL 15
#define ICAT_ACTION 16
#define ICAT_BUTTON 17
#define ICAT_FORM 18

class TemplateReaderClass
{
protected:
	std::ifstream pFile;
	int lineNumber;

public:
	TemplateReaderClass() {lineNumber=0;}
	~TemplateReaderClass() {pFile.close();}

	bool openFile(const char* filename) {pFile.close(); pFile.open(filename, std::ifstream::in); return pFile.is_open();}
	int selectCategory();
	int getLineNumber() {return lineNumber;}
	tileTemplate parseTile();
	bool testLinkToColor(const char* codename, colorVarianceTemplate cvt);
	colorVarianceTemplate parseColor();
	entityTemplate parseEntity(unsigned char category);
	decoPackTemplate parseDecoration(int entityIndex);
	seedPackTemplate parseSeed(int entityIndex);
	creaturePackTemplate parseCreature(int entityIndex);
	vegetationPackTemplate parseVegetation(int entityIndex);
	summonPackTemplate parseSummon(int entityIndex);
	toolPackTemplate parseTool(int entityIndex);
	ingredientPackTemplate parseIngredient(int entityIndex);
	actionTemplate parseAction();
	terrainPoolTemplate parseTerrain();
	biomeInformationTemplate parseBiome();
	guiButtonTemplate parseButton();
};



#endif//INTERPRET_H