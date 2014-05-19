/*
game.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 19, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#include "globals.h"

//helps the constructor by reseting values and loading settings
void GameClass::initialize()
{
	loadSettings();
	render.loadGraphicsFiles(settings);
	gameClock.restart();
	frameClock.restart();
	gameConstant=60;
}

//contructor, sets default values to member variables, opens debug log file
//then creates the 'app' window
GameClass::GameClass()
{
	debugFile.open("processes.txt");
	newGame=true;
	quitGame=false;
	header.mapIndex=0;
	gamemode=0;
	header.randSeed=(unsigned int)time(NULL);
	srand(header.randSeed);
	initialize();

	app.create(sf::VideoMode(settings.winWid, settings.winHig, 32), settings.verTitle);
}

//performs update tasks each frame
void GameClass::gameUpdater(float actSeconds)
{
	fillShape(SHAPE_BOX, ID_WATER, ID_WATER, coord(0,0), coord(settings.tileCols,settings.tileRows));
	fillShape(SHAPE_CIRCLE, ID_NONE, ID_SAND, coord(10,5), coord(20,15));
	fillShape(SHAPE_CIRCLE, ID_NONE, ID_SAND, coord(5,4), coord(16,15));
	fillShape(SHAPE_CIRCLE, ID_NONE, ID_GREENGRASS, coord(8,7), coord(17,12));
	fillShape(SHAPE_PARABOLIC, ID_NONE, ID_CLAY, coord(23, 0), coord(35, 18));
	fillShape(SHAPE_BOX, ID_NONE, ID_PLANKS, coord(19, 11), coord(25, 12));

}

//_tl is top left of region
//_br is bottom right of region
//mainTileID is the id of the tile surrounding the shape and filling the region
//accentTileID is the id of the tile inside of the shape
//TODO: set up a registry for shapes. for now we'll handle the formation inside this function
void GameClass::fillShape(int shapeID, int mainTileID, int accentTileID, coord _tl, coord _br)
{
	int rangeX=_br.x-_tl.x;
	int rangeY=_br.y-_tl.y;
	int axisX=rangeX/2;
	float ratio=float(rangeX)/float(rangeY);
	int adjY=0;
	for(int y=0; y<rangeY; y++)
	{
		for(int x=0; x<rangeX; x++)
		{
			if(mainTileID != ID_NONE) fillTile(mainTileID, _tl+coord(x,y));
			adjY=int(float(y)*ratio);
			switch(shapeID)
			{
				case SHAPE_NULL:
					break;
				case SHAPE_CIRCLE:
					if((pow(x-axisX, 2)+pow(adjY-(rangeY/2), 2))<pow(axisX, 2))
					{
						fillTile(accentTileID, _tl+coord(x,y));
					}
					break;

				case SHAPE_BOX: //a rectangle, easy -- fills the entire region with accentTileID tiles
					fillTile(accentTileID, _tl+coord(x,y));
					break;
				case SHAPE_CONIC:
					if(adjY>=abs(x-axisX))
						fillTile(accentTileID, _tl+coord(x,y));
					break;
				case SHAPE_PARABOLIC:
					if(adjY>=pow(((axisX-x)/2),2))
						fillTile(accentTileID, _tl+coord(x,y));
					break;

				case SHAPE_TRIANGLE:
				default:
					if(adjY>=x)
						fillTile(accentTileID, _tl+coord(x,y));
					break;
			}
		}
	}
}

//places a tiles map of the selected tile index from the template registry
void GameClass::fillTile(int tileID, coord _pos)
{
	int index = registry.cloneTile(tileID, _pos, gameConstant);
	if(index==0 || registry.regTiles[index]==NULL)
	{
		//if there's nothing matching to clone, we must skip this step and inform the debug log
		debugFile << "FillTile failed at (" << _pos.x << ", " << _pos.y << "). clone was undefined.\n";
		return;
	}
}

GameClass::~GameClass()
{
	for(int i=int(registry.regTiles.size())-1; i>=0; i--)
		delete registry.regTiles[i];
	debugFile.close();
	app.close();
}

//returns the size of the regTiles list
int GameClass::numberOfTiles()
{
	return int(registry.regTiles.size()-1);
}

//returns the grid coordinates of the mouse pointer
coord GameClass::getMouseGrid()
{
	if(app.getSize().x>0 && app.getSize().y>0)
	{
		//STANDARD LAYOUT (480x320, fully flexible)
		mouse.x=(((sf::Mouse::getPosition(app).x)*settings.winWid)/(app.getSize().x));
		mouse.y=(((sf::Mouse::getPosition(app).y)*settings.winHig)/(app.getSize().y));

		//align with grid
		mouse.x=mouse.x/settings.tileWid;
		mouse.y=mouse.y/settings.tileHig;
	}
	return mouse;
}

//runs an update each frame (1/8 second)
bool GameClass::gameLoop()
{
	while(!quitGame)
	{
		inputHandler();

		//game world drawing routine

		if(frameClock.getElapsedTime().asSeconds()>0.125f) //force a constant framerate
		{
			gameUpdater(gameClock.getElapsedTime().asSeconds());
			frameClock.restart();
			gameRenderer();
		}
	}

	return true;
}

//handles the mouse, keyboard, and window-close input
void GameClass::inputHandler()
{
	while(app.pollEvent(gameEvent))
	{	
		if((gameEvent.type == sf::Event::Closed) || (gameEvent.type == sf::Event::KeyPressed && gameEvent.key.code == sf::Keyboard::Escape))
		{
			quitGame=true;
		}
	}

	//mouse handling routine
	getMouseGrid();
}

//renders the game state to the screen
void GameClass::gameRenderer()
{
	app.clear();

	//draw the tiles that are registered
	//TODO: make it map-specific
	for(int i=0; i<int(registry.regTiles.size()); i++)
	{
		if(registry.regTiles[i] != NULL)
			render.DrawTile(app, registry.regTiles[i], registry.regTiles[i]->grid);
	}
	app.display();
}


//loads the game setup information from 'settings.cfg' text file
//so far inefficient and sloppy code
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