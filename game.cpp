#include "globals.h"
GameClass::GameClass()
{
	newGame=true;
	quitGame=false;
	header.mapIndex=0;
	gamemode=0;
	gameClock.restart();
	frameClock.restart();
	initialize();
	header.randSeed=(unsigned int)time(NULL);
	gameConstant=0;

	app.create(sf::VideoMode(settings.winWid, settings.winHig, 32), settings.verTitle);
}

void GameClass::fillShape(int shapeID, int mainTileID, int accentTileID, coord _tl, coord _br)
{
	//_tl is top left of region
	//_br is bottom right of region
	//mainTileID is the id of the tile surrounding the shape and filling the region
	//accentTileID is the id of the tile inside of the shape
	//TODO: set up a registry for shapes. for now we'll handle the formation inside this function
	int rangeX=_br.x-_tl.x;
	int rangeY=_br.y-_tl.y;
	int axisX=rangeX/2;
	float ratio=float(rangeX)/float(rangeY);
	int adjY=0;
	for(int y=0; y<rangeY; y++)
	{
		for(int x=0; x<rangeX; x++)
		{
			adjY=int(float(y)*ratio);
			switch(shapeID)
			{
				case SHAPE_BOX: //a rectangle, easy -- fills the entire region with accentTileID tiles
					fillTile(accentTileID, _tl+coord(x,y));
					break;
				case SHAPE_CONIC:
					if(adjY>=abs(x-axisX))
						fillTile(accentTileID, _tl+coord(x,y));
					else
						fillTile(mainTileID, _tl+coord(x,y));
					break;
				case SHAPE_PARABOLIC:
					if(adjY>=pow(((axisX-x)/2),2))
						fillTile(accentTileID, _tl+coord(x,y));
					else
						fillTile(mainTileID, _tl+coord(x,y));
					break;

				case SHAPE_TRIANGLE:
				default:
					if(adjY>=x)
						fillTile(accentTileID, _tl+coord(x,y));
					else
						fillTile(mainTileID, _tl+coord(x,y));
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
		debugFile << "FillMap failed. clone was undefined.\n";
		return;
	}
}

GameClass::~GameClass()
{
	debugFile.close();
	app.close();
}

int GameClass::numberOfTiles()
{
	return int(registry.regTiles.size()-1);
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
			gameUpdater(gameClock.getElapsedTime().asSeconds());
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

	if(keys.isKeyPressed(sf::Keyboard::Down))
	{
		registry.regTiles.clear();
		gameConstant=(gameConstant+1)%40;
	}

	//mouse handling routine
	getMouseGrid();
	//using this toggle to keep from processing mouse button holding as multiple clicks.
	//mouse button MUST be released in order to process another click.
}

void GameClass::gameUpdater(float actSeconds)
{
	for(int y=0; y<settings.tileRows; y++)
	{
		for(int x=0; x<settings.tileCols; x++)
		{
			fillTile(ID_EMERALDGRASS, coord(x,y));
		}
	}
	fillShape(SHAPE_PARABOLIC, ID_GREENGRASS, ID_SNOW, coord(0,0), coord(settings.tileCols,settings.tileRows));
}

void GameClass::gameRenderer()
{
	app.clear();

	//draw the tiles that are registered TODO: make it map-specific
	for(int i=0; i<int(registry.regTiles.size()); i++)
	{
		if(registry.regTiles[i] != NULL)
			render.DrawTile(app, registry.regTiles[i], registry.regTiles[i]->grid);
	}
	app.display();
}


void GameClass::initialize()
{
	debugFile.open("processes.txt");
	loadSettings();
	srand(header.randSeed);
	tileSize = sf::IntRect(0, 0, settings.tileWid, settings.tileHig);
	render.loadGraphicsFiles(settings);
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