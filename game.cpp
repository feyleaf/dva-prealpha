/*
game.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
June 14, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#include "globals.h"

//helps the constructor by reseting values and loading settings
void GameClass::initialize()
{
	registry.clear();
	header.randSeed+=100;
	isClicking=true;
	pin=false;
	loadSettings();
	gameClock.restart();
	frameClock.restart();
	gameConstant=60;
	gamemode=0;
	initRandom(header.randSeed);
}

//contructor, sets default values to member variables, opens debug log file
//then creates the 'app' window
GameClass::GameClass()
{
	startTime=int(unsigned long(time(NULL))-unsigned long(JAN1_2014));
	debugFile.open("processes.txt");
	newGame=true;
	quitGame=false;
	header.mapIndex=0;
	header.randSeed=(unsigned int)time(NULL);
	initialize();
	tmp.parseFile("allreg-testing.txt");
	render.createTileSheet(tmp);
	render.createEntitySheet(tmp);
	render.createGuiSheet(tmp);
	render.loadGraphicsFiles(settings);
	debugFile << "Read " << tmp.parser.getLineNumber()-1 << " templates from the file.\n";
	for(int i=1; i<int(tmp.container.valuesList.size()); i++)
	{
		debugFile << "List: " << tmp.container.valuesList[i].cname << "\n";
		for(int j=0; j<int(tmp.container.valuesList[i].list.size()); j++)
		{
			debugFile << tmp.container.valuesList[i].list[j] << ", ";
		}
		debugFile << "\n";
	}
	app.create(sf::VideoMode(settings.winWid, settings.winHig, 32), settings.verTitle);
	mainfont.loadFromFile(settings.mainFontFile);
	sidebar = sf::Text("Sidebar Line 1\nLine 2\nLine 3\nLine 4\nLast Line", mainfont, 24);
	sidebar.setPosition(float(settings.tileCols*settings.tileWid)+10.0f, 10.0f);
	for(int t=0; t<int(tmp.container.decoPackList.size()); t++)
	{
		debugFile.write(":", 1);
		debugFile << tmp.container.entityList[tmp.container.decoPackList[t].entityID].name;
		debugFile.write("\n", 1);
	}
	registry.createAction(tmp, "generatemap", 0,0,0,0.0f);
}

//runs an update each frame (~1/8 second)
bool GameClass::gameLoop()
{
	while(!quitGame)
	{
		inputHandler();

		//game world drawing routine

		if(frameClock.getElapsedTime().asSeconds()>0.125f) //force a constant framerate
		{
			gameUpdater(gameTime());
			frameClock.restart();
			gameRenderer();
		}
	}

	return true;
}

//handles the mouse, keyboard, and window-close input
void GameClass::inputHandler()
{
	pollWindowsEvents();
	pollKeys();
	pollMouseClicks();
}

void GameClass::pollWindowsEvents()
{
	while(app.pollEvent(gameEvent))
	{	
		if((gameEvent.type == sf::Event::Closed) || (gameEvent.type == sf::Event::KeyPressed && gameEvent.key.code == sf::Keyboard::Escape))
		{
			quitGame=true;
		}
	}
}

void GameClass::pollKeys()
{
}

void GameClass::pollMouseClicks()
{
	//mouse handling routine
	getMouseGrid();
	if(!theMouse.isButtonPressed(sf::Mouse::Button::Left)) isClicking=false;
	if(theMouse.isButtonPressed(sf::Mouse::Button::Left) && !isClicking)
	{
		isClicking=true;
		if(isClickOnGUI())
		{
			handleGUIClick(mouse); //creates actions based on the button that was clicked
		}
		else if(isClickOnBoard())
		{
			handleBoardClick(mouse); //creates actions based on the point that was clicked
		}
	}
}

bool GameClass::actionCodeEquals(int index, const char* _code)
{
	if(index==0 || index>int(tmp.container.actionList.size()) || _code==NULL || tmp.container.actionList[index].cname==NULL) return false;
	return (strcmp(tmp.container.actionList[index].cname, _code)==0);
}

//performs update tasks each frame
void GameClass::gameUpdater(float actSeconds)
{
	//must keep track of the size prior to running the action queues, because the queue will grow every tick
	//which could result in an infinite loop
	int thisTick=int(registry.obj.actions.size());
	processActionList(thisTick, actSeconds);
}

void GameClass::processActionList(int maxlength, float actSeconds)
{
	for(int i=1; i<maxlength; i++)
	{
		if(registry.obj.actions[i] != NULL && registry.obj.actions[i]->active)
		{
			if(registry.obj.actions[i]->timeToActivate<=actSeconds)
			{
				processAction(registry.obj.actions[i]);
			}
		}
	}

}

//processes an active, valid and queued action
void GameClass::processAction(actionStruct* act)
{
	if(!act->active) return;
	act->active = false;
	int currentIndex=act->actionTemplateIndex;
	bool enSrc=(act->entityIndexSource != 0 && registry.obj.regEntities[act->entityIndexSource] != NULL);
	bool enTrg=(act->entityIndexTarget != 0 && registry.obj.regEntities[act->entityIndexTarget] != NULL);
	bool tlTrg=(act->tileIndexTarget != 0 && registry.obj.regTiles[act->tileIndexTarget] != NULL);
	if(enSrc)
	{
		//split between entity targets and tile targets
		if(enTrg)
		{
			//target is an entity
		}
		else if(tlTrg)
		{
			//target is a tile
		}
		else
		{
			//source entity with no target specified
			//ideal for growflower
			if(actionCodeEquals(currentIndex, "growflower"))
			{
				int src=act->entityIndexSource;
				int veg=registry.obj.regEntities[act->entityIndexSource]->packIndex;
				if(veg==0 || src==0) return;
				float duration=float(registry.obj.regVeg[veg]->growthTicks)*0.2f;
				//update the frame as long as it's still less than the max growth stages
				if(registry.obj.regVeg[veg]->currentGrowth < (registry.obj.regVeg[veg]->maxGrowth-1))
				{
					act->active=false;
					registry.obj.regVeg[veg]->currentGrowth++;
					registry.obj.regEntities[src]->frame = registry.obj.regVeg[veg]->currentGrowth;
					registry.createAction(tmp, "growflower", src, 0, 0, gameTime()+duration);
				}
				return;
			}
			if(actionCodeEquals(currentIndex, "convertflower"))
			{
				bool isRed=false;
				int src=act->entityIndexSource;
				isRed=(registry.obj.regEntities[src]->entityTemplateIndex==registry.obj.getEntityTemplateIndex(tmp, "redrose"));
				coord psx=registry.obj.regEntities[src]->pos;
				int veg=registry.obj.regEntities[src]->packIndex;
				registry.eraseEntity(src);
				//registry.regEntities.erase(registry.regEntities.begin()+src);
				//registry.regVeg.erase(registry.regVeg.begin()+veg);
				if(isRed)
				{
					fillEntity("redpetal", psx);
				}
				else
				{
					fillEntity("bluepetal", psx);
				}
				return;
			}
			if(actionCodeEquals(currentIndex, "selectentity"))
			{
				if(gamemode==GAMEMODE_INSPECT)
				{
					sidebar.setString(outputEntity(act->entityIndexSource));
				}
				return;
			}
		}
	}
	else
	{
			if(actionCodeEquals(currentIndex, "generatemap"))
			{
				gamemode=GAMEMODE_NEUTRAL;
				initialize();
				experimentalMapGen();
				fillButton("magnifier", coord(settings.tileCols, 5));
				fillButton("recycle", coord(settings.tileCols+1, 5));
				fillButton("camera", coord(settings.tileCols+2, 5));
				return;
			}
			if(actionCodeEquals(currentIndex, "screenshot"))
			{
				gamemode=GAMEMODE_NEUTRAL;
				capture();
				return;
			}
			if(actionCodeEquals(currentIndex, "infoget"))
			{
				if(gamemode==GAMEMODE_INSPECT)
				{
					registry.createAction(tmp, "clearsidebar", 0,0,0,gameTime()+0.125f);
				}
				else
				{
					sidebar.setString("Click an Entity\nto Inspect...");
					gamemode=GAMEMODE_INSPECT;
				}
				return;
			}
			if(actionCodeEquals(currentIndex, "clearsidebar"))
			{
				sidebar.setString("");
				gamemode=GAMEMODE_NEUTRAL;
				return;
			}
	}

}

void GameClass::capture()
{
	sf::Image shot(app.capture());
	char buffer[65];
	sprintf_s(buffer, 65, "screenshots/%i.png", int(gameTime()));
	shot.saveToFile(buffer);
	sidebar.setString(sf::String("Saved screen to:\n"+sf::String(buffer))+"\n");
}

void GameClass::experimentalMapGen()
{
	initRandom(header.randSeed);
	registry.createMapTerrain(tmp, "beachterrain", "beachecology");
	int i=registry.obj.regMaps.size()-1;
	if(registry.obj.regMaps[i] == NULL) return;
	else
	{
		fillShape("box", tmp.container.tileList[registry.obj.regMaps[i]->baseTiles].cname,  tmp.container.tileList[registry.obj.regMaps[i]->baseTiles].cname, coord(0,0), coord(settings.tileCols, settings.tileRows));
		for(int j=1; j<int(registry.obj.regMaps[i]->shapeLayer.size()); j++)
		{
			fillShape(tmp.container.valuesList[registry.obj.regMaps[i]->shapeLayer[j]->shapeTemplateIndex].list[registry.obj.regMaps[i]->shapeLayer[j]->shapeNameIndex].c_str(), tmp.container.tileList[registry.obj.regMaps[i]->baseTiles].cname, tmp.container.tileList[registry.obj.regMaps[i]->shapeLayer[j]->terrainTiles].cname,registry.obj.regMaps[i]->shapeLayer[j]->tl, registry.obj.regMaps[i]->shapeLayer[j]->br);
		}
	}
	if(registry.obj.regMaps[i]->decoLayer[1] == NULL) return;
	for(int yy=registry.obj.regMaps[i]->decoLayer[1]->tl.y; yy<registry.obj.regMaps[i]->decoLayer[1]->br.y; yy++)
	{
		for(int xx=registry.obj.regMaps[i]->decoLayer[1]->tl.x; xx<registry.obj.regMaps[i]->decoLayer[1]->br.x; xx++)
		{
			if(rand()%100<registry.obj.regMaps[i]->decoLayer[1]->density)
			{
				fillEntity(tmp.container.entityList[registry.obj.regMaps[i]->decoLayer[1]->entityTemplateIndex].cname, coord(xx,yy));
			}
		}
	}
	if(registry.obj.regMaps[i]->vegLayer[1] == NULL) return;
	for(int yy=registry.obj.regMaps[i]->vegLayer[1]->tl.y; yy<registry.obj.regMaps[i]->vegLayer[1]->br.y; yy++)
	{
		for(int xx=registry.obj.regMaps[i]->vegLayer[1]->tl.x; xx<registry.obj.regMaps[i]->vegLayer[1]->br.x; xx++)
		{
			if(rand()%100<registry.obj.regMaps[i]->vegLayer[1]->density)
			{
				fillEntity(tmp.container.entityList[registry.obj.regMaps[i]->vegLayer[1]->entityTemplateIndex].cname, coord(xx,yy));
			}
		}
	}
	if(registry.obj.regMaps[i]->creatureLayer[1] == NULL) return;
	for(int yy=registry.obj.regMaps[i]->creatureLayer[1]->tl.y; yy<registry.obj.regMaps[i]->creatureLayer[1]->br.y; yy++)
	{
		for(int xx=registry.obj.regMaps[i]->creatureLayer[1]->tl.x; xx<registry.obj.regMaps[i]->creatureLayer[1]->br.x; xx++)
		{
			if(rand()%100<registry.obj.regMaps[i]->creatureLayer[1]->density)
			{
				fillEntity(tmp.container.entityList[registry.obj.regMaps[i]->creatureLayer[1]->entityTemplateIndex].cname, coord(xx,yy));
			}
		}
	}
}


//_tl is top left of region
//_br is bottom right of region
//mainTileID is the id of the tile surrounding the shape and filling the region
//accentTileID is the id of the tile inside of the shape
//TODO: set up a registry for shapes. for now we'll handle the formation inside this function
void GameClass::fillShape(const char* shapename, const char* codename_main, const char* codename_accent, coord _tl, coord _br)
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
			if(codename_main != NULL) fillTile(codename_main, _tl+coord(x,y));
			if(codename_main == NULL || codename_accent == NULL) return;
			adjY=int(float(y)*ratio);
			if(strcmp(shapename, "circle")==0)
			{
				if((pow(x-axisX, 2)+pow(adjY-(rangeY/2), 2))<pow(axisX, 2))
				{
					fillTile(codename_accent, _tl+coord(x,y));
				}
			}
			if(strcmp(shapename, "box")==0)
			{
				fillTile(codename_accent, _tl+coord(x,y));
			}
			if(strcmp(shapename, "conic")==0)
			{
				if(adjY>=abs(x-axisX))
				{
					fillTile(codename_accent, _tl+coord(x,y));
				}
			}
			if(strcmp(shapename, "parabola")==0)
			{
				if(adjY>=pow(((axisX-x)/2),2))
				{
					fillTile(codename_accent, _tl+coord(x,y));
				}
			}
			if(strcmp(shapename, "triangle")==0)
			{
				if(adjY>=x)
				{
					fillTile(codename_accent, _tl+coord(x,y));
				}
			}
		}
	}
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
	if(!registry.createTile(tmp, codename, _pos, 16, rand()%50000))
	{
		//if there's nothing matching to clone, we must skip this step and inform the debug log
		debugFile << "FillTile failed at (" << _pos.x << ", " << _pos.y << "). clone was undefined.\n";
		return;
	}
}

void GameClass::fillEntity(const char* codename, coord _pos)
{
	if(!registry.createEntity(tmp, codename, _pos, gameTime()))
	{
		//if there's nothing matching to clone, we must skip this step and inform the debug log
		debugFile << "FillEntity failed at (" << _pos.x << ", " << _pos.y << "). clone was undefined.\n";
		return;
	}
}

void GameClass::fillButton(const char* codename, coord _pos)
{
	if(!registry.createButton(tmp, codename, _pos))
	{
		//if there's nothing matching to clone, we must skip this step and inform the debug log
		debugFile << "FillGui failed at (" << _pos.x << ", " << _pos.y << "). clone was undefined.\n";
		return;
	}
}

GameClass::~GameClass()
{
	debugFile << "Closed with " << int(registry.obj.actions.size()) << " pending/created actions\n";
	debugFile << "Time in unsigned long format: " << (unsigned long(time(NULL))) << "\n";
	debugFile.close();
	app.close();
}

//returns the size of the regTiles list
int GameClass::numberOfTiles()
{
	return int(registry.obj.regTiles.size());
}

int GameClass::numberOfEntities()
{
	return int(registry.obj.regEntities.size());
}

int GameClass::entityHover()
{
	//only returns oldest registrered index
	for(int i=1; i<numberOfEntities(); i++)
	{
		if(registry.obj.regEntities[i]->active && registry.obj.regEntities[i]->box.contains(finemouse.x, finemouse.y))
		{
			return i;
		}
	}
	return 0;
}

int GameClass::buttonHover()
{
	//only returns oldest registrered index
	for(int i=1; i<int(registry.obj.regButtons.size()); i++)
	{
		if(registry.obj.regButtons[i]->active && registry.obj.regButtons[i]->pos == mouse)
		{
			return i;
		}
	}
	return 0;
}

int GameClass::tileHover()
{
	//if(entityHover()>0) return 0; //maybe??
	for(int i=1; i<numberOfTiles(); i++)
	{
		if(registry.obj.regTiles[i]->pos == mouse)
		{
			return i;
		}
	}
	return 0;
}

//returns the grid coordinates of the mouse pointer
coord GameClass::getMouseGrid()
{
	if(app.getSize().x>0 && app.getSize().y>0)
	{
		//STANDARD LAYOUT (480x320, fully flexible)
		finemouse.x=(((theMouse.getPosition(app).x)*settings.winWid)/(app.getSize().x));
		finemouse.y=(((theMouse.getPosition(app).y)*settings.winHig)/(app.getSize().y));

		//align with grid
		mouse.x=finemouse.x/settings.tileWid;
		mouse.y=(finemouse.y-16)/settings.tileHig;
	}
	return mouse;
}

bool GameClass::isClickOnBoard()
{
	return (!(mouse.x<0 || mouse.y<0 || mouse.x>settings.tileCols || mouse.y>settings.tileRows));
}

bool GameClass::isClickOnGUI()
{
	if(!(finemouse.x<0 || finemouse.y<0 || finemouse.x>settings.winWid || finemouse.y>settings.winHig))
	{
		return (buttonHover()>0);
	}
	else return false;
}

void GameClass::handleBoardClick(coord _mouse)
{
	int entityIndex=entityHover();
	if(entityIndex>0)
	{
		registry.createAction(tmp, "selectentity", entityIndex, 0,0,gameTime()+0.125f);
	}
}

void GameClass::handleGUIClick(coord _mouse)
{
	int index=buttonHover();
	if(index==0 || !registry.obj.regButtons[index]->active) return;
	if(registry.obj.regButtons[index]->actionTemplateIndex>0)
	{
		for(int i=1; i<int(tmp.container.actionList.size()); i++)
		{
			if(registry.obj.regButtons[index]->actionTemplateIndex == i)
			{
				registry.createAction(tmp, tmp.container.actionList[i].cname, 0, 0, 0,gameTime());
				return;
			}
		}
	}
}

//renders the game state to the screen
void GameClass::gameRenderer()
{
	app.clear(sf::Color(101,88,65,255));

	//draw the tiles that are registered
	//TODO: make it map-specific
	for(int i=1; i<int(registry.obj.regTiles.size()); i++)
	{
		if(registry.obj.regTiles[i] != NULL)
		{
			if(i==tileHover() && entityHover()==0)
			{
				render.DrawTile(app, registry.obj.regTiles[i], registry.obj.regTiles[i]->pos, sf::Color::Red);
			}
			else render.DrawTile(app, registry.obj.regTiles[i], registry.obj.regTiles[i]->pos, registry.obj.regTiles[i]->distortionColor);
		}
	}
	for(int i=1; i<int(registry.obj.regEntities.size()); i++)
	{
		if(registry.obj.regEntities[i] != NULL && registry.obj.regEntities[i]->active)
			render.DrawEntity(app, registry.obj.regEntities[i], registry.obj.regEntities[i]->pos, (i==entityHover()));
	}
	for(int i=1; i<int(registry.obj.regButtons.size()); i++)
	{
		if(registry.obj.regButtons[i] != NULL && registry.obj.regButtons[i]->active)
			render.DrawGui(app, registry.obj.regButtons[i], registry.obj.regButtons[i]->pos);
	}
	app.draw(sidebar);
	app.display();
}

sf::String GameClass::outputEntity(int index)
{
	sf::String ret="";
	char buffer[64];
	int calc=0;
	if(registry.obj.regEntities[index] == NULL) return ret;

	ret+=tmp.container.entityList[registry.obj.regEntities[index]->entityTemplateIndex].name;
	ret+="\n";
	switch (registry.obj.regEntities[index]->type)
	{
	case ICAT_CREATURE: ret+="Creature\n"; ret+="HP: ";
		calc=registry.obj.regEntities[index]->packIndex;
		sprintf_s(buffer, "%i / %i", registry.obj.regCreature[calc]->currentHP,registry.obj.regCreature[calc]->maxHP);
		ret+=sf::String(buffer);
		break;
	case ICAT_DECORATION: ret+="Decoration\n"; break;
	case ICAT_VEGETATION: ret+="Vegetation\n"; 
		calc=registry.obj.regEntities[index]->packIndex;
		sprintf_s(buffer, "Growth: %i / %i", registry.obj.regVeg[calc]->currentGrowth+1,registry.obj.regVeg[calc]->maxGrowth);
		ret+=sf::String(buffer);
		break;
	case ICAT_INGREDIENT: ret+="Ingredient\n"; break;
	case ICAT_SEED: ret+="Seed\n"; break;
	case ICAT_SUMMON: ret+="Summon Charm\n"; break;
	case ICAT_TOOL: ret+="Tool\n"; break;
	default: ret+="Something Else...\n"; break;
	}
	return ret;
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