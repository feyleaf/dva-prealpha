/*
templates.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 19, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#ifndef TEMPLATES_H
#define TEMPLATES_H

struct settingStruct
{
	int tileWid;
	int tileHig;
	int tileRows;
	int tileCols;
	int winWid;
	int winHig;

	int hotbarSize;
	int invWid;
	int invHig;
	char verTitle[40];
	char verTx[40];
	char saveFile[40];
	char mainFontFile[40];
	char auxFontFile[40];
	char tileSheetFile[40];
	char entitySheetFile[40];
	char guiSheetFile[40];
};

//constructed template rules for tile creation
struct tileTemplate 
// selection of non-unique tile templates used in map creation and rendering
// registered as the basis of unique tiles in the map generation and storage
{
	char cname[16];
	char name[16];
	coord dimensions;
	char spritefile[40];
	unsigned char iconRange;
	unsigned char variance;
	tileTemplate(const char* _cname,
					const char* _name,
					coord _dimensions,
					const char* _spritefile,
					unsigned char _iconRange,
					unsigned char _variance)
	{strncpy_s(cname, 16, _cname, 16);  strncpy_s(name, 16, _name, 16); dimensions=_dimensions; strncpy_s(spritefile, 40, _spritefile, 40); iconRange=_iconRange; variance=_variance;}
	tileTemplate()
	{strncpy_s(cname, 16, "none", 16);  strcpy_s(name, "Undef"); dimensions=coord(0,0); strncpy_s(spritefile, 40, "tiles-1_2.png", 40); iconRange=0; variance=0;}
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
	entityTemplate(unsigned int _id,
					unsigned char _type,
					const char* _name,
					unsigned char _sheet,
					unsigned char _sheetOrigin,
					unsigned char _iconRange,
					unsigned char _creationProtocol)
	{id=_id; type=_type; strncpy_s(name, 40, _name, 40); sheet=_sheet; sheetOrigin=_sheetOrigin; iconRange=_iconRange; creationProtocol=_creationProtocol;}
	entityTemplate()
	{id=0; type=0; strncpy_s(name, 40, "Undef", 40); sheet=0; sheetOrigin=0; iconRange=0; creationProtocol=0;}
};//size 52 bytes

//constructed template rules for varying colors on map tiles
struct colorVarianceTemplate
// selection of color variance values for use in the ic_randomTile distortion algorithm
{
	char cname[32];
	unsigned char redBase;
	unsigned char redRange;
	unsigned char greenBase;
	unsigned char greenRange;
	unsigned char blueBase;
	unsigned char blueRange;
	unsigned char whiteBase;
	unsigned char whiteRange;
	colorVarianceTemplate(const char* _name, unsigned char _redBase,
							unsigned char _redRange,
							unsigned char _greenBase,
							unsigned char _greenRange,
							unsigned char _blueBase,
							unsigned char _blueRange,
							unsigned char _whiteBase,
							unsigned char _whiteRange)
	{strcpy_s(cname, _name); redBase=_redBase; redRange=_redRange;
	greenBase=_greenBase; greenRange=_greenRange;
	blueBase=_blueBase; blueRange=_blueRange;
	whiteBase=_whiteBase; whiteRange=_whiteRange;}
	colorVarianceTemplate()
	{strcpy_s(cname, "Undef"); redBase=0; redRange=0;
	greenBase=0; greenRange=0;
	blueBase=0; blueRange=0;
	whiteBase=0; whiteRange=0;}
};//size 8 bytes

//constructed template of rules for directing tool creation
struct toolPackTemplate
{
	unsigned char id;
	unsigned int maxUses;
	unsigned char usageProtocol; //ie: what it works on, the input to give that subject
	toolPackTemplate(unsigned char _id,
					unsigned int _maxUses,
					unsigned char _usageProtocol)
	{id=_id; maxUses=_maxUses; usageProtocol=_usageProtocol;}
};

//constructed template of rules for directing seed creation
struct seedPackTemplate
{
	unsigned char id;
	unsigned char plantSummon;
	unsigned char usageProtocol;	//ie: allowed terrain
	seedPackTemplate(unsigned char _id,
						unsigned char _plantSummon,
						unsigned char _usageProtocol)
	{id=_id; plantSummon=_plantSummon; usageProtocol=_usageProtocol;}
};

//constructed template of rules for generating vegetation
struct vegetationPackTemplate
{
	unsigned char id;
	float growthTime;
	unsigned char maxGrowthStages;
	unsigned char mapBonus;
	std::vector<unsigned char> lootList;
	vegetationPackTemplate(unsigned char _id,
							float _growthTime,
							unsigned char _maxGrowthStages,
							unsigned char _mapBonus)
	{id=_id; growthTime=_growthTime; maxGrowthStages=_maxGrowthStages; mapBonus=_mapBonus;}
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
	creaturePackTemplate(unsigned char _id,
						unsigned int _maxHP,
						unsigned char _attack,
						unsigned char _defense,
						unsigned char _agility,
						unsigned char _moveSpeed)
	{id=_id; maxHP=_maxHP; attack=_attack; defense=_defense; agility=_agility; moveSpeed=_moveSpeed;}
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
	decoPackTemplate(unsigned char _id,
					unsigned int _maxHP,
					unsigned char _mapBonus,
					unsigned char _defense,
					unsigned char _element)
	{id=_id; maxHP=_maxHP; mapBonus=_mapBonus; defense=_defense; element=_element;}
};

//constructed template of rules for summon charm generation
struct summonPackTemplate
{
	unsigned char id;
	unsigned int registeredCreatureSummon;	//0 if empty, must hold the game-registered id
											//meaning create the creature first then the summon
											//or create it empty
	unsigned char usageProtocol;
	summonPackTemplate(unsigned char _id,
						unsigned int _registeredCreatureSummon,
						unsigned char _usageProtocol)
	{id=_id; registeredCreatureSummon=_registeredCreatureSummon; usageProtocol=_usageProtocol;}
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
	terrainPoolTemplate(unsigned char _id, const char* _name)
	{
		id=_id;
		strncpy_s(name, _name, 16);
		landTilesList.clear();
		wornTilesList.clear();
		accentTilesList.clear();
		decoEntitiesList.clear();
		shapesList.clear();
	}
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

//constructed template of update processes
struct actionTemplate
{
	unsigned int protocol;
	unsigned char category;
	float coolDown;
	unsigned char priority;
	unsigned char restrictions;
	actionTemplate(unsigned int _protocol,
				unsigned char _category,
				float _coolDown,
				unsigned char _priority,
				unsigned char _restrictions)
	{protocol=_protocol; category=_category; coolDown=_coolDown; priority=_priority; restrictions=_restrictions;}
};

//constructed template of buttons
struct guiButtonTemplate
{
	unsigned int actionID;
	char name[32];				//button will need a name
	unsigned char sheet;
	unsigned char sheetOrigin;
	unsigned char iconRange;	//animating and state
	unsigned char state;
	sf::IntRect placement;
	guiButtonTemplate(unsigned int _actionID,
						const char* _name,
						unsigned char _sheet,
						unsigned char _sheetOrigin,
						unsigned char _iconRange,
						unsigned char _state,
						sf::IntRect _placement)
	{actionID=_actionID; strncpy_s(name, 32, _name, 32); sheet=_sheet; sheetOrigin=_sheetOrigin; iconRange=_iconRange; state=_state; placement=_placement;}
};

//template of forms using multiple gui buttons
struct guiFormsTemplate
{
	unsigned char id;
	char name[16];
	std::vector<guiButtonTemplate> buttonList;
};

#endif//TEMPLATES_H