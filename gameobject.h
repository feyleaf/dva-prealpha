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

#define MAPMODE_NEUTRAL 0
#define MAPMODE_SANCTUARY 1
#define MAPMODE_BATTLE 2
#define MAPMODE_THREATEN 3

struct actionStruct
{
	bool active;
	bool queued;
	int actionTemplateIndex;
	int entityIndexSource;
	int entityIndexTarget;
	int tileIndexTarget;
	int priority;
	float timeToActivate;
};

class GameObjectContainerClass
{
public:
	GameObjectContainerClass();
	~GameObjectContainerClass();
	std::vector<actionStruct*> actions;

	sf::Texture mapSheet;

	std::vector<int> listHandler;
	void init();

	void eraseEntity(int entityIndex);

	int numberOfTilesOnGrid(coord _grid);
	int numberOfEntitiesOnGrid(coord _grid);
	int tileIndexOnGrid(coord _grid);
	int entityIndexOnGrid(coord _grid);
	int entityIndexAtPoint(coord _pixel);
	int buttonIndexAtPoint(coord _pixel);

	int getEntityTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename);
	int getActionTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename);
	int getTileTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename);
	int getGuiTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename);
	int getButtonForAction(const TemplateRegistryClass& tmp, const char* _codename);
	int getButtonLinkedEntity(const TemplateRegistryClass& tmp, const char* _codename);
	int getListTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename);
	int getTerrainTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename);
	int getEcologyTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename);
	int getBiomeTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename);
	std::string randomShapeFromList(const TemplateRegistryClass& tmp, const char* _codename);

	void activateEntityButtons(int entityIndex);
	void deactivateEntityButtons(int entityIndex);

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

	bool createTile(const TemplateRegistryClass& tmp, const char* _name, coord _pos, int _con, long _seed, coord worldCoord);
	int createEntity(const TemplateRegistryClass& tmp, const char* _name, coord _pos, float time, coord worldCoord);
	bool createAction(const TemplateRegistryClass& tmp, const char* _name, int entitySrc, int entityTrg, int tileTrg, float time, coord worldCoord);
	bool createButton(const TemplateRegistryClass& tmp, const char* name, coord _pos, coord worldCoord, int linkedEntity=0, bool act=true);
	bool createMapTerrain(const TemplateRegistryClass& tmp, const char* _biomeName, coord worldCoord);
	sf::Color getTileDistortion(const colorVarianceTemplate& var, coord _pos, int con, long seed);
	void clear(coord worldCursor);
	void cloneToInventory(int entityIndex, coord worldCoord);
	void cloneFromInventory(int entityIndex, coord _pos, coord worldCoord);
};

#endif