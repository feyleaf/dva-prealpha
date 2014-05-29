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
	unsigned int randSeed;
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
	coord mouse;

	RenderManager render;


public:
	ParserClass parse;
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
	void fillEntity(int tileID, coord _pos);
	void fillShape(int shapeID, const char* codename_main, const char* codename_accent, coord _tl, coord _br);
	void scatterDeco(int entityID, int con, unsigned char density, coord _tl, coord _br);

	int getTileIndexAt(coord _pt);
	tileObjectStruct* getTileAt(coord _pt);
	bool eraseTileAt(coord _pt);
	void wipeMap(); //experimental!!

	int numberOfTiles();
	int numberOfEntities();

};

#endif//GAME_H