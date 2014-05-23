/*
gameobject.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 19, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
This class header defines the routines and members to contain pointers to objects that exist in the world
Using vector containers we are able to index an object based on its 'serial number': the location of
the pointer inside the vector list. This allows for quick and efficient loading, however, there is no
search algorithm or tree structure to optimize the searching of values (yet).
*/

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

struct tileObjectStruct
	//tmp is the template information, the base data for the object
	//grid is the location of the map
	//curColor is the calculated random distortion color to paint the texture with
{
	tileTemplate tmp;
	coord grid;
	sf::Color curColor;
	tileObjectStruct() {
		tmp.id=0; tmp.iconRange=0; strcpy_s(tmp.name, "Undef"); tmp.sheet=SHEET_TILES; tmp.sheetOrigin=0; tmp.variance=VARIANCE_NONE;
		grid=coord(0,0);
		curColor = sf::Color::White;
	}
	tileObjectStruct(tileTemplate& tmpsrc, coord _grid = coord(0,0)) {
		tmp = tileTemplate(tmpsrc);
		grid=_grid;
		curColor = sf::Color::White;

	}
	tileObjectStruct(tileObjectStruct& objsrc) {
		tmp = tileTemplate(objsrc.tmp);
		grid=objsrc.grid;
		curColor = objsrc.curColor;

	}
};
struct entityObjectStruct
	//tmp is the template information, the base data for the object
	//grid is the location of the map
	//curColor is the calculated random distortion color to paint the texture with
{
	entityTemplate tmp;
	coord grid;
	entityObjectStruct() {
		tmp.id=0; tmp.type=ET_NONE; tmp.iconRange=0; strcpy_s(tmp.name, "Undef"); tmp.sheet=SHEET_TILES; tmp.sheetOrigin=0; tmp.creationProtocol=CPROTO_NONE;
		grid=coord(0,0);
	}
	entityObjectStruct(entityTemplate& tmpsrc, coord _grid = coord(0,0)) {
		tmp = entityTemplate(tmpsrc);
		grid=_grid;
	}
	entityObjectStruct(entityObjectStruct& objsrc) {
		tmp = entityTemplate(objsrc.tmp);
		grid=objsrc.grid;
	}
};

class GameObjectClass
{
public:
	GameObjectClass();
	~GameObjectClass(){}

	ValueRegistryClass templateRegistry;
	std::vector<colorVarianceTemplate*> varianceList;
	std::vector<tileObjectStruct*> regTiles;
	std::vector<entityObjectStruct*> regEntities;

	int cloneTile(const unsigned int tileID, coord _grid, int con = 0);
	int cloneEntity(const unsigned int entityID, coord _grid);
	tileObjectStruct newTile(tileTemplate _t, coord _grid, int con = 0);
	entityObjectStruct newEntity(entityTemplate _t, coord _grid);
	sf::Color getTileDistortion(const colorVarianceTemplate* _var, coord _pos, int con, long seed);

};

#endif