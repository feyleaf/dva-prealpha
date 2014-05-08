/*
debugrecords.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 7, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/
#include "globals.h"


void saveSizesTextfile(const char* outputfile)
{
	std::ofstream fout;
	fout.open(outputfile, std::ios_base::trunc|std::ios_base::out);
	
	fout << "Template Structure Sizes:\n";
	fout << "tileTemplate: " << sizeof(tileTemplate) << " bytes\n";
	fout << "entityTemplate: " << sizeof(entityTemplate) << " bytes\n";
	fout << "colorVarianceTemplate: " << sizeof(colorVarianceTemplate) << " bytes\n";
	fout << "toolPackTemplate: " << sizeof(toolPackTemplate) << " bytes\n";
	fout << "seedPackTemplate: " << sizeof(seedPackTemplate) << " bytes\n";
	fout << "vegetationPackTemplate: " << sizeof(vegetationPackTemplate) << " bytes\n";
	fout << "creaturePackTemplate: " << sizeof(creaturePackTemplate) << " bytes\n";
	fout << "decoPackTemplate: " << sizeof(decoPackTemplate) << " bytes\n";
	fout << "summonPackTemplate: " << sizeof(summonPackTemplate) << " bytes\n";
	fout << "terrainPoolTemplate: " << sizeof(terrainPoolTemplate) << " bytes\n";
	fout << "ecoPoolTemplate: " << sizeof(ecoPoolTemplate) << " bytes\n";
	fout << "biomeInformationTemplate: " << sizeof(biomeInformationTemplate) << " bytes\n";
	fout << "itemTemplate: " << sizeof(itemTemplate) << " bytes\n";
	fout << "actionTemplate: " << sizeof(actionTemplate) << " bytes\n";
	fout << "guiButtonTemplate: " << sizeof(guiButtonTemplate) << " bytes\n";
	fout << "guiFormsTemplate: " << sizeof(guiFormsTemplate) << " bytes\n";

	fout.close();
}