/*
game.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
July 17, 2014
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

#define IS_NOTARGET 0
#define IS_TILETARGET 1
#define IS_ENTITYTARGET 2
#define IS_BOTHTARGET 3

#define JAN1_2014	1387584000
struct gameHeader
{
	unsigned long randSeed;
	unsigned int mapIndex;
};

class GameClass
{
protected:
	int startTime;
	int gameConstant;
	gameHeader header;
	settingStruct settings;
	bool newGame;
	bool quitGame;
	int gamemode;
	std::ofstream debugFile;
	TemplateRegistryClass tmp;
	GameObjectClass registry;
	InventoryClass inv;
	sf::Clock frameClock;
	sf::Clock gameClock;
	sf::Image entitySheet;
	sf::Image tileSheet;
	sf::Image guiSheet;
	sf::Image screenshot;
	sf::RenderWindow app;
	sf::Event gameEvent;
	sf::Keyboard keys;
	sf::Mouse theMouse;
	coord mouse;
	coord finemouse;
	bool isClicking;
	bool pin;
	sf::Font mainfont;
	sf::Text sidebar;

	TerrainClass terrain;
	AStarClass astar;

	RenderManager render;


public:
	GameClass();
	~GameClass();
	void initialize();
	bool gameLoop();
	coord getMouseGrid();
	float gameTime() {return float(startTime)+gameClock.getElapsedTime().asSeconds();}

	void inputHandler();
	void pollWindowsEvents();
	void pollKeys();
	void pollMouseClicks();
	void capture();

	bool isClickOnBoard();
	bool isClickOnGUI();
	void handleGUIClick(coord _mouse);
	void handleBoardClick(coord _mouse);
	void useTool(int entityIndex, int entityTarget, int tileTarget);
	void useCharm(int entityIndex, int entityTarget, int tileTarget);
	void plantSeed(int entityIndex, int entityTarget, int tileTarget);

	void gameUpdater(float actSeconds);
	void processActionList(int maxlength, float actSeconds);
	bool validateAction(const actionStruct* act);
	bool isPerformingAction(int entityIndex, const char* actionName);
	coord getLatestTargetPosition(int entityIndex);

	void processGrowth(int entityIndex);
	void processMagic(int entityIndex);
	void processFlowerConversion(int entityIndex);

	void gameRenderer();

	bool loadSettings();
	void experimentalMapGen(const char* biome);
	void createBaseMapLayer(const mapGenStruct* map);
	void createDecorationLayer(const mapGenStruct* map);
	void createEcologyLayer(const mapGenStruct* map);

	void fillPathingRoutes();
	void fillTile(const char* codename, coord _pos);
	void fillRoad(const char* codename, coord start, coord end);
	void fillButton(const char* codename, coord _pos, int linkedEntity=0, bool act=true);
	void fillEntity(const char* codename, coord _pos);
	void fillShape(const char* shapename, const char* codename, coord _tl, coord _br);
	bool processConic(coord _pt, float a, float b, float c, float d, float e, float f);
	void scatterEntity(const mapSpreadStruct* spread);

	void processAction(actionStruct* act);
	bool hasSource(const actionStruct* act);
	bool noTarget(const actionStruct* act);
	bool entityTarget(const actionStruct* act);
	bool tileTarget(const actionStruct* act);
	bool fullTargets(const actionStruct* act);

	void handleMovementPipeline(const actionStruct* act);
	void handleCombatPipeline(const actionStruct* act);
	void handleCreationPipeline(const actionStruct* act);
	void handleButtonPipeline(const actionStruct* act);
	void handleItemsPipeline(const actionStruct* act);
	void handleAIPipeline(const actionStruct* act);
	void handleGUIPipeline(const actionStruct* act);

	void fillSourceAction(const char* actionname, int entityIndex, float delay=0.0f);
	void fillEntityTargetAction(const char* actionname, int sourceIndex, int entityTarget, float delay=0.0f);
	void fillTileTargetAction(const char* actionname, int sourceIndex, int tileTarget, float delay=0.0f);
	bool actionCodeEquals(int index, const char* _code);
	int entityHover();
	int buttonHover();
	int tileHover();

	int getTileIndexAt(coord _pt);
	bool eraseTileAt(coord _pt);

	int numberOfTiles();
	int numberOfEntities();

	sf::String outputEntity(int index);
	sf::String outputTile(int index);

};

#endif//GAME_H