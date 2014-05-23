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



public:
	RenderManager render;
	bool loadSettings();
	GameClass();
	~GameClass();
	void initialize();
	bool gameLoop();
	void inputHandler();
	void gameUpdater(float actSeconds);
	void gameRenderer();
	coord getMouseGrid();

	void experimentalMapGen();
	void fillTile(int tileID, coord _pos);
	void fillEntity(int tileID, coord _pos);
	void fillShape(int shapeID, int mainTileID, int accentTileID, coord _tl, coord _br);
	void scatterDeco(int entityID, int con, unsigned char density, coord _tl, coord _br);
	int numberOfTiles();

};

#endif//GAME_H