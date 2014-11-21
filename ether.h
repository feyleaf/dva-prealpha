/*
ether.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
September 11, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
This class header defines the routines and members to contain pointers to objects that exist in the ether,
a container for indexes of template-registered objects which exist in any location inside the game.
This includes maps, GUIs, and inventories.
This class heavily uses the template registry, and requires that the parser has already
registered the templates before use.
Using vector containers we are able to index an object based on its 'serial number': the location of
the pointer inside the ethereal vector list. This allows for quick and efficient loading, however, there is no
search algorithm or tree structure to optimize the searching of values (yet).
*/

#ifndef ETHER_H
#define ETHER_H

#define MAPMODE_FRESH 0
#define MAPMODE_SANCTUARY 1
#define MAPMODE_BATTLE 2
#define MAPMODE_THREATEN 3
#define MAPMODE_OVERTAKEN 4

#define PRINT_FADEUP 1
#define PRINT_PANIN 2
#define PRINT_FADESTILL 3


struct registeredTile
{
	int tileTemplateIndex;
	coord origin;
	coord dimensions;
	int frame;
	coord pos;
	sf::Color distortionColor;
	sf::Color minimapColor;
	registeredTile(int _index, coord _orig, coord _dim, coord _pos) {tileTemplateIndex=_index; origin=_orig; dimensions=_dim; frame=0; pos=_pos; distortionColor=sf::Color::White; minimapColor=sf::Color::White;}
};

struct registeredEntity
{
	bool active;
	unsigned char type;
	bool isEnemy;
	int entityTemplateIndex;
	int packIndex;
	coord origin;
	coord dimensions;
	coord cog;
	sf::IntRect box;
	int frame;
	coord pos;
	int plane; //0 for the world plane, 1 for inventory, 2 for registered entities inside of other entities (ie loot or summon)
	registeredEntity(int _index, unsigned char _type, int _pack, coord _orig, coord _dim, coord _cog, sf::IntRect& _box, coord _pos)
	{active=true; type=_type; isEnemy=false; entityTemplateIndex=_index; packIndex=_pack; origin=_orig; dimensions=_dim; cog=_cog; frame=0; box=_box; pos=_pos; plane=0;}
};

struct buttonStruct
{
	bool active;
	bool hidden;
	int linkedEntityIndex;
	int actionTemplateIndex;
	coord origin;
	coord dimensions;
	sf::IntRect box;
	int frame;
	coord pixel;
	buttonStruct(bool act, int _linked, int _index, coord _orig, coord _dim, sf::IntRect _box, coord _pixel)
	{active=act; hidden=false; linkedEntityIndex=_linked; actionTemplateIndex=_index; origin=_orig; dimensions=_dim; frame=0; box=_box; pixel=_pixel;}
};

struct toolPack
{
	bool active;
	int maxUses;
	int usesLeft;
	unsigned char usageProtocol;
	toolPack()
	{active=false; maxUses=0; usesLeft=0; usageProtocol=0;}
	toolPack(int _max, int _left, unsigned char _proto)
	{active=true; maxUses=_max; usesLeft=_left; usageProtocol=_proto;}
	toolPack(const toolPackTemplate& src)
	{active=true; maxUses=src.maxUses; usesLeft=src.maxUses; usageProtocol=0;}
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
	{active=true;  usageProtocol=0;}
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
	seedPack(const seedPackTemplate& src)
	{active=true; vegetationContained=0;  usageProtocol=0;}
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
	summonPack(const summonPackTemplate& src)
	{active=true; creatureContained=0;  usageProtocol=0;}
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
	coord offset;
	coord velocity;
	creaturePack()
	{active=false; maxHP=0; currentHP=0, attack=0; defense=0; agility=0; move=0;offset=coord(0,0);velocity=coord(0,0);}
	creaturePack(int _max, int _hp, int _atk, int _def, int _agi, int _move)
	{active=true; maxHP=_max; currentHP=_hp, attack=_atk; defense=_def; agility=_agi; move=_move;offset=coord(0,0);velocity=coord(0,0);}
	creaturePack(const creaturePackTemplate& src)
	{active=true; maxHP=src.maxHP; currentHP=src.maxHP, attack=src.attack; defense=src.defense; agility=src.agility; move=src.moveSpeed;offset=coord(0,0);velocity=coord(0,0);}
};

struct decoPack
{
	bool active;
	bool isTree;
	int maxHP;
	int currentHP;
	int defense;
	int element;
	std::vector<int> lootList;
	decoPack()
	{active=false; isTree=false; maxHP=0; currentHP=0; defense=0; element=0; lootList.clear();}
	decoPack(int _max, int _hp, int _def, int _elem)
	{active=true; isTree=false; maxHP=_max; currentHP=_hp; defense=_def; element=_elem; lootList.clear();}
	decoPack(const decoPackTemplate& src)
	{active=true; isTree=src.isTree; maxHP=src.maxHP; currentHP=src.maxHP; defense=src.defense; element=src.element; lootList.clear();}
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
	int mapMode;
	bool displaying;
	coord worldCoords;
	int baseTiles;
	char showName[40];
	std::vector<mapShapeStruct*> shapeLayer;
	std::vector<mapSpreadStruct*> decoLayer;
	std::vector<mapSpreadStruct*> vegLayer;
	std::vector<mapSpreadStruct*> creatureLayer;
	mapGenStruct() {active=true; mapMode=MAPMODE_FRESH; displaying=false; worldCoords=coord(0,0); baseTiles=0; shapeLayer.clear(); shapeLayer.push_back(NULL);
		decoLayer.clear(); decoLayer.push_back(NULL); vegLayer.clear(); vegLayer.push_back(NULL); creatureLayer.clear(); creatureLayer.push_back(NULL); strcpy_s(showName, "");}
};

struct textStruct
{
	bool active;
	int printType;
	int alphamax;
	int alphacur;
	sf::Color color;
	sf::Text msg;
	textStruct() {active=false; printType=0; alphamax=255; alphacur=255; color=sf::Color::White; msg.setCharacterSize(30); msg.setString("");}
	textStruct(const textStruct& src) {msg.setFont(*src.msg.getFont()); active=true; printType=src.printType; alphamax=src.alphamax; alphacur=src.alphacur; color=src.msg.getColor(); msg.setColor(color); msg.setCharacterSize(30); msg.setString(src.msg.getString());}
	void setFontSize(unsigned int sz) {msg.setCharacterSize(sz);}
};

class EtherRegistryClass
{
public:
	EtherRegistryClass();
	~EtherRegistryClass();

	std::vector<registeredTile*> regTiles;
	std::vector<registeredEntity*> regEntities;
	std::vector<vegPack*> regVeg;
	std::vector<decoPack*> regDeco;
	std::vector<seedPack*> regSeed;
	std::vector<summonPack*> regSummon;
	std::vector<toolPack*> regTool;
	std::vector<creaturePack*> regCreature;
	std::vector<ingredientPack*> regIng;
	std::vector<buttonStruct*> regButtons;
	std::vector<mapGenStruct*> regMaps;
	std::vector<textStruct*> regText;

	int createTile(const TemplateRegistryClass& tmp, const char* _codename);
	int createEntity(const TemplateRegistryClass& tmp, const char* _codename);
	int createButton(const TemplateRegistryClass& tmp, const char* _codename, int linkedEntity=0);
	int createMapTerrain(const TemplateRegistryClass& tmp, const char* _biomeName);
	int createString(const char* string, sf::Color& _color, sf::Font& _font, int pType, int _alphamax);

	int randomTileFromList(const TemplateRegistryClass& tmp, const char* _codename);
	int randomEntityFromList(const TemplateRegistryClass& tmp, const char* _codename);
	std::string randomShapeFromList(const TemplateRegistryClass& tmp, const char* _codename);

};

#endif//ETHER_H