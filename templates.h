/*
templates.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 3, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#ifndef TEMPLATES_H
#define TEMPLATES_H

//constructed template rules for tile creation
struct tileTemplate 
// selection of non-unique tile templates used in map creation and rendering
// registered as the basis of unique tiles in the map generation and storage
{
	unsigned int id;
	char name[16];
	unsigned char sheet;
	unsigned char sheetOrigin;
	unsigned char iconRange;
	unsigned char variance;
};//size 24 bytes

//constructed template rules for entity creation
struct entityTemplate 
// selection of non-unique entity templates used in entity creation and rendering
// registered as the basis of unique objects in the gameplay and storage
// creationProtocol is the index to a set of rules that are applied to the
// entity's own data during creation, which requires specifics outside of the
// template scope (such as the HP of a creature, growth rate of a plant, etc.)
{
	unsigned int id;
	unsigned char type;
	char name[40];
	unsigned char sheet;
	unsigned char sheetOrigin;
	unsigned char iconRange;
	unsigned char creationProtocol;
};//size 52 bytes

//constructed template rules for varying colors on map tiles
struct colorVarianceTemplate
// selection of color variance values for use in the ic_randomTile distortion algorithm
{
	char redBase;
	char redRange;
	char greenBase;
	char greenRange;
	char blueBase;
	char blueRange;
	char whiteBase;
	char whiteRange;
};//size 8 bytes

//constructed template of rules for directing entity creation
struct creationProtocolTemplate
{
	unsigned char id;
	bool hostile;

};
//compiled container of terrain pools in a template format
struct terrainPoolTemplate
{
	std::vector<unsigned char> landTilesList;
	std::vector<unsigned char> wornTilesList;
	std::vector<unsigned char> accentTilesList;
	std::vector<unsigned char> decoEntitiesList;
	std::vector<unsigned char> shapesList;
};//size 60 bytes(?)

#endif//TEMPLATES_H