/*
game.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 21, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#include "globals.h"

//helps the constructor by reseting values and loading settings
void GameClass::initialize()
{
	pin=false;
	loadSettings();
	gameClock.restart();
	frameClock.restart();
	gameConstant=60;
	gamemode=0;
	header.randSeed=(unsigned int)time(NULL);
	initRandom(header.randSeed);
	tmp.parseFile("allreg-testing.txt");
	render.createTileSheet(tmp);
	render.createEntitySheet(tmp);
	render.loadGraphicsFiles(settings);
	debugFile << "Read " << tmp.parser.getLineNumber()-1 << " templates from the file.\n";
}

//contructor, sets default values to member variables, opens debug log file
//then creates the 'app' window
GameClass::GameClass()
{
	debugFile.open("processes.txt");
	newGame=true;
	quitGame=false;
	header.mapIndex=0;
	initialize();
	app.create(sf::VideoMode(settings.winWid, settings.winHig, 32), settings.verTitle);
	for(int t=0; t<int(tmp.container.decoPackList.size()); t++)
	{
		debugFile.write(":", 1);
		debugFile << tmp.container.entityList[tmp.container.decoPackList[t].entityID].name;
		debugFile.write("\n", 1);
	}
}

//performs update tasks each frame
void GameClass::gameUpdater(float actSeconds)
{
	if(gameConstant==60)
	{
		experimentalMapGen();
		gameConstant=623;
	}
	int thisTick=int(registry.actions.size());
	for(int i=1; i<thisTick; i++)
	{
		if(registry.actions[i] != NULL && registry.actions[i]->active)
		{
			if(registry.actions[i]->timeToActivate<=actSeconds)
			{
				//this is gonna be a more complicated routine lol
				//right now the only action we're trying to test is the growth, so i'll assume all actions
				//active are "growflower"
				if(registry.regEntities[registry.actions[i]->entityIndexSource] != NULL)
				{
					int veg=registry.regEntities[registry.actions[i]->entityIndexSource]->packIndex;
					int src=registry.actions[i]->entityIndexSource;
					int index=registry.regEntities[src]->entityTemplateIndex;
					int act=registry.actions[i]->actionTemplateIndex;
					float duration=float(tmp.container.actionList[act].coolDownTicks)*0.2f;
					//update the frame as long as it's still less than the max growth stages
					if(registry.regVeg[veg]->currentGrowth < (registry.regVeg[veg]->maxGrowth-1))
					{
						registry.actions[i]->active=false;
						registry.regVeg[veg]->currentGrowth++;
						registry.regEntities[src]->frame = registry.regVeg[veg]->currentGrowth;
						registry.createAction(tmp, "growflower", src, src, 0, actSeconds+duration);
					}
				}
			}
		}
	}
}

void GameClass::experimentalMapGen()
{
	initRandom(header.randSeed);
	for(int y=0; y<settings.tileRows; y++)
	{
		for(int x=0; x<settings.tileCols; x++)
		{
			if(rand()%4==2)
			{
				if(rand()%2==0)
				{
					fillTile("customgrass", coord(x,y));
				}				
				else
				{
					fillTile("dirt", coord(x,y));
					int r=rand()%4;
					switch(r)
					{
						case 0:fillEntity("hibiscus", coord(x,y)); break;
						case 1:fillEntity("heavenmirror", coord(x,y)); break;
						case 2:fillEntity("redrose", coord(x,y)); break;
						default:fillEntity("bluerose", coord(x,y)); break;
					}
				}
			}
			else
				fillTile("greengrass", coord(x,y));
		}
	}
	fillEntity("squirrel", coord(5,5));
	fillEntity("irongolem", coord(10,10));

	/*
	//we'll experiment with TERRAIN_BEACH
	int terr_id=TERRAIN_BEACH;
	int regCursor=registry.templateRegistry.matchTerrain(terr_id);
	terrainPoolTemplate* t=registry.templateRegistry.allTerrain[regCursor];

	
	//make a base layout from one of the main tile types
	int backgroundTiles=rand()%(unsigned char(t->landTilesList.size()));
	int tileID=t->landTilesList[backgroundTiles];
	fillShape(SHAPE_BOX, tileID, tileID, coord(0,0), coord(settings.tileCols, settings.tileRows));

	srand(header.randSeed);
	//now we test a shape and an accent tile
	int accentTiles=rand()%(unsigned char(t->accentTilesList.size()));
	int shapeIndex=rand()%(unsigned char(t->shapesList.size()));
	int accentID=t->accentTilesList[accentTiles];
	int shapeID=t->shapesList[shapeIndex];
	coord a(rand()%5, rand()%5);
	coord b(rand()%5+15, rand()%5+10);
	fillShape(shapeID, ID_NONE, accentID, a, b);
	scatterDeco(ID_SQUIRREL, 15, 60, coord(0,0), coord(settings.tileCols, settings.tileRows));
	*/
}
//_tl is top left of region
//_br is bottom right of region
//mainTileID is the id of the tile surrounding the shape and filling the region
//accentTileID is the id of the tile inside of the shape
//TODO: set up a registry for shapes. for now we'll handle the formation inside this function
void GameClass::fillShape(int shapeID, const char* codename_main, const char* codename_accent, coord _tl, coord _br)
{
/*	int rangeX=_br.x-_tl.x;
	int rangeY=_br.y-_tl.y;
	int axisX=rangeX/2;
	float ratio=float(rangeX)/float(rangeY);
	int adjY=0;
	for(int y=0; y<rangeY; y++)
	{
		for(int x=0; x<rangeX; x++)
		{
			if(codename_main != NULL) fillTile(codename_main, _tl+coord(x,y));
			adjY=int(float(y)*ratio);
			switch(shapeID)
			{
				case SHAPE_NULL:
					break;
				case SHAPE_CIRCLE:
					if((pow(x-axisX, 2)+pow(adjY-(rangeY/2), 2))<pow(axisX, 2))
					{
						fillTile(codename_accent, _tl+coord(x,y));
					}
					break;

				case SHAPE_BOX: //a rectangle, easy -- fills the entire region with accentTileID tiles
					fillTile(codename_accent, _tl+coord(x,y));
					break;
				case SHAPE_CONIC:
					if(adjY>=abs(x-axisX))
						fillTile(codename_accent, _tl+coord(x,y));
					break;
				case SHAPE_PARABOLIC:
					if(adjY>=pow(((axisX-x)/2),2))
						fillTile(codename_accent, _tl+coord(x,y));
					break;

				case SHAPE_TRIANGLE:
				default:
					if(adjY>=x)
						fillTile(codename_accent, _tl+coord(x,y));
					break;
			}
		}
	}*/
}

void GameClass::scatterDeco(int entityID, int con, unsigned char density, coord _tl, coord _br)
{//this routine is broken anyway!
	initRandom(header.randSeed);
	for(int y=_tl.y; y<=_br.y; y++)
	{
		for(int x=_tl.x; x<=_br.x; x++)
		{
//			if(abs(noiseyPixel(coord(x,y), 0, 255, con, header.randSeed)/density) ==(con/2))
//				fillEntity(entityID, coord(x,y));
		}
	}
}


//places a tiles map of the selected tile index from the template registry
void GameClass::fillTile(const char* codename, coord _pos)
{
	if(!registry.createTile(tmp, codename, _pos))
	{
		//if there's nothing matching to clone, we must skip this step and inform the debug log
		debugFile << "FillTile failed at (" << _pos.x << ", " << _pos.y << "). clone was undefined.\n";
		return;
	}
}

void GameClass::fillEntity(const char* codename, coord _pos)
{
	if(!registry.createEntity(tmp, codename, _pos, gameClock.getElapsedTime().asSeconds()))
	{
		//if there's nothing matching to clone, we must skip this step and inform the debug log
		debugFile << "FillEntity failed at (" << _pos.x << ", " << _pos.y << "). clone was undefined.\n";
		return;
	}
}

GameClass::~GameClass()
{
	debugFile.close();
	app.close();
}

//returns the size of the regTiles list
int GameClass::numberOfTiles()
{
	return 0;
//	return int(registry.regTiles.size()-1);
}

int GameClass::numberOfEntities()
{
	return 0;
//	return int(registry.regEntities.size()-1);
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
	app.clear(sf::Color(101,88,65,255));

	//draw the tiles that are registered
	//TODO: make it map-specific
	for(int i=0; i<int(registry.regTiles.size()); i++)
	{
		if(registry.regTiles[i] != NULL)
		{
			render.DrawTile(app, registry.regTiles[i], registry.regTiles[i]->pos);
		}
	}
	for(int i=0; i<int(registry.regEntities.size()); i++)
	{
		if(registry.regEntities[i] != NULL)
			render.DrawEntity(app, registry.regEntities[i], registry.regEntities[i]->pos);
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