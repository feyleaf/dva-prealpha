#ifndef GAME_H
#define GAME_H

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
	ValueRegistryClass templateRegistry;
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
	bool loadSettings();
	GameClass();
	~GameClass();
	void initialize();
	bool gameLoop();
	void inputHandler();
	void gameUpdater(float actSeconds);
	void gameRenderer();
	coord getMouseGrid();

	tileTemplate* cloneTile(const unsigned int tileID);
	void fillMap(int tileID);
	sf::IntRect tileSize;

};

#endif//GAME_H