/*
game.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
September 12, 2014
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
	std::vector<BiomeClass> biomes;
	EtherRegistryClass ether;
	GameObjectClass registry;
	ActionManager actions;

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

	int getEntityTemplateIndex(const char* _codename);
	int getActionTemplateIndex(const char* _codename);
	int getTileTemplateIndex(const char* _codename);
	int getGuiTemplateIndex(const char* _codename);
	int getButtonForAction(const char* _codename);
	int getButtonLinkedEntity(const char* _codename);
	int getListTemplateIndex(const char* _codename);
	int getTerrainTemplateIndex(const char* _codename);
	int getEcologyTemplateIndex(const char* _codename);
	int getBiomeTemplateIndex(const char* _codename);

	bool matchAction(const actionStruct* _act, const char* _name);
	//world logic stuff
	void inputHandler();
	void gameUpdater(float actSeconds);

	void processActionList(int maxlength, float actSeconds);
	void processTextList(float actSeconds);

	void processAction(actionStruct* act);
	//handling groups of processes one action at a time
	void handleMovementPipeline(const actionStruct* act);
	void handleCombatPipeline(const actionStruct* act);
	void handleCreationPipeline(const actionStruct* act);
	void handleAIPipeline(const actionStruct* act);
	void handleGUIPipeline(const actionStruct* act);
	void handleButtonPipeline(const actionStruct* act);
	void handleItemsPipeline(const actionStruct* act);


	bool validateTilePosition(coord pos);
	bool validateEntity(int entityIndex);
	bool isCreature(int entityIndex);
	bool isEntityOnMap(int entityIndex);

	//interface with the game via windows buttons, keys and clicks
	void pollKeys();
	void pollMouseClicks();
	void pollWindowsEvents();
	int translateClickLayer(coord _pixel);
	void handleClick(int clickLayer, coord _pixel);
	int getButtonIndexAtPoint(coord map_pos, coord _pixel);
	void handleGUIClick(GUIFormClass& form);
	void handleMinimapClick();

	//dealing with the world biome generation
	void experimentalWorldGen(coord map_pos);
	void biomeGeneration(coord map_pos);

	//dealing with a single map's generation
	void refreshMap(coord map_pos);
	bool mapExists(coord map_pos);
	bool mapComplete(coord map_pos);
	bool biomeExists(coord map_pos);
	BiomeClass getBiomeData(coord map_pos);
	void generateMapName(coord map_pos);
	char* getMapName(coord map_pos);
	void experimentalMapGen(coord map_pos, const char* biome);
	void createBaseMapLayer(const mapGenStruct* map);
	bool createDecorationLayer(const mapGenStruct* map);
	void createEcologyLayer(const mapGenStruct* map);

	//generating a map one registry element at a time
	void fillGuiForm(GUIFormClass& form, int linked = 0, bool active = true);
	void fillInventory();
	void eraseGuiForm(GUIFormClass& form);
	bool addTile(const char* _name, coord map_pos, coord grid_pos);
	int addEntity(const char* _name, coord map_pos, coord grid_pos);
	int addButton(const char* _name, coord map_pos, coord _pixel, int linked = 0);
	int addText(const char* _string, coord map_pos, coord _pixel, sf::Color _color);
	void activateButton(int reg);
	void activateEntityButton(int entityReg);
	void deactivateEntityButton(int entityReg);
	void fillRoad(const char* codename, coord start, coord end);
	void fillShape(const char* shapename, const char* codename, coord _tl, coord _br);
	bool processConic(coord _pt, float a, float b, float c, float d, float e, float f);
	void scatterEntity(const mapSpreadStruct* spread);

	//handling movement of creatures or other enitity elements
	//requires interacting with the map and action queue
	void fillPathingRoutes();
	void commitSmartPathing(int entityIndex, coord pos, const actionStruct* act);
	coord smartPathing(int entityIndex, coord pos);
	void forcePushCreature(int entityIndex, coord direction);
	bool moveToAlign(int entityIndex);
	void moveInDirection(const actionStruct* act, coord dir, const char* protocol);
	bool commitMovement(int entityIndex, coord tTarget, const actionStruct* act);
	bool isOffsetCongruent(int entityIndex);
	void forceOffsetCorrection(int entityIndex, coord direction);
	void gridAlignEntity(int entityIndex, coord pos);
	coord getLatestTargetPosition(int entityIndex);
	int getEnemyNeighbor(int entityIndex);
	bool isEnemyNeighbor(int entityIndex);
	int getFriendlyNeighbor(int entityIndex);
	bool isFriendlyNeighbor(int entityIndex);
	int getClosestFriendlyRange(int entityIndex, float agroRange);
	bool isFriendlyRange(int entityIndex, float agroRange);
	bool isReachedEntityTarget(int entityIndex, int entityTarget);
	bool isEntityOccupyingGrid(int tileIndex);
	void establishTargetTile(int tileIndex);
	void establishTargetEntity(int entityIndex);
	int getMapMode(coord map_pos) {return registry.objMap[map_pos].mapMode;}
	void setMapMode(coord map_pos, int _mode) {registry.objMap[map_pos].mapMode=_mode;}

	coord minimapHover(coord map_pos);

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