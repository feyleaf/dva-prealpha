/*
game.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 22, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#ifndef GAME_H
#define GAME_H

struct gameHeader
{
	unsigned long randSeed;
	unsigned int mapIndex;
};

class GameClass
{
protected:
	int gameConstant;
	gameHeader header;
	settingStruct settings;
	bool newGame;
	bool quitGame;
	int gamemode;
	std::ofstream debugFile;
	TemplateRegistryClass tmp;
	GameObjectClass registry;
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

	RenderManager render;


public:
	GameClass();
	~GameClass();
	void initialize();
	bool gameLoop();
	coord getMouseGrid();

	void inputHandler();
	void gameUpdater(float actSeconds);
	void gameRenderer();

	bool loadSettings();
	void experimentalMapGen();

	void fillTile(const char* codename, coord _pos);
	void fillButton(const char* codename, coord _pos);
	void fillEntity(const char* codename, coord _pos);
	void fillShape(int shapeID, const char* codename_main, const char* codename_accent, coord _tl, coord _br);
	void scatterDeco(int entityID, int con, unsigned char density, coord _tl, coord _br);

	void processAction(actionStruct* act);
	int entityHover();
	int buttonHover();
	int tileHover();

	int getTileIndexAt(coord _pt);
	bool eraseTileAt(coord _pt);

	int numberOfTiles();
	int numberOfEntities();

	sf::String outputEntity(int index);

};

#endif//GAME_H