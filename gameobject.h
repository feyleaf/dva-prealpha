/*
gameobject.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
July 17, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
This class header defines the routines and members to contain pointers to objects that exist in the world
Using vector containers we are able to index an object based on its 'serial number': the location of
the pointer inside the vector list. This allows for quick and efficient loading, however, there is no
search algorithm or tree structure to optimize the searching of values (yet).
*/

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObjectContainerClass
{
public:
	GameObjectContainerClass();
	~GameObjectContainerClass();

	sf::Texture mapSheet;

	int mapgenerator;
	int mapMode;
	int biomeIndex;
	bool walked;
	bool generated;
	std::vector<int> tiles;
	std::vector<int> entities;
	std::vector<int> buttons;
	std::vector<int> strings;

	void init();
	void releaseEntityList();
	void releaseTileList();
	void releaseButtonList();

	void eraseEntity(int entityIndex);
	void eraseButton(int buttonIndex);

	int tileIndexOnGrid(EtherRegistryClass& _ether, coord _grid);
	int entityIndexOnGrid(EtherRegistryClass& _ether, coord _grid, int lower);
	int entityIndexAtPoint(EtherRegistryClass& _ether, coord _pixel);
	int buttonIndexAtPoint(EtherRegistryClass& _ether, coord _pixel);

	std::string randomShapeFromList(const TemplateRegistryClass& tmp, const char* _codename);

	//void activateEntityButtons(int entityIndex);
	//void deactivateEntityButtons(int entityIndex);

	void fillVegDropList(const TemplateRegistryClass& tmp, int packIndex, const char* _codename);

	int randomTileFromList(const TemplateRegistryClass& tmp, const char* _codename);
	int randomEntityFromList(const TemplateRegistryClass& tmp, const char* _codename);

	void handleIndexesOnGrid(coord _grid);
	void handleEntitiesList(int _index);
	void handleTypesList(int _catType);
	void handleTilesList(int _index);
};

class GameObjectClass
{
public:
	GameObjectClass();
	~GameObjectClass(){}

	std::map<coord, GameObjectContainerClass> objMap;

	sf::Color getTileDistortion(const colorVarianceTemplate& var, coord _pos, int con, long seed);
	void clear(coord worldCursor);
};

#endif