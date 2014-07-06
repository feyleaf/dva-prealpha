/*
gameobject.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 31, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
This class header defines the routines and members to contain pointers to objects that exist in the world
Using vector containers we are able to index an object based on its 'serial number': the location of
the pointer inside the vector list. This allows for quick and efficient loading, however, there is no
search algorithm or tree structure to optimize the searching of values (yet).
*/

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

struct registeredTile
{
	int tileTemplateIndex;
	coord origin;
	coord dimensions;
	int frame;
	coord pos;
	sf::Color distortionColor;
	registeredTile(int _index, coord _orig, coord _dim, coord _pos) {tileTemplateIndex=_index; origin=_orig; dimensions=_dim; frame=0; pos=_pos; distortionColor=sf::Color::White;}
};

struct registeredEntity
{
	bool active;
	unsigned char type;
	int entityTemplateIndex;
	int packIndex;
	coord origin;
	coord dimensions;
	sf::IntRect box;
	int frame;
	coord pos;
	int plane; //0 for the world plane, 1 for inventory, 2 for registered entities inside of other entities (ie loot or summon)
	registeredEntity(int _index, unsigned char _type, int _pack, coord _orig, coord _dim, sf::IntRect _box, coord _pos)
	{active=true; type=_type; entityTemplateIndex=_index; packIndex=_pack; origin=_orig; dimensions=_dim; frame=0; box=_box; pos=_pos; plane=0;}
};

struct buttonStruct
{
	bool active;
	int actionTemplateIndex;
	coord origin;
	coord dimensions;
	sf::IntRect box;
	int frame;
	coord pos;
	buttonStruct(bool act, int _index, coord _orig, coord _dim, sf::IntRect _box, coord _pos)
	{active=act; actionTemplateIndex=_index; origin=_orig; dimensions=_dim; frame=0; box=_box; pos=_pos;}
};

struct toolPack
{
	bool active;
	unsigned int maxUses;
	unsigned int usesLeft;
	unsigned char usageProtocol;
	toolPack()
	{active=false; maxUses=0; usesLeft=0; usageProtocol=0;}
	toolPack(unsigned int _max, unsigned int _left, unsigned char _proto)
	{active=true; maxUses=_max; usesLeft=_left; usageProtocol=_proto;}
	toolPack(const toolPackTemplate& src)
	{active=true; maxUses=src.maxUses; usesLeft=src.maxUses; usageProtocol=src.usageProtocol;}
};

struct ingredientPack
{ //this is superfluous i probably don't need it?
	bool active;
	unsigned char usageProtocol;
	ingredientPack()
	{active=false; usageProtocol=0;}
	ingredientPack(unsigned char _proto)
	{active=true; usageProtocol=_proto;}
	ingredientPack(const ingredientPackTemplate& src)
	{active=true; usageProtocol=src.usageProtocol;}
};

struct seedPack
{
	bool active;
	int vegetationContained;
	unsigned char usageProtocol;
	seedPack()
	{active=false; vegetationContained=0; usageProtocol=0;}
	seedPack(int _veg, unsigned char _proto)
	{active=true; vegetationContained=_veg; usageProtocol=_proto;}
	seedPack(int vegIndex, const seedPackTemplate& src)
	{active=true; vegetationContained=vegIndex; usageProtocol=src.usageProtocol;}
};

struct summonPack
{
	bool active;
	int creatureContained;
	unsigned char usageProtocol;
	summonPack()
	{active=false; creatureContained=0; usageProtocol=0;}
	summonPack(int _creat, unsigned char _proto)
	{active=true; creatureContained=_creat; usageProtocol=_proto;}
	summonPack(int creIndex, const summonPackTemplate& src)
	{active=true; creatureContained=creIndex; usageProtocol=src.usageProtocol;}
};

struct vegPack
{
	bool active;
	float bornTime;
	int growthTicks;
	int maxGrowth;
	int currentGrowth;
	unsigned char mapBonus;
	int dropList; //the template index to the list
	vegPack()
	{active=false; bornTime=0.0f; growthTicks=0; maxGrowth=0; currentGrowth=0; mapBonus=0; dropList=0;}
	vegPack(float _born, int _ticks, int _max, int _stage, unsigned char _bonus)
	{active=false; bornTime=_born; growthTicks=_ticks; maxGrowth=_max; currentGrowth=0; mapBonus=_bonus; dropList=0;}
	vegPack(float curTime, const vegetationPackTemplate& src)
	{active=false; bornTime=curTime; growthTicks=src.growthTicks; maxGrowth=src.maxGrowthStages; currentGrowth=0; mapBonus=src.mapBonus; dropList=0;}
};

struct creaturePack
{
	bool active;
	int maxHP;
	int currentHP;
	int attack;
	int defense;
	int agility;
	int move;
	creaturePack()
	{active=false; maxHP=0; currentHP=0, attack=0; defense=0; agility=0; move=0;}
	creaturePack(int _max, int _hp, int _atk, int _def, int _agi, int _move)
	{active=true; maxHP=_max; currentHP=_hp, attack=_atk; defense=_def; agility=_agi; move=_move;}
	creaturePack(const creaturePackTemplate& src)
	{active=true; maxHP=src.maxHP; currentHP=src.maxHP, attack=src.attack; defense=src.defense; agility=src.agility; move=src.moveSpeed;}
};

struct decoPack
{
	bool active;
	int maxHP;
	int currentHP;
	int defense;
	int element;
	std::vector<int> lootList;
	decoPack()
	{active=false; maxHP=0; currentHP=0; defense=0; element=0; lootList.clear();}
	decoPack(int _max, int _hp, int _def, int _elem)
	{active=true; maxHP=_max; currentHP=_hp; defense=_def; element=_elem; lootList.clear();}
	decoPack(const decoPackTemplate& src)
	{active=true; maxHP=src.maxHP; currentHP=src.maxHP; defense=src.defense; element=src.element; lootList.clear();}
};

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

struct mapShapeStruct
{
	char shapeName[32];
	coord tl;
	coord br;
	int terrainTiles;
	mapShapeStruct() {strcpy_s(shapeName, ""); tl=coord(0,0); br=coord(0,0); terrainTiles=0;}
};

struct mapSpreadStruct
{
	int entityTemplateIndex;
	int density;
	coord tl;
	coord br;
	mapSpreadStruct() {entityTemplateIndex=0; density=0; tl=coord(0,0); br=coord(0,0);}
};

struct mapGenStruct
{
	bool active;
	bool displaying;
	coord worldCoords;
	int baseTiles;
	std::vector<mapShapeStruct*> shapeLayer;
	std::vector<mapSpreadStruct*> decoLayer;
	std::vector<mapSpreadStruct*> vegLayer;
	std::vector<mapSpreadStruct*> creatureLayer;
	mapGenStruct() {active=true; displaying=false; worldCoords=coord(0,0); baseTiles=0; shapeLayer.clear(); shapeLayer.push_back(NULL);
		decoLayer.clear(); decoLayer.push_back(NULL); vegLayer.clear(); vegLayer.push_back(NULL); creatureLayer.clear(); creatureLayer.push_back(NULL);}
};

class GameObjectContainerClass
{
public:
	GameObjectContainerClass();
	~GameObjectContainerClass() {}

	std::vector<registeredTile*> regTiles;
	std::vector<registeredEntity*> regEntities;
	std::vector<vegPack*> regVeg;
	std::vector<decoPack*> regDeco;
	std::vector<seedPack*> regSeed;
	std::vector<summonPack*> regSummon;
	std::vector<toolPack*> regTool;
	std::vector<creaturePack*> regCreature;
	std::vector<ingredientPack*> regIng;
	std::vector<actionStruct*> actions;
	std::vector<buttonStruct*> regButtons;
	std::vector<mapGenStruct*> regMaps;

	std::vector<int> listHandler;
	void init();

	void eraseEntity(int entityIndex);

	int numberOfTilesOnGrid(coord _grid);
	int numberOfEntitiesOnGrid(coord _grid);

	int getEntityTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename);
	int getTileTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename);
	int getGuiTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename);
	int getButtonForAction(const TemplateRegistryClass& tmp, const char* _codename);
	int getListTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename);
	int getTerrainTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename);
	int getEcologyTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename);
	int getBiomeTemplateIndex(const TemplateRegistryClass& tmp, const char* _codename);
	std::string randomShapeFromList(const TemplateRegistryClass& tmp, const char* _codename);

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

	GameObjectContainerClass obj;

	bool createTile(const TemplateRegistryClass& tmp, const char* _name, coord _pos, int _con, long _seed);
	bool createEntity(const TemplateRegistryClass& tmp, const char* _name, coord _pos, float time);
	bool createAction(const TemplateRegistryClass& tmp, const char* _name, int entitySrc, int entityTrg, int tileTrg, float time);
	bool createButton(const TemplateRegistryClass& tmp, const char* name, coord _pos, bool act=true);
	bool createMapTerrain(const TemplateRegistryClass& tmp, const char* _biomeName);
	sf::Color getTileDistortion(const colorVarianceTemplate& var, coord _pos, int con, long seed);
	void clear();
	void cloneToInventory(int entityIndex);
	void cloneFromInventory(int entityIndex, coord _pos);
};

#endif