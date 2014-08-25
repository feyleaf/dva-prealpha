/*
templates.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
August 24, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#ifndef TEMPLATES_H
#define TEMPLATES_H

struct settingStruct
{
	coord tileDimensions;
	coord mapGridDimensions;
	coord screenDimensions;

	char verTitle[40];
	char verTx[40];
	char saveFile[40];
	char templateDataFile[40];
	char debugFile[40];
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
	coord origin;
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
	{strncpy_s(cname, 32, _cname, 32);  strncpy_s(name, 32, _name, 32); origin=coord(0,0); dimensions=_dimensions; strncpy_s(spritefile, 40, _spritefile, 40); iconRange=_iconRange; strncpy_s(var_codename, 32, _varcode, 32); variance=_variance;}
	tileTemplate()
	{strncpy_s(cname, 32, "none", 32);  strcpy_s(name, "Undef"); origin=coord(0,0); dimensions=coord(0,0); strncpy_s(spritefile, 40, "tiles-1_2.png", 40); iconRange=0; strcpy_s(var_codename, "undef"); variance=0;}
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
	coord origin;
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
	{strncpy_s(cname, 40, _cname, 40); type=_type; strncpy_s(name, 40, _name, 40); strncpy_s(spritefile, 40, _spriteFile, 40); origin=coord(0,0); iconRange=_iconRange; strncpy_s(creation, 32, _creation, 32); creationProtocol=_creationProtocol;}
	entityTemplate()
	{strncpy_s(cname, 40, "undef", 40); type=0; strncpy_s(name, 40, "Undef", 40); strncpy_s(spritefile, 40, "entity-1_2.png", 40); origin=coord(0,0); iconRange=0; strcpy_s(creation, "undef"); creationProtocol=0;}
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
	int maxUses;
	char usageProtocol[32]; //ie: what it works on, the input to give that subject
	toolPackTemplate(int _id,
					int _maxUses,
					const char* _usageProtocol)
	{entityID=_id; maxUses=_maxUses; strcpy_s(usageProtocol, 32, _usageProtocol);}
	toolPackTemplate()
	{entityID=0; maxUses=0; strcpy_s(usageProtocol, 32, "none");}
};

//constructed template of rules for directing seed creation
struct seedPackTemplate
{
	int entityID;
	char plantSummon[32];
	char usageProtocol[32];	//ie: allowed terrain
	seedPackTemplate(int _id,
						const char* _plantSummon,
						const char* _usageProtocol)
	{entityID=_id; strcpy_s(plantSummon, 32, _plantSummon);strcpy_s(usageProtocol, 32, _usageProtocol);}
	seedPackTemplate()
	{entityID=0; strcpy_s(usageProtocol, 32, "none");strcpy_s(usageProtocol, 32, "none");}
};

//constructed template of rules for generating vegetation
struct vegetationPackTemplate
{
	int entityID;
	int growthTicks;
	unsigned char maxGrowthStages;
	unsigned char mapBonus;
	char lootList[32];
	vegetationPackTemplate(int _id,
							int _growthTime,
							unsigned char _maxGrowthStages,
							unsigned char _mapBonus)
	{entityID=_id; growthTicks=_growthTime; maxGrowthStages=_maxGrowthStages; mapBonus=_mapBonus; strcpy_s(lootList, "");}
	vegetationPackTemplate()
	{entityID=0; growthTicks=0; maxGrowthStages=0; mapBonus=0; strcpy_s(lootList, "");}
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
	unsigned int regEntityIndex;	//0 if empty, must hold the **game-registered** id
									//meaning create the creature first then the summon
									//or create it empty
	char usageProtocol[32];
	summonPackTemplate(int _id,
						unsigned int _regEntityIndex,
						const char* _usageProtocol)
	{entityID=_id; regEntityIndex=_regEntityIndex; strcpy_s(usageProtocol, 32, _usageProtocol);}
	summonPackTemplate()
	{entityID=0; regEntityIndex=0; strcpy_s(usageProtocol, 32, "none");}
};

//constructed template of rules for summon charm generation
struct ingredientPackTemplate
{
	int entityID;
	char usageProtocol[32];
	ingredientPackTemplate(int _id,
						const char* _usageProtocol)
	{entityID=_id; strcpy_s(usageProtocol, 32, _usageProtocol);}
	ingredientPackTemplate()
	{entityID=0; strcpy_s(usageProtocol, 32, "none");}
};

//compiled container of terrain pools in a template format, used in map generation
struct terrainPoolTemplate
{
	char cname[32];
	char name[32];
	char landListName[32];
	char wornListName[32];
	char accentListName[32];
	char decoListName[32];
	char shapesListName[32];

	terrainPoolTemplate(const char* _cname, const char* _name)
	{
		strncpy_s(cname, _cname, 32);
		strncpy_s(name, _name, 32);
	}
	terrainPoolTemplate()
	{
		strncpy_s(cname, "undef", 32);
		strncpy_s(name, "Undefined", 32);
		strncpy_s(landListName, "land", 32);
		strncpy_s(wornListName, "worn", 32);
		strncpy_s(accentListName, "accent", 32);
		strncpy_s(decoListName, "deco", 32);
		strncpy_s(shapesListName, "shapes", 32);
	}
};//size 60 bytes(?)

struct stringList
{
	char cname[32];
	std::vector<std::string> list;
	stringList() {strncpy_s(cname, 32, "undef", 32); list.clear(); list.push_back("nothing");}
};

//compiled template of eco pools in the biome/map generation
struct ecoPoolTemplate
{
	char cname[32];
	char name[32];
	char plantListName[32];
	char creatureListName[32];
	ecoPoolTemplate(const char* _cname, const char* _name)
	{
		strncpy_s(cname, _cname, 32);
		strncpy_s(name, _name, 32);
	}
	ecoPoolTemplate()
	{
		strncpy_s(cname, "undef", 32);
		strncpy_s(name, "Undefined", 32);
		strncpy_s(plantListName, "plant", 32);
		strncpy_s(creatureListName, "creature", 32);
	}};

//compiled template of biome information used in map generation
struct biomeInformationTemplate
{
	char cname[32];
	char name[32];
	char terrainListName[32];
	char ecologyListName[32];
	unsigned char temperature;
	unsigned char elevation;
	unsigned char atmosphere;
	biomeInformationTemplate()
	{
		strncpy_s(cname, "biome", 32);
		strncpy_s(name, "Undefined Biome", 32);
		strncpy_s(terrainListName, "terrain", 32);
		strncpy_s(ecologyListName, "ecology", 32);
		temperature=0; elevation=0; atmosphere=0;
	}
};

//constructed template of update processes
struct actionTemplate
{
	char cname[40];
	unsigned char category;	//divide actions to movement, combat, healing, item use, etc.
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
	coord origin;
	coord dimensions;
	sf::IntRect box;
	char spritefile[40];
	unsigned char iconRange;
	char actionName[32];
	unsigned char actionID;
	guiButtonTemplate(const char* _cname,
						const char* _name,
						coord _dim,
						sf::IntRect _box,
						const char* _sprite,
						const char* _actionName,
						unsigned char _iconRange,
						unsigned char _action)
	{strncpy_s(cname, 40, _cname, 40); strncpy_s(name, 32, _name, 32); origin=coord(0,0); dimensions=_dim; box=_box; actionID=_action; strncpy_s(spritefile, 40, _sprite, 40); strncpy_s(actionName, 40, _actionName, 40); iconRange=_iconRange;}
	guiButtonTemplate()
	{strncpy_s(cname, 40, "undef", 40); strncpy_s(name, 32, "Undefined", 32); actionID=0; origin=coord(0,0); dimensions=coord(0,0); box=sf::IntRect(0,0,0,0); strncpy_s(spritefile, 40, "gui-1_2.png", 40); strncpy_s(actionName, 32, "none", 32); iconRange=0; actionID=0;}
};

//template of forms using multiple gui buttons
struct guiFormsTemplate
{
	char cname[40];
	char name[32];
	coord dimensions;
	std::vector<unsigned int> buttonList; //holds the **game-registered** button index or 0
	guiFormsTemplate()
	{strncpy_s(cname, 40, "undef", 40); strncpy_s(name, 32, "Undefined", 32); buttonList.clear();}
};

class TemplateContainerClass
{
	public:
		//all vectors begin with a default entry at record 0, so no list is empty and all indexes are positive
		TemplateContainerClass() {
			varianceList.clear(); varianceList.push_back(colorVarianceTemplate());
			tileList.clear(); tileList.push_back(tileTemplate());
			actionList.clear(); actionList.push_back(actionTemplate());
			entityList.clear(); entityList.push_back(entityTemplate());
			decoPackList.clear(); decoPackList.push_back(decoPackTemplate());
			toolPackList.clear(); toolPackList.push_back(toolPackTemplate());
			ingredientPackList.clear(); ingredientPackList.push_back(ingredientPackTemplate());
			creaturePackList.clear(); creaturePackList.push_back(creaturePackTemplate());
			vegPackList.clear(); vegPackList.push_back(vegetationPackTemplate());
			seedPackList.clear(); seedPackList.push_back(seedPackTemplate());
			summonPackList.clear(); summonPackList.push_back(summonPackTemplate());
			buttonList.clear(); buttonList.push_back(guiButtonTemplate());
			valuesList.clear(); valuesList.push_back(stringList());
			terrainList.clear(); terrainList.push_back(terrainPoolTemplate());
			ecoList.clear(); ecoList.push_back(ecoPoolTemplate());
			biomeList.clear(); biomeList.push_back(biomeInformationTemplate());
			formsList.clear(); formsList.push_back(guiFormsTemplate());
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
		std::vector<actionTemplate> actionList;
		std::vector<guiButtonTemplate> buttonList;
		std::vector<guiFormsTemplate> formsList;
		std::vector<stringList> valuesList;
};

#endif//TEMPLATES_H