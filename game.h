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
	gameHeader header;
	settingStruct settings;
	bool newGame;
	bool quitGame;
	int gamemode;
	std::ofstream debugFile;
	GameObjectClass registry;
	sf::Clock frameClock;
	sf::Image entitySheet;
	sf::Image tileSheet;
	sf::Image guiSheet;
	sf::Image screenshot;
	sf::RenderWindow app;
	sf::Event gameEvent;
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

	void fillTile(int tileID, coord _pos);
	sf::IntRect tileSize;
	int numberOfTiles();

};

#endif//GAME_H