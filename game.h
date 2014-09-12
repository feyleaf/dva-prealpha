/*
game.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
August 23, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#ifndef GAME_H
#define GAME_H

#define GAMEMODE_NEUTRAL 0
#define GAMEMODE_INSPECT 1
#define GAMEMODE_INVENTORY 2
#define GAMEMODE_ENTITYACTION 3
#define GAMEMODE_ENTITYTARGETING 4
#define GAMEMODE_MAGICSPELL 5
#define GAMEMODE_ZOOMOUT 6
#define GAMEMODE_MINIMAP 7
#define GAMEMODE_CRAFTING 8

#define ACAT_NEUTRAL 0
#define ACAT_MOVEMENT 1
#define ACAT_COMBAT 2
#define ACAT_ITEMS 3
#define ACAT_PEACEFULAI 4
#define ACAT_HOSTILEAI 5
#define ACAT_PLANTS 6
#define ACAT_GUI 7

#define CLICKLAYER_ZERO 0
#define CLICKLAYER_TILE 1
#define CLICKLAYER_DECORATIONS 2
#define CLICKLAYER_CREATURES 3
#define CLICKLAYER_GUIFORM 4
#define CLICKLAYER_BUTTON 5
#define CLICKLAYER_TOP 6

#define FRAMESPEED 0.0333f //amount of ticks per second

#define JAN1_2014	1387584000
struct gameHeader
{
	unsigned long randSeed;
	unsigned int mapIndex;
};

class GameClass
{
protected:
	PlayerEngine player;
	//world logic stuff
	//--prerequisite data for game at runtime
	bool newGame;
	int startTime;
	sf::Clock gameClock;
	int gameConstant;
	gameHeader header;
	TemplateRegistryClass tmp;

	//--state filters, for one specific map
	coord generatorCursor;
	coord viewerCursor;

	//--temporary value structures for one specific map
	TerrainClass terrain;
	AStarClass astar;
	bool dumpActionList;

	//--data output manager
	std::ofstream debugFile;

	//--data in flux, for entire world
	GameObjectClass registry;

////////////////////////

	//rendering stuff
	//--class for managing render engine
	RenderManager render;

	//--object for creating the window, vital to game :)
	sf::RenderWindow app;
	sf::Event gameEvent;

	//--prerequisite data for rendering at runtime
	settingStruct settings;
	sf::Clock frameClock;
	//sf::Font mainfont;

	//--data in flux, for rendering communications with the player
	sf::Text sidebar;
	sf::Image tempImg;

public:
	//universal game-handling stuff
	GameClass();
	~GameClass();
	void initialize();
	bool gameLoop();
	float gameTime() {return float(startTime)+gameClock.getElapsedTime().asSeconds();}
	coord gridToPixel(coord _grid) {return coord(_grid.x*settings.tileDimensions.x, _grid.y*settings.tileDimensions.y);}

	//world logic stuff
	void inputHandler();
	void gameUpdater(float actSeconds);

	//routines for simplifying actions
	void useTool(int entityIndex, int entityTarget, int tileTarget);
	void useCharm(int entityIndex, int entityTarget, int tileTarget);
	void plantSeed(int entityIndex, int entityTarget, int tileTarget);
	void processGrowth(int entityIndex);
	void processMagic(int entityIndex);
	void processFlowerConversion(int entityIndex);

	//routines for filtering and handling actions and action queues
	bool validateAction(const actionStruct* act);
	bool validateTilePosition(coord pos);
	bool validateEntity(int entityIndex);
	bool isCreature(int entityIndex);

	//interface with the game via windows buttons, keys and clicks
	void pollKeys();
	void pollMouseClicks();
	void pollWindowsEvents();
	int translateClickLayer(coord _pixel);
	void handleClick(int clickLayer, coord _pixel);
	void handleGUIClick(GUIFormClass& form);
	void handleBoardClick();
	void handleMinimapClick();

	//dealing with a single map's generation
	void refreshMap(coord map_pos);
	bool mapExists(coord map_pos);
	void experimentalMapGen(coord map_pos, const char* biome);
	void createBaseMapLayer(const mapGenStruct* map);
	void createDecorationLayer(const mapGenStruct* map);
	void createEcologyLayer(const mapGenStruct* map);

	//generating a map one registry element at a time
	void fillGuiForm(GUIFormClass& form, int linked = 0, bool active = true);
	void fillInventory();
	void eraseGuiForm(GUIFormClass& form);
	void fillTile(const char* codename, coord _pos);
	void fillRoad(const char* codename, coord start, coord end);
	int fillButton(const char* codename, coord _pixel_pos, int linkedEntity=0, bool act=true);
	void fillEntity(const char* codename, coord _pos);
	void fillShape(const char* shapename, const char* codename, coord _tl, coord _br);
	bool processConic(coord _pt, float a, float b, float c, float d, float e, float f);
	void scatterEntity(const mapSpreadStruct* spread);

	//handling movement of creatures or other enitity elements
	//requires interacting with the map and action queue
	void fillPathingRoutes();
	coord smartPathing(int entityIndex, coord pos);
	void forcePushCreature(int entityIndex, coord direction);
	bool moveToAlign(int entityIndex);
	bool commitMovement(int entityIndex, coord tTarget, const actionStruct* act);
	bool isOffsetCongruent(int entityIndex);
	void forceOffsetCorrection(int entityIndex, coord direction);
	void gridAlignEntity(int entityIndex, coord pos);
	coord getLatestTargetPosition(int entityIndex);
	int getEnemyNeighbor(int entityIndex);
	bool isEnemyNeighbor(int entityIndex);
	bool isReachedEntityTarget(int entityIndex, int entityTarget);

	//handlers for the action queue on a single map
	void processActionList(int maxlength, float actSeconds);
	void stopAction(int entityIndex, const char* actionName);
	void stopActionCategory(int entityIndex, int category);
	int getActionIndex(int entityIndex, const char* actionName);
	bool isPerformingAction(int entityIndex, const char* actionName);

	//handlers for processing and filtering actions within an action queue
	void processAction(actionStruct* act);
	bool hasSource(const actionStruct* act);
	bool noTarget(const actionStruct* act);
	bool entityTarget(const actionStruct* act);
	bool tileTarget(const actionStruct* act);
	bool fullTargets(const actionStruct* act);
	coord minimapHover(coord map_pos);

	//handling groups of processes one action at a time
	void handleMovementPipeline(const actionStruct* act);
	void handleCombatPipeline(const actionStruct* act);
	void handleCreationPipeline(const actionStruct* act);
	void handleAIPipeline(const actionStruct* act);
	void handleGUIPipeline(const actionStruct* act);
	void handleButtonPipeline(const actionStruct* act);
	void handleItemsPipeline(const actionStruct* act);

	//creation of new actions on a single map's action queue
	void fillSourceAction(const char* actionname, int entityIndex, float delay=0.0f);
	void fillEntityTargetAction(const char* actionname, int sourceIndex, int entityTarget, float delay=0.0f);
	void fillTileTargetAction(const char* actionname, int sourceIndex, int tileTarget, float delay=0.0f);
	bool actionCodeEquals(int index, const char* _code);

	//misc data pertaining to the current map's elements
	int getTileIndexAt(coord _pt);
	bool eraseTileAt(coord _pt);
	int numberOfTiles();
	int numberOfEntities();

	//rendering stuff
	void gameRenderer();

	//prerequisites for rendering on screen
	bool loadSettings();
	float mapscale;

	//data for display when communicating with player on screen
	sf::String outputEntity(int index);
	sf::String outputTile(int index);

	//screenshot function
	void capture();
};

#endif//GAME_H