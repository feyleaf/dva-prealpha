#include "globals.h"
GameClass::GameClass()
{
	newGame=true;
	quitGame=false;
	header.mapIndex=0;
	gamemode=0;
	frameClock.restart();
	initialize();
	header.randSeed=(unsigned int)time(NULL);

	app.create(sf::VideoMode(settings.winWid, settings.winHig, 32), settings.verTitle);
}

//returns the index from the template registry matching the search ID of the tile
tileTemplate* GameClass::cloneTile(const unsigned int tileID)
{
	for(int i=0; i<int(templateRegistry.allTiles.size()); i++)
	{
		if(templateRegistry.allTiles[i] != NULL)
		{
			if(templateRegistry.allTiles[i]->id == tileID) //denotes a match from the queried tile
			{
				//we clone the selected tile with index i
				return new tileTemplate(*templateRegistry.allTiles[i]);
			}
		}
	}
	return NULL;
}

//places a tiles map of the selected tile index from the template registry
void GameClass::fillMap(int tileID)
{
	tileTemplate* clone = cloneTile(tileID);
	if(clone==NULL)
	{
		//if there's nothing matching to clone, we must skip this step and inform the debug log
		debugFile << "FillMap failed. clone was undefined.\n";
		return;
	}
	sf::Sprite spr;
	sf::Texture tex;
	int spriteIndex=clone->sheetOrigin;
	int sx=spriteIndex%8;
	int sy=int(spriteIndex/8);
	sf::IntRect area = tileSize;
	area.left = sx*settings.tileHig;
	area.top = sy*settings.tileWid;
	tex.loadFromFile(settings.tileSheetFile, area);
	spr.setTexture(tex);
	spr.setScale(2,2);
	for(int y=0; y<settings.tileRows; y++)
	{
		for(int x=0; x<settings.tileCols; x++)
		{
			spr.setPosition(x*settings.tileWid*2, y*settings.tileHig*2);
			app.draw(spr);
		}
	}

}

GameClass::~GameClass()
{
	debugFile.close();
	app.close();
}

coord GameClass::getMouseGrid()
{
	if(app.getSize().x>0 && app.getSize().y>0)
	{
		//LETTERBOXING WINDOW LAYOUT (broken)
//			mouse.x=(((app.GetInput().GetMouseX()-(lboxX))*WINDOW_WIDTH)/(app.GetWidth()-(2*lboxX)));
//			mouse.y=(((app.GetInput().GetMouseY()-(lboxY))*WINDOW_HEIGHT)/(app.GetHeight()-(2*lboxY)));

		//STANDARD LAYOUT (480x320, fully flexible)
		mouse.x=(((sf::Mouse::getPosition(app).x)*settings.winWid)/(app.getSize().x));
		mouse.y=(((sf::Mouse::getPosition(app).y)*settings.winHig)/(app.getSize().y));

		//align with grid
		mouse.x=mouse.x/settings.tileWid;
		mouse.y=mouse.y/settings.tileHig;
	}
	return mouse;
}

bool GameClass::gameLoop()
{
	while(!quitGame)
	{
		inputHandler();

		//game world drawing routine

		if(frameClock.getElapsedTime().asSeconds()>0.125f)
		{
			gameUpdater(frameClock.getElapsedTime().asSeconds());
			frameClock.restart();
			gameRenderer();
		}
	}

	return true;
}

void GameClass::inputHandler()
{
	while(app.pollEvent(gameEvent))
	{	
		if((gameEvent.type == sf::Event::Closed) || (gameEvent.type == sf::Event::KeyPressed && gameEvent.key.code == sf::Keyboard::Escape))
		{
			//saveGame();
			quitGame=true;
			//app.close();
		}
	}

	//mouse handling routine
	getMouseGrid();
	//using this toggle to keep from processing mouse button holding as multiple clicks.
	//mouse button MUST be released in order to process another click.
}

void GameClass::gameUpdater(float actSeconds)
{
}

void GameClass::gameRenderer()
{
	app.clear(sf::Color::White);
	fillMap(ID_SNOW);
	app.display();
}


void GameClass::initialize()
{
	debugFile.open("processes.txt");
	loadSettings();
	srand(header.randSeed);
	tileSize = sf::IntRect(0, 0, settings.tileWid, settings.tileHig);
}

bool GameClass::loadSettings()
{
	std::string buf;
	std::ifstream fin("settings.cfg", std::ios::in|std::ios::beg);
	fin >> settings.tileWid;
	fin >> settings.tileHig;
	fin >> settings.tileRows;
	fin >> settings.tileCols;
	fin >> settings.winWid;
	fin >> settings.winHig;
	fin >> settings.hotbarSize;
	fin >> settings.invWid;
	fin >> settings.invHig;
	fin.get();
	std::getline(fin, buf);
	strncpy_s(settings.verTitle,buf.c_str(), 40);
	std::getline(fin, buf);
	strncpy_s(settings.verTx,buf.c_str(), 40);
	std::getline(fin, buf);
	strncpy_s(settings.saveFile,buf.c_str(), 40);
	std::getline(fin, buf);
	strncpy_s(settings.mainFontFile,buf.c_str(), 40);
	std::getline(fin, buf);
	strncpy_s(settings.auxFontFile,buf.c_str(), 40);
	std::getline(fin, buf);
	strncpy_s(settings.tileSheetFile,buf.c_str(), 40);
	std::getline(fin, buf);
	strncpy_s(settings.entitySheetFile,buf.c_str(), 40);
	std::getline(fin, buf);
	strncpy_s(settings.guiSheetFile,buf.c_str(), 40);
	fin.close();
	debugFile << settings.tileWid << " x " << settings.tileHig << "\n";
	debugFile << "Title: " << settings.verTitle << "\n";
	return true;
}