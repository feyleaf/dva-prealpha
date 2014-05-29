#include "globals.h"

ValueRegistryClass::ValueRegistryClass()
{
	allColorVariances.clear();
	registerColorVariance(VARIANCE_GREENGRASS, 25, 100, 109, 100, 142, 100, 140, 50);
	registerColorVariance(VARIANCE_EMERALDGRASS, 0, 4, 120,100, 19, 5, 170, 50);
	registerColorVariance(VARIANCE_SAND, 240, 12, 240, 12, 90, 0, 200, 30);
	registerColorVariance(VARIANCE_DIRT, 81, 20, 68, 20, 45, 20, 190, 50);
	registerColorVariance(VARIANCE_MARSHGRASS, 0, 40, 200, 50, 0, 0, 160, 60);
	registerColorVariance(VARIANCE_WATER, 0, 50, 19, 50, 200, 50, 180, 70);
	registerColorVariance(VARIANCE_REDBRICKS, 40, 60, 0, 30, 0, 20, 200, 50);
	registerColorVariance(VARIANCE_MARBLE, 128, 40, 128, 40, 166, 40, 190, 20);
	registerColorVariance(VARIANCE_BLUEBRICKS, 128, 0, 128, 0, 200, 30, 210, 45);
	registerColorVariance(VARIANCE_GREYBRICKS, 99, 40, 99, 40, 99, 40, 190, 45);
	registerColorVariance(VARIANCE_PLANKS, 96, 10, 69, 10, 28, 0, 240, 10);
	registerColorVariance(VARIANCE_CLAY, 160, 60, 140, 15, 110, 25, 168, 75);
	registerColorVariance(VARIANCE_STONEWALL, 128, 0, 128, 0, 128, 0, 190, 50);
	registerColorVariance(VARIANCE_SNOW, 220, 15, 210, 0, 210, 45, 240, 15);
	registerColorVariance(VARIANCE_OBSIDIAN, 60, 50, 0, 12, 40, 50, 60, 30);
	registerColorVariance(VARIANCE_ICE, 150, 10, 199, 10, 215, 40, 200, 42);

	allTiles.clear();
/*	registerTile(ID_GREENGRASS, "Green Grass", SHEET_TILES, 1, 1, VARIANCE_GREENGRASS);
	registerTile(ID_EMERALDGRASS, "Emerald Grass", SHEET_TILES, 2, 1, VARIANCE_EMERALDGRASS);
	registerTile(ID_SAND, "Sand", SHEET_TILES, 3, 1, VARIANCE_SAND);
	registerTile(ID_DIRT, "Dirt", SHEET_TILES, 4, 1, VARIANCE_DIRT);
	registerTile(ID_MARSHGRASS, "Marsh Grass", SHEET_TILES, 5, 1, VARIANCE_MARSHGRASS);
	registerTile(ID_WATER, "Water", SHEET_TILES, 6, 1, VARIANCE_WATER);
	registerTile(ID_REDBRICKS, "Red Brick", SHEET_TILES, 7, 1, VARIANCE_REDBRICKS);
	registerTile(ID_MARBLE, "Marble", SHEET_TILES, 8, 1, VARIANCE_MARBLE);
	registerTile(ID_BLUEBRICKS, "Blue Brick", SHEET_TILES, 9, 1, VARIANCE_BLUEBRICKS);
	registerTile(ID_GREYBRICKS, "Castle Brick", SHEET_TILES, 10, 1, VARIANCE_GREYBRICKS);
	registerTile(ID_PLANKS, "Wood Planks", SHEET_TILES, 11, 1, VARIANCE_PLANKS);
	registerTile(ID_CLAY, "Clay", SHEET_TILES, 12, 1, VARIANCE_CLAY);
	registerTile(ID_STONEWALL, "Stone Wall", SHEET_TILES, 13, 1, VARIANCE_STONEWALL);
	registerTile(ID_SNOW, "Snow", SHEET_TILES, 14, 1, VARIANCE_SNOW);
	registerTile(ID_OBSIDIAN, "Obsidian", SHEET_TILES, 15, 1, VARIANCE_OBSIDIAN);
	registerTile(ID_ICE, "Ice", SHEET_TILES, 16, 1, VARIANCE_ICE);
*/
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

	registerTerrain(TERRAIN_BEACH, "Beach");
	registerTerrain(TERRAIN_CONIFEROUS, "Coniferous");
	registerTerrain(TERRAIN_GLACIER, "Glacier");
	registerTerrain(TERRAIN_ICY, "Icy");
	registerTerrain(TERRAIN_JUNGLE, "Jungle");
	registerTerrain(TERRAIN_MEADOW, "Meadow");
	registerTerrain(TERRAIN_SNOWY, "Snowy");
	registerTerrain(TERRAIN_STREAM, "Stream");

	addTerrainLandTile(TERRAIN_BEACH, ID_WATER);
	addTerrainLandTile(TERRAIN_BEACH, ID_GREENGRASS);
	addTerrainLandTile(TERRAIN_BEACH, ID_EMERALDGRASS);
	addTerrainLandTile(TERRAIN_BEACH, ID_MARBLE);
	addTerrainAccentTile(TERRAIN_BEACH, ID_OBSIDIAN);
	addTerrainAccentTile(TERRAIN_BEACH, ID_SAND);
	addTerrainAccentTile(TERRAIN_BEACH, ID_CLAY);
	addTerrainWornTile(TERRAIN_BEACH, ID_WATER);
	addTerrainDeco(TERRAIN_BEACH, ID_MAHOGANYLOG);
	addTerrainShape(TERRAIN_BEACH, SHAPE_CIRCLE);
	addTerrainShape(TERRAIN_BEACH, SHAPE_BOX);
	addTerrainShape(TERRAIN_BEACH, SHAPE_PARABOLIC);
	addTerrainShape(TERRAIN_BEACH, SHAPE_CONIC);

}