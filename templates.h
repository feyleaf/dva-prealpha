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

//constructed template of rules for directing tool creation
struct toolPackTemplate
{
	unsigned char id;
	unsigned int maxUses;
	unsigned char usageProtocol; //ie: what it works on, the input to give that subject
};

//constructed template of rules for directing seed creation
struct seedPackTemplate
{
	unsigned char id;
	unsigned char plantSummon;
	unsigned char usageProtocol; //ie: allowed terrain
};

//constructed template of rules for generating vegetation
struct vegetationPackTemplate
{
	unsigned char id;
	float growthTime;
	unsigned char maxGrowthStages;
	unsigned char mapBonus;
	std::vector<unsigned char> lootList;
};

//constructed template of rules for generating creatures
struct creaturePackTemplate
{
	unsigned char id;
	unsigned int maxHP;
	unsigned char attack;
	unsigned char defense;
	unsigned char agility;
	unsigned char moveSpeed;
	std::vector<unsigned char> actionList;
};

//constructed template of rules for decoration generation
struct decoPackTemplate
{
	unsigned char id;
	unsigned int maxHP;
	unsigned char mapBonus;
	unsigned char defense;
	unsigned char element;
	std::vector<unsigned char> lootList;
};

//constructed template of rules for summon charm generation
struct summonPackTemplate
{
	unsigned char id;
	unsigned int registeredCreatureSummon;	//0 if empty, must hold the game-registered id
											//meaning create the creature first then the summon
											//or create it empty
	unsigned char usageProtocol;
};

//compiled container of terrain pools in a template format, used in map generation
struct terrainPoolTemplate
{
	unsigned char id;
	char name[16];
	std::vector<unsigned char> landTilesList;
	std::vector<unsigned char> wornTilesList;
	std::vector<unsigned char> accentTilesList;
	std::vector<unsigned char> decoEntitiesList;
	std::vector<unsigned char> shapesList;
};//size 60 bytes(?)

//compiled template of eco pools in the biome/map generation
struct ecoPoolTemplate
{
	unsigned char id;
	char name[16];
	std::vector<unsigned char> vegetationList;
	std::vector<unsigned char> seedList;
	std::vector<unsigned char> creatureList;
};

//compiled template of biome information used in map generation
struct biomeInformationTemplate
{
	unsigned char id;
	char name[32];
	unsigned char terrainIndex;
	unsigned char ecoIndex;
	unsigned char temperature;
	unsigned char elevation;
	unsigned char atmosphere;
};

//item template
struct itemTemplate
{
	unsigned int registeredEntityIndex;	//0 if empty, must hold the game-registered id
											//meaning create the entity first then the item
											//or create it empty
	bool bStackable;	//stack quantity is 999
	bool bStorable;
	bool bRituals;
};

#endif//TEMPLATES_H