#include "globals.h"

ValueRegistryClass::ValueRegistryClass()
{
	allTiles.clear();
	allTiles.push_back(new tileTemplate(ID_GREENGRASS, "Green Grass", SHEET_TILES, 1, 1, VARIANCE_GREENGRASS));
	allTiles.push_back(new tileTemplate(ID_EMERALDGRASS, "Emerald Grass", SHEET_TILES, 2, 1, VARIANCE_EMERALDGRASS));
	allTiles.push_back(new tileTemplate(ID_SAND, "Sand", SHEET_TILES, 3, 1, VARIANCE_SAND));
	allTiles.push_back(new tileTemplate(ID_DIRT, "Dirt", SHEET_TILES, 4, 1, VARIANCE_DIRT));
	allTiles.push_back(new tileTemplate(ID_MARSHGRASS, "Marsh Grass", SHEET_TILES, 5, 1, VARIANCE_MARSHGRASS));
	allTiles.push_back(new tileTemplate(ID_WATER, "Water", SHEET_TILES, 6, 1, VARIANCE_WATER));
	allTiles.push_back(new tileTemplate(ID_REDBRICKS, "Red Brick", SHEET_TILES, 7, 1, VARIANCE_REDBRICKS));
	allTiles.push_back(new tileTemplate(ID_MARBLE, "Marble", SHEET_TILES, 8, 1, VARIANCE_MARBLE));
	allTiles.push_back(new tileTemplate(ID_BLUEBRICKS, "Blue Brick", SHEET_TILES, 9, 1, VARIANCE_BLUEBRICKS));
	allTiles.push_back(new tileTemplate(ID_GREYBRICKS, "Castle Brick", SHEET_TILES, 10, 1, VARIANCE_GREYBRICKS));
	allTiles.push_back(new tileTemplate(ID_PLANKS, "Wood Planks", SHEET_TILES, 11, 1, VARIANCE_PLANKS));
	allTiles.push_back(new tileTemplate(ID_CLAY, "Clay", SHEET_TILES, 12, 1, VARIANCE_CLAY));
	allTiles.push_back(new tileTemplate(ID_STONEWALL, "Stone Wall", SHEET_TILES, 13, 1, VARIANCE_STONEWALL));
	allTiles.push_back(new tileTemplate(ID_SNOW, "Snow", SHEET_TILES, 14, 1, VARIANCE_SNOW));
	allTiles.push_back(new tileTemplate(ID_OBSIDIAN, "Obsidian", SHEET_TILES, 15, 1, VARIANCE_OBSIDIAN));
	allTiles.push_back(new tileTemplate(ID_ICE, "Ice", SHEET_TILES, 16, 1, VARIANCE_ICE));

	allEntities.clear();
	allEntities.push_back(new entityTemplate(ID_MARLBOROBUSH, ET_DECO, "Marlboro Bush", SHEET_ENTITY, 1, 0, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_BOULDER, ET_DECO, "Boulder", SHEET_ENTITY, 2, 0, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_MAHOGANYLOG, ET_DECO, "Mahogany Log", SHEET_ENTITY, 3, 0, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_SQUIRREL, ET_CREATURE, "Squirrel", SHEET_ENTITY, 4, 0, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_IRONGOLEM, ET_CREATURE, "Iron Golem", SHEET_ENTITY, 5, 0, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_REDSEED, ET_SEED, "Red Rose Seed", SHEET_ENTITY, 6, 0, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_BLUESEED, ET_SEED, "Blue Rose Seed", SHEET_ENTITY, 15, 0, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_HEAVENSMIRRORSEED, ET_SEED, "Heavens Mirror Seed", SHEET_ENTITY, 11, 0, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_HIBISCUSSEED, ET_SEED, "Hibiscus Seed", SHEET_ENTITY, 24, 0, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_REDPETAL, ET_INGREDIENT, "Red Rose Petal", SHEET_ENTITY, 7, 0, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_BLUEPETAL, ET_INGREDIENT, "Blue Rose Petal", SHEET_ENTITY, 8, 0, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_REDROSE, ET_CREATURE, "Red Rose", SHEET_ENTITY, 16, 3, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_BLUEROSE, ET_CREATURE, "Blue Rose", SHEET_ENTITY, 20, 3, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_HEAVENSMIRROR, ET_CREATURE, "Heavens Mirror", SHEET_ENTITY, 12, 2, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_HIBISCUS, ET_CREATURE, "Hibiscus", SHEET_ENTITY, 25, 3, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_STUMP, ET_CREATURE, "Ritual Stump", SHEET_ENTITY, 29, 0, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_HOLE, ET_CREATURE, "Alchemist Hole", SHEET_ENTITY, 30, 0, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_SCISSOR, ET_CREATURE, "Scissors", SHEET_ENTITY, 31, 0, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_WOLF, ET_CREATURE, "Wolf", SHEET_ENTITY, 49, 1, CPROTO_NONE));
	allEntities.push_back(new entityTemplate(ID_DRYAD, ET_CREATURE, "Dryad", SHEET_ENTITY, 50, 1, CPROTO_NONE));
}