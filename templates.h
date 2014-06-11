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
	char cname[32];
	char name[32];
	coord dimensions;
	char spritefile[40];
	unsigned char iconRange;
	char var_codename[32];
	unsigned char variance;
	tileTemplate(const char* _cname,
					const char* _name,
					coord _dimensions,
					const char* _spritefile,
					unsigned char _iconRange,
					const char* _varcode,
					unsigned char _variance)
	{strncpy_s(cname, 32, _cname, 32);  strncpy_s(name, 32, _name, 32); dimensions=_dimensions; strncpy_s(spritefile, 40, _spritefile, 40); iconRange=_iconRange; strncpy_s(var_codename, 32, _varcode, 32); variance=_variance;}
	tileTemplate()
	{strncpy_s(cname, 32, "none", 32);  strcpy_s(name, "Undef"); dimensions=coord(0,0); strncpy_s(spritefile, 40, "tiles-1_2.png", 40); iconRange=0; strcpy_s(var_codename, "undef"); variance=0;}
};//size 24 bytes

//constructed template rules for entity creation
struct entityTemplate 
// selection of non-unique entity templates used in entity creation and rendering
// registered as the basis of unique objects in the gameplay and storage
// creationProtocol is the index to a set of rules that are applied to the
// entity's own data during creation, which requires specifics outside of the
// template scope (such as the HP of a creature, growth rate of a plant, etc.)
{
	char cname[40];
	unsigned char type;
	char name[40];
	coord dimensions;
	sf::IntRect box;
	char spritefile[40];
	unsigned char iconRange;
	char creation[32];
	unsigned char creationProtocol;
	entityTemplate(const char* _cname,
					unsigned char _type,
					const char* _name,
					const char* _spriteFile,
					unsigned char _iconRange,
					const char* _creation,
					unsigned char _creationProtocol)
	{strncpy_s(cname, 40, _cname, 40); type=_type; strncpy_s(name, 40, _name, 40); strncpy_s(spritefile, 40, _spriteFile, 40); iconRange=_iconRange; strncpy_s(creation, 32, _creation, 32); creationProtocol=_creationProtocol;}
	entityTemplate()
	{strncpy_s(cname, 40, "undef", 40); type=0; strncpy_s(name, 40, "Undef", 40); strncpy_s(spritefile, 40, "entity-1_2.png", 40); iconRange=0; strcpy_s(creation, "undef"); creationProtocol=0;}
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
	colorVarianceTemplate(colorVarianceTemplate& src)
	{strcpy_s(cname, src.cname); redBase=src.redBase; redRange=src.redRange;
	greenBase=src.greenBase; greenRange=src.greenRange;
	blueBase=src.blueBase; blueRange=src.blueRange;
	whiteBase=src.whiteBase; whiteRange=src.whiteRange;}
};//size 8 bytes

//constructed template of rules for directing tool creation
struct toolPackTemplate
{
	int entityID;
	unsigned int maxUses;
	unsigned char usageProtocol; //ie: what it works on, the input to give that subject
	toolPackTemplate(int _id,
					unsigned int _maxUses,
					unsigned char _usageProtocol)
	{entityID=_id; maxUses=_maxUses; usageProtocol=_usageProtocol;}
	toolPackTemplate()
	{entityID=0; maxUses=0; usageProtocol=0;}
};

//constructed template of rules for directing seed creation
struct seedPackTemplate
{
	int entityID;
	unsigned char plantSummon;
	unsigned char usageProtocol;	//ie: allowed terrain
	seedPackTemplate(int _id,
						unsigned char _plantSummon,
						unsigned char _usageProtocol)
	{entityID=_id; plantSummon=_plantSummon; usageProtocol=_usageProtocol;}
	seedPackTemplate()
	{entityID=0; plantSummon=0; usageProtocol=0;}
};

//constructed template of rules for generating vegetation
struct vegetationPackTemplate
{
	int entityID;
	int growthTicks;
	unsigned char maxGrowthStages;
	unsigned char mapBonus;
	std::vector<unsigned char> lootList;
	vegetationPackTemplate(int _id,
							int _growthTime,
							unsigned char _maxGrowthStages,
							unsigned char _mapBonus)
	{entityID=_id; growthTicks=_growthTime; maxGrowthStages=_maxGrowthStages; mapBonus=_mapBonus; lootList.clear();}
	vegetationPackTemplate()
	{entityID=0; growthTicks=0; maxGrowthStages=0; mapBonus=0; lootList.clear();}
};

//constructed template of rules for generating creatures
struct creaturePackTemplate
{
	int entityID;
	unsigned int maxHP;
	unsigned char attack;
	unsigned char defense;
	unsigned char agility;
	unsigned char moveSpeed;
	//IDK std::vector<unsigned char> actionList;
	creaturePackTemplate(int _id,
						unsigned int _maxHP,
						unsigned char _attack,
						unsigned char _defense,
						unsigned char _agility,
						unsigned char _moveSpeed)
	{entityID=_id; maxHP=_maxHP; attack=_attack; defense=_defense; agility=_agility; moveSpeed=_moveSpeed;}
	creaturePackTemplate()
	{entityID=0; maxHP=0; attack=0; defense=0; agility=0; moveSpeed=0;}
};

//constructed template of rules for decoration generation
struct decoPackTemplate
{
	int entityID;
	unsigned int maxHP;
	unsigned char mapBonus; //will link to another list, unused for now
	unsigned char defense;
	unsigned char element;	//will link to another list, unused for now
	std::vector<unsigned char> lootList;	//a list of entity index or nothing. empty for now
	decoPackTemplate(int _id,
					unsigned int _maxHP,
					unsigned char _mapBonus,
					unsigned char _defense,
					unsigned char _element)
	{entityID=_id; maxHP=_maxHP; mapBonus=_mapBonus; defense=_defense; element=_element; lootList.clear();}
	decoPackTemplate()
	{entityID=0; maxHP=0; mapBonus=0; defense=0; element=0; lootList.clear();}
};

//constructed template of rules for summon charm generation
struct summonPackTemplate
{
	int entityID;
	unsigned int registeredCreatureSummon;	//0 if empty, must hold the **game-registered** id
											//meaning create the creature first then the summon
											//or create it empty
	unsigned char usageProtocol;
	summonPackTemplate(int _id,
						unsigned int _registeredCreatureSummon,
						unsigned char _usageProtocol)
	{entityID=_id; registeredCreatureSummon=_registeredCreatureSummon; usageProtocol=_usageProtocol;}
	summonPackTemplate()
	{entityID=0; registeredCreatureSummon=0; usageProtocol=0;}
};

//constructed template of rules for summon charm generation
struct ingredientPackTemplate
{
	int entityID;
	unsigned char usageProtocol;
	ingredientPackTemplate(int _id,
						unsigned char _usageProtocol)
	{entityID=_id; usageProtocol=_usageProtocol;}
	ingredientPackTemplate()
	{entityID=0; usageProtocol=0;}
};

//compiled container of terrain pools in a template format, used in map generation
struct terrainPoolTemplate
{
	char cname[40];
	char name[16];
	std::vector<unsigned char> landTilesList;
	std::vector<unsigned char> wornTilesList;
	std::vector<unsigned char> accentTilesList;
	std::vector<unsigned char> decoEntitiesList;
	std::vector<unsigned char> shapesList;
	terrainPoolTemplate(const char* _cname, const char* _name)
	{
		strncpy_s(cname, 40, _cname, 40);
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
	char cname[40];
	char name[16];
	std::vector<unsigned char> vegetationList;
	std::vector<unsigned char> seedList;
	std::vector<unsigned char> creatureList;
};

//compiled template of biome information used in map generation
struct biomeInformationTemplate
{
	char cname[40];
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
	char cname[40];
	unsigned char category;	//split between immediate actions, and queued actions 0=imm 1=que
	int coolDownTicks;			//number of ticks * 0.20f
	unsigned char priority; //greater number is more ugrent
	unsigned char restrictions;	//another linked vector that will be needed, no value now
	actionTemplate(const char* _cname,
				unsigned char _category,
				int _coolDown,
				unsigned char _priority,
				unsigned char _restrictions)
	{strncpy_s(cname, 40, _cname, 40); category=_category; coolDownTicks=_coolDown; priority=_priority; restrictions=_restrictions;}
	actionTemplate()
	{strcpy_s(cname, "undef"); category=0; coolDownTicks=0; priority=0; restrictions=0;}
};

//constructed template of buttons
struct guiButtonTemplate
{
	char cname[40];
	char name[32];				//button will need a name
	unsigned int actionID;
	unsigned char sheet;
	unsigned char sheetOrigin;
	unsigned char iconRange;	//animating and state
	unsigned char state;
	sf::IntRect placement;
	guiButtonTemplate(const char* _cname,
						const char* _name,
						unsigned int _action,
						unsigned char _sheet,
						unsigned char _sheetOrigin,
						unsigned char _iconRange,
						unsigned char _state,
						sf::IntRect _placement)
	{strncpy_s(cname, 40, _cname, 40); strncpy_s(name, 32, _name, 32); actionID=_action; sheet=_sheet; sheetOrigin=_sheetOrigin; iconRange=_iconRange; state=_state; placement=_placement;}
};

//template of forms using multiple gui buttons
struct guiFormsTemplate
{
	char cname[40];
	char name[16];
	std::vector<guiButtonTemplate> buttonList;
};

class TemplateContainerClass
{
	public:
		TemplateContainerClass() {
			varianceList.clear(); varianceList.push_back(colorVarianceTemplate());
			tileList.clear(); tileList.push_back(tileTemplate());
			actionList.clear(); actionList.push_back(actionTemplate());
			entityList.clear(); entityList.push_back(entityTemplate());
			decoPackList.clear(); decoPackList.push_back(decoPackTemplate());
			creaturePackList.clear(); creaturePackList.push_back(creaturePackTemplate());
			vegPackList.clear(); vegPackList.push_back(vegetationPackTemplate());
		}
		~TemplateContainerClass() {}

		std::vector<tileTemplate> tileList;
		std::vector<entityTemplate> entityList;
		std::vector<colorVarianceTemplate> varianceList;
		std::vector<toolPackTemplate> toolPackList;
		std::vector<seedPackTemplate> seedPackList;
		std::vector<vegetationPackTemplate> vegPackList;
		std::vector<creaturePackTemplate> creaturePackList;
		std::vector<decoPackTemplate> decoPackList;
		std::vector<summonPackTemplate> summonPackList;
		std::vector<ingredientPackTemplate> ingredientPackList;
		std::vector<terrainPoolTemplate> terrainList;
		std::vector<ecoPoolTemplate> ecoList;
		std::vector<biomeInformationTemplate> biomeList;
		std::vector<itemTemplate> itemList;
		std::vector<actionTemplate> actionList;
		std::vector<guiButtonTemplate> buttonList;
		std::vector<guiFormsTemplate> formsList;
};

#endif//TEMPLATES_H