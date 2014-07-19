/*
game.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
July 17, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#include "globals.h"

//helps the constructor by reseting values and loading settings
void GameClass::initialize()
{
	registry.clear();
	inv.clearAll();
	header.randSeed+=100;
	isClicking=true;
	pin=false;
	loadSettings();
	astar.initBounds(settings);
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
	app.create(sf::VideoMode(settings.winWid, settings.winHig, 32), settings.verTitle);
	app.setFramerateLimit(60);
	mainfont.loadFromFile(settings.mainFontFile);
	sidebar = sf::Text("Sidebar Line 1\nLine 2\nLine 3\nLine 4\nLast Line", mainfont, 24);
	sidebar.setPosition(float(settings.tileCols*settings.tileWid)+10.0f, 10.0f);
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
				registry.obj.actions[i]->active=false;
			}
		}
	}

}

void GameClass::processGrowth(int entityIndex)
{
	int veg=registry.obj.regEntities[entityIndex]->packIndex;
	if(veg==0 || entityIndex==0) return;
	float duration=float(registry.obj.regVeg[veg]->growthTicks)*0.2f;
	if(registry.obj.regVeg[veg]->currentGrowth < (registry.obj.regVeg[veg]->maxGrowth-1))
	{
		registry.obj.regVeg[veg]->currentGrowth++;
		registry.obj.regEntities[entityIndex]->frame = registry.obj.regVeg[veg]->currentGrowth;
		registry.createAction(tmp, "growflower", entityIndex, 0, 0, gameTime()+duration);
	}
}

void GameClass::processMagic(int entityIndex)
{
	if(entityIndex==0) return;
	float duration=0.5f;
	if(registry.obj.regEntities[entityIndex]->frame < 8)
	{
		registry.obj.regEntities[entityIndex]->frame += 1;
		registry.createAction(tmp, "effects", entityIndex, 0, 0, gameTime()+duration);
	}
}

void GameClass::processFlowerConversion(int entityIndex)
{
	if(registry.obj.regEntities[entityIndex]->type != ICAT_VEGETATION) return;
	int veg=registry.obj.regEntities[entityIndex]->packIndex;
	int drops=registry.obj.regVeg[veg]->dropList;
	coord psx=registry.obj.regEntities[entityIndex]->pos;
	int pick=registry.obj.randomEntityFromList(tmp, tmp.container.valuesList[drops].cname);
	registry.obj.eraseEntity(entityIndex);
	fillEntity(tmp.container.entityList[pick].cname, psx);
}

bool GameClass::validateAction(const actionStruct* act)
{
	if(act==NULL) return false;
	if(!act->active) return false;
	if(act->entityIndexSource<0) return false;
	if(act->entityIndexTarget<0 || act->entityIndexTarget>numberOfEntities()) return false;
	if(act->tileIndexTarget<0 || act->tileIndexTarget>numberOfTiles()) return false;
	return true;
}

bool GameClass::isPerformingAction(int entityIndex, const char* actionName)
{
	bool ret=false;
	for(int i=1; i<int(registry.obj.actions.size()); i++)
	{
		if(actionCodeEquals(registry.obj.actions[i]->actionTemplateIndex, actionName) && registry.obj.actions[i]->entityIndexSource==entityIndex)
		{
			if(registry.obj.actions[i]->active)
				ret=true;
		}
	}
	return ret;
}

void GameClass::fillSourceAction(const char* actionname, int entityIndex, float delay)
{
	registry.createAction(tmp, actionname, entityIndex, 0, 0, gameTime()+delay);
}

void GameClass::fillEntityTargetAction(const char* actionname, int entityIndex, int entityTarget, float delay)
{
	registry.createAction(tmp, actionname, entityIndex, entityTarget, 0, gameTime()+delay);
}

void GameClass::fillTileTargetAction(const char* actionname, int entityIndex, int tileTarget, float delay)
{
	registry.createAction(tmp, actionname, entityIndex, 0, tileTarget, gameTime()+delay);
}

coord GameClass::getLatestTargetPosition(int entityIndex)
{
	int latestTile=0;
	int latestEnt=0;
	float tiletime=0.0f;
	float enttime=0.0f;
	for(int i=1; i<int(registry.obj.actions.size()); i++)
	{
		if(registry.obj.actions[i]->entityIndexSource==entityIndex)
		{
			float time=registry.obj.actions[i]->timeToActivate;
			if(actionCodeEquals(registry.obj.actions[i]->actionTemplateIndex, "movestep") || actionCodeEquals(registry.obj.actions[i]->actionTemplateIndex, "establishtarget"))
			{
				int tile=registry.obj.actions[i]->tileIndexTarget;
				int ent=registry.obj.actions[i]->entityIndexTarget;
				if(tile>0 && time>tiletime)
				{
					tiletime=time;
					latestTile=tile;
				}
				if(ent>0 && time>enttime)
				{
					enttime=time;
					latestEnt=ent;
				}
			}
		}
	}
	if(enttime==tiletime && tiletime==0.0f) return registry.obj.regEntities[entityIndex]->pos;
	if(enttime>tiletime)
	{
		//entity position is latest
		return registry.obj.regEntities[latestEnt]->pos;
	}
	else
	{
		return registry.obj.regTiles[latestTile]->pos;
	}
	return coord(0,0);
}

bool GameClass::hasSource(const actionStruct* act)
{
	return (act->entityIndexSource != 0 && registry.obj.regEntities[act->entityIndexSource] != NULL);
}

bool GameClass::noTarget(const actionStruct* act)
{
	return (!(entityTarget(act) || tileTarget(act)));
}

bool GameClass::entityTarget(const actionStruct* act)
{
	return (act->entityIndexTarget != 0 && registry.obj.regEntities[act->entityIndexTarget] != NULL);
}

bool GameClass::tileTarget(const actionStruct* act)
{
	return (act->tileIndexTarget != 0 && registry.obj.regTiles[act->tileIndexTarget] != NULL);
}

bool GameClass::fullTargets(const actionStruct* act)
{
	return (entityTarget(act) && tileTarget(act));
}

void GameClass::handleMovementPipeline(const actionStruct* act)
{
	if(actionCodeEquals(act->actionTemplateIndex, "selecttile"))
	{
		if(gamemode==GAMEMODE_ENTITYTARGETING)
		{
			int regEnt=registry.obj.getButtonLinkedEntity(tmp, "movecreature");
			if(tileTarget(act))
			{
				fillTileTargetAction("establishtarget", regEnt, act->tileIndexTarget, 0.5f);
			}
			fillSourceAction("creatureguioff", regEnt);
			return;
		}
	}
	if(actionCodeEquals(act->actionTemplateIndex, "selectentity"))
	{
		if(gamemode==GAMEMODE_ENTITYTARGETING)
		{
			int regEnt=registry.obj.getButtonLinkedEntity(tmp, "movecreature");
			fillEntityTargetAction("establishtarget", regEnt, act->entityIndexTarget, 0.5f);
			fillSourceAction("creatureguioff", regEnt);
			return;
		}
	}
	if(actionCodeEquals(act->actionTemplateIndex, "establishtarget"))
	{
		char buffer[64]="";
		if(tileTarget(act))
		{
			sprintf_s(buffer, "(%i, %i)", registry.obj.regTiles[act->tileIndexTarget]->pos.x, registry.obj.regTiles[act->tileIndexTarget]->pos.y);
			fillTileTargetAction("movestep", act->entityIndexSource, act->tileIndexTarget);
		}
		else
		{
			sprintf_s(buffer, "(%i, %i)", registry.obj.regEntities[act->entityIndexTarget]->pos.x, registry.obj.regEntities[act->entityIndexTarget]->pos.y);
			fillEntityTargetAction("movestep", act->entityIndexSource, act->entityIndexTarget);
		}
		sidebar.setString("Target moving to:\n" + std::string(buffer));
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "movestep"))
	{
		if(!hasSource(act) || noTarget(act)) return;
		if(registry.obj.regEntities[act->entityIndexSource]->type != ICAT_CREATURE) return;
		coord d;
		coord grid;
		coord ff;
		coord fine;
		grid=registry.obj.regEntities[act->entityIndexSource]->pos;
		ff=coord(0,0);
		fine=registry.obj.regCreature[registry.obj.regEntities[act->entityIndexSource]->packIndex]->offset;
		if(entityTarget(act))
		{
			if(calcDist(toVector(registry.obj.regEntities[act->entityIndexTarget]->pos), toVector(registry.obj.regEntities[act->entityIndexSource]->pos))<1.4f)
			{
				fillEntityTargetAction("engagecombat", act->entityIndexSource, act->entityIndexTarget);
				return;
			}
			fillPathingRoutes();
			terrain.setTerrainRuleAt(registry.obj.regEntities[act->entityIndexTarget]->pos, 0);
			d=astar.getNextTile(terrain, registry.obj.regEntities[act->entityIndexSource]->pos, registry.obj.regEntities[act->entityIndexTarget]->pos);
		}
		else
		{
			fillPathingRoutes();
			if(registry.obj.regTiles[act->tileIndexTarget]->pos != getLatestTargetPosition(act->entityIndexSource)) return;
			d=astar.getNextTile(terrain, registry.obj.regEntities[act->entityIndexSource]->pos, registry.obj.regTiles[act->tileIndexTarget]->pos);
		}
		if(d==grid)
		{
			registry.obj.regEntities[act->entityIndexSource]->pos=d;
			return;
		}
		if(d.x>grid.x) {ff=coord(4,0);}
		else if(d.x<grid.x) {ff=coord(-4,0);}
		else if(d.y>grid.y) ff=coord(0,4);
		else if(d.y<grid.y) ff=coord(0,-4);

		fine = fine+ff;
		fine.x=fine.x%settings.tileWid;
		fine.y=fine.y%settings.tileHig;

		if(fine==coord(0,0)) grid=d;
		registry.obj.regEntities[act->entityIndexSource]->box.left=(grid.x*32)+fine.x;
		registry.obj.regEntities[act->entityIndexSource]->box.top=(grid.y*32)+fine.y;

		registry.obj.regEntities[act->entityIndexSource]->pos=grid;
		registry.obj.regCreature[registry.obj.regEntities[act->entityIndexSource]->packIndex]->offset=fine;
		if(entityTarget(act))
		{
			fillEntityTargetAction("movestep", act->entityIndexSource, act->entityIndexTarget);
			return;
		}
		else
		{
			fillTileTargetAction("movestep", act->entityIndexSource, act->tileIndexTarget);
			return;
		}
	}
	if(actionCodeEquals(act->actionTemplateIndex, "movecreature"))
	{
		sidebar.setString("targeting!?\n"+outputEntity(act->entityIndexSource));
		gamemode=GAMEMODE_ENTITYTARGETING;
		return;
	}
}

void GameClass::handleCombatPipeline(const actionStruct* act)
{
	if(!entityTarget(act) || !hasSource(act)) return;
	if(actionCodeEquals(act->actionTemplateIndex, "attack"))
	{
		if(calcDist(toVector(registry.obj.regEntities[act->entityIndexTarget]->pos), toVector(registry.obj.regEntities[act->entityIndexSource]->pos))>1.4f)
			return;
		int regSlash = registry.createEntity(tmp, "claw", registry.obj.regEntities[act->entityIndexTarget]->pos, gameTime());
		fillEntityTargetAction("endattack", regSlash, act->entityIndexTarget, 0.15f);
		fillEntityTargetAction("engagecombat", act->entityIndexSource, act->entityIndexTarget, 0.5f);
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "endattack"))
	{
		//erase an effect from existence
		registry.obj.eraseEntity(act->entityIndexSource);

		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "engagecombat"))
	{
		char buffer[64]="";
		sprintf_s(buffer, "(%i, %i)", registry.obj.regEntities[act->entityIndexTarget]->pos.x, registry.obj.regEntities[act->entityIndexTarget]->pos.y);
		sidebar.setString("Combat Engaged:\n" + std::string(buffer));
		fillEntityTargetAction("attack", act->entityIndexSource, act->entityIndexTarget, 0.5f);
		return;
	}
}

void GameClass::handleCreationPipeline(const actionStruct* act)
{
	if(!hasSource(act)) return;
	if(actionCodeEquals(act->actionTemplateIndex, "makecreature"))
	{
		float seconds=float(rand()%7+2);
		fillSourceAction("wandercreature", act->entityIndexSource, seconds);
		fillButton("movebutton", coord(settings.tileCols+1,3), act->entityIndexSource, false);
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "growflower"))
	{
		//update the frame as long as it's still less than the max growth stages
		processGrowth(act->entityIndexSource);

		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "effects"))
	{
		//update the frame as long as it's still less than the max growth stages
		processMagic(act->entityIndexSource);

		return;
	}
}

void GameClass::handleButtonPipeline(const actionStruct* act)
{
	if(actionCodeEquals(act->actionTemplateIndex, "infoget"))
	{
		if(gamemode==GAMEMODE_INSPECT)
		{
			fillSourceAction("clearsidebar", 0, 0.125f);
		}
		else
		{
			sidebar.setString("Click an Entity\nto Inspect...");
			gamemode=GAMEMODE_INSPECT;
		}
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "generatemap"))
	{
		gamemode=GAMEMODE_NEUTRAL;
		initialize();
		experimentalMapGen("forest");
		fillButton("magnifier", coord(settings.tileCols, 5));
		fillButton("recycle", coord(settings.tileCols+1, 5));
		fillButton("camera", coord(settings.tileCols+2, 5));
		fillButton("backpack", coord(settings.tileCols+3, 5));
		fillButton("inventorycell", coord(0,0), 0, false);
		int toolIndex=registry.createEntity(tmp, "magicwand", coord(0,0), gameTime());
		registry.createAction(tmp, "randomheld", toolIndex, 0, 0, gameTime());
		int plantIndex=registry.createEntity(tmp, "s_redrose", coord(0,0), gameTime());
		inv.add(registry.obj.regEntities[plantIndex], plantIndex, 4);
		registry.cloneToInventory(plantIndex);
		plantIndex=registry.createEntity(tmp, "s_heaven", coord(0,0), gameTime());
		inv.add(registry.obj.regEntities[plantIndex], plantIndex, 4);
		registry.cloneToInventory(plantIndex);
		plantIndex=registry.createEntity(tmp, "s_hibiscus", coord(0,0), gameTime());
		inv.add(registry.obj.regEntities[plantIndex], plantIndex, 4);
		registry.cloneToInventory(plantIndex);
		plantIndex=registry.createEntity(tmp, "redcharm", coord(0,0), gameTime());
		inv.add(registry.obj.regEntities[plantIndex], plantIndex);
		registry.cloneToInventory(plantIndex);
		plantIndex=registry.createEntity(tmp, "redcharm", coord(0,0), gameTime());
		inv.add(registry.obj.regEntities[plantIndex], plantIndex);
		registry.cloneToInventory(plantIndex);
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "screenshot"))
	{
//		gamemode=GAMEMODE_NEUTRAL;
		capture();
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex,"backpack"))
	{
		if(gamemode!=GAMEMODE_INVENTORY) gamemode=GAMEMODE_INVENTORY;
		else gamemode=GAMEMODE_NEUTRAL;
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex,"selectinventory"))
	{
		if(gamemode==GAMEMODE_INVENTORY) inv.select(mouse);
		return;
	}
}

void GameClass::handleItemsPipeline(const actionStruct* act)
{
	if(actionCodeEquals(act->actionTemplateIndex, "usecharm"))
	{
		if(!hasSource(act)) return;
		int summonPack = registry.obj.regEntities[act->entityIndexSource]->packIndex;
		int held = registry.obj.regSummon[summonPack]->creatureContained;
		if(entityTarget(act))
		{
			registry.obj.regEntities[act->entityIndexTarget]->plane=1;
			registry.obj.regEntities[act->entityIndexTarget]->pos=coord(0,0);
			registry.obj.regEntities[act->entityIndexTarget]->active=false;
		
			if(held==0) registry.obj.regSummon[summonPack]->creatureContained=act->entityIndexTarget;
			return;
		}
		else if(tileTarget(act))
		{
			if(held>0)
			{
				int regMagic = registry.createEntity(tmp, "magiceffect", registry.obj.regTiles[act->tileIndexTarget]->pos, gameTime()+0.125f);
				fillSourceAction("effects", regMagic);
				registry.obj.regEntities[held]->plane=0;
				registry.obj.regEntities[held]->pos=registry.obj.regTiles[act->tileIndexTarget]->pos;
				registry.obj.regEntities[held]->active=true;

				//fix this, boxes will be wrong if not 32x32
				registry.obj.regEntities[held]->box.left=registry.obj.regEntities[held]->pos.x*32;
				registry.obj.regEntities[held]->box.top=registry.obj.regEntities[held]->pos.y*32;

				registry.obj.regSummon[summonPack]->creatureContained=0;
			}
			return;
		}
	}
	if(actionCodeEquals(act->actionTemplateIndex, "spelltile"))
	{
		int regMagic = registry.createEntity(tmp, "magiceffect", registry.obj.regTiles[act->tileIndexTarget]->pos, gameTime()+0.125f);
		fillSourceAction("effects", regMagic);
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "plantflower"))
	{
		if(registry.obj.regEntities[act->entityIndexSource]->type != ICAT_SEED) return;
		int seedIndex=registry.obj.regEntities[act->entityIndexSource]->packIndex;
		int vegTemplate=registry.obj.regSeed[seedIndex]->vegetationContained;
		fillEntity(tmp.container.entityList[vegTemplate].cname, registry.obj.regTiles[act->tileIndexTarget]->pos);
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "destroytool"))
	{
		//erase a tool from existence
		inv.clearSlot(inv.cursor);
		registry.obj.eraseEntity(act->entityIndexSource);

		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "convertflower"))
	{
		processFlowerConversion(act->entityIndexSource);
		return;
	}
}

void GameClass::handleAIPipeline(const actionStruct* act)
{
	if(actionCodeEquals(act->actionTemplateIndex, "wandercreature"))
	{
		float seconds=float(rand()%7+2);
		if(!isPerformingAction(act->entityIndexSource, "movestep"))
		{
			int dir=rand()%4;
			coord ff=registry.obj.regEntities[act->entityIndexSource]->pos;
			switch(dir)
			{
				case 0://north
					if(ff.y>0) ff=ff+coord(0,-1);
					break;
				case 1://east
					if(ff.x<settings.tileCols) ff=ff+coord(1,0);
					break;
				case 2://south
					if(ff.y<settings.tileRows) ff=ff+coord(0,1);
					break;
				default://west
					if(ff.x>0) ff=ff+coord(-1,0);
					break;
			}
			if(terrain.getTerrainAt(ff)==0)
			{
				registry.createAction(tmp, "movestep", act->entityIndexSource, 0, getTileIndexAt(ff), gameTime());
			}
		}
		fillSourceAction("wandercreature", act->entityIndexSource, seconds);
		return;
	}
}

void GameClass::handleGUIPipeline(const actionStruct* act)
{
	if(actionCodeEquals(act->actionTemplateIndex, "randomheld"))
	{
		inv.add(registry.obj.regEntities[act->entityIndexSource], act->entityIndexSource);
		registry.cloneToInventory(act->entityIndexSource);
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "creatureguion"))
	{
		if(gamemode!=GAMEMODE_ENTITYACTION)
		{
			gamemode=GAMEMODE_ENTITYACTION;
			sidebar.setString("SELECTED:\n" + outputEntity(act->entityIndexSource));
			registry.obj.activateEntityButtons(act->entityIndexSource);
		}
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex,"creatureguioff"))
	{
		registry.obj.deactivateEntityButtons(act->entityIndexSource);
		gamemode=GAMEMODE_NEUTRAL;
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "selecttile"))
	{
		if(gamemode==GAMEMODE_INSPECT)
		{
			sidebar.setString(outputTile(act->tileIndexTarget));
			return;
		}
	}
	if(actionCodeEquals(act->actionTemplateIndex, "selectentity"))
	{
		if(gamemode==GAMEMODE_INSPECT)
		{
			sidebar.setString(outputEntity(act->entityIndexTarget));
			fillSourceAction("convertflower", act->entityIndexTarget);
			return;
		}
		if(gamemode==GAMEMODE_NEUTRAL)
		{
			if(act->entityIndexTarget==0) return;
			if(registry.obj.regEntities[act->entityIndexTarget]->type == ICAT_CREATURE)
			{
				fillSourceAction("creatureguion", act->entityIndexTarget);
			}
			return;
		}
	}
	if(actionCodeEquals(act->actionTemplateIndex, "clearsidebar"))
	{
		sidebar.setString("");
		gamemode=GAMEMODE_NEUTRAL;
		return;
	}
}

//processes an active, valid and queued action
void GameClass::processAction(actionStruct* act)
{
	if(!validateAction(act)) return;

	debugFile << "Handling event: " << tmp.container.actionList[act->actionTemplateIndex].cname << "\t";
	debugFile << "Source: " << act->entityIndexSource << ": " << outputEntity(act->entityIndexSource).toAnsiString();
	debugFile << "Target Entity: " << act->entityIndexTarget << ": " << outputEntity(act->entityIndexTarget).toAnsiString();
	debugFile << "Target Tile: " << act->tileIndexTarget << ": " << outputTile(act->tileIndexTarget).toAnsiString();
	debugFile << "==================================================\n";
	handleMovementPipeline(act);
	handleCombatPipeline(act);
	handleCreationPipeline(act);
	handleButtonPipeline(act);
	handleItemsPipeline(act);
	handleAIPipeline(act);
	handleGUIPipeline(act);
}

void GameClass::capture()
{
	sf::Image shot(app.capture());
	char buffer[65];
	sprintf_s(buffer, 65, "screenshots/%i.png", int(gameTime()));
	shot.saveToFile(buffer);
	sidebar.setString(sf::String("Saved screen to:\n"+sf::String(buffer))+"\n");
}

void GameClass::experimentalMapGen(const char* biome)
{
	initRandom(header.randSeed);
	registry.createMapTerrain(tmp, biome);
	int i=registry.obj.regMaps.size()-1;
	if(registry.obj.regMaps[i] == NULL) return;
	else
	{
		createBaseMapLayer(registry.obj.regMaps[i]);
		createDecorationLayer(registry.obj.regMaps[i]);
		createEcologyLayer(registry.obj.regMaps[i]);
	}
	fillPathingRoutes();
}

void GameClass::fillPathingRoutes()
{
	//establish pathing rules
	for(int y=0; y<settings.tileRows; y++)
	{
		for(int x=0; x<settings.tileCols; x++)
		{
			if(registry.obj.numberOfEntitiesOnGrid(coord(x,y))>0)
			{
				terrain.setTerrainRuleAt(coord(x,y), 4);
			}
			else terrain.setTerrainRuleAt(coord(x,y),0);
		}
	}
}

void GameClass::createBaseMapLayer(const mapGenStruct* map)
{
	fillShape("box", tmp.container.tileList[map->baseTiles].cname, coord(0,0), coord(settings.tileCols, settings.tileRows));
	for(int j=1; j<int(map->shapeLayer.size()); j++)
	{
		fillShape(map->shapeLayer[j]->shapeName, tmp.container.tileList[map->shapeLayer[j]->terrainTiles].cname, coord(0,0), coord(settings.tileCols, settings.tileRows));
		fillShape(map->shapeLayer[j]->shapeName, tmp.container.tileList[map->shapeLayer[j]->terrainTiles].cname, coord(0,0), coord(settings.tileCols, settings.tileRows));
	}

}

void GameClass::createDecorationLayer(const mapGenStruct* map)
{
	for(int j=0; j<int(map->decoLayer.size()); j++)
		scatterEntity(map->decoLayer[j]);
}

void GameClass::createEcologyLayer(const mapGenStruct* map)
{
	for(int j=0; j<int(map->vegLayer.size()); j++)
		scatterEntity(map->vegLayer[j]);
	for(int j=0; j<int(map->creatureLayer.size()); j++)
		scatterEntity(map->creatureLayer[j]);
}

//_tl is top left of region
//_br is bottom right of region
//mainTileID is the id of the tile surrounding the shape and filling the region
//accentTileID is the id of the tile inside of the shape
//TODO: set up a registry for shapes. for now we'll handle the formation inside this function
void GameClass::fillShape(const char* shapename, const char* codename, coord _tl, coord _br)
{
	float A=0;	float B=0;	float C=0;	float D=0;	float E=0;	float F=0;
	float thetaRotation=(float(rand()%360)/360.0f)*(2.0f*PI);
	int rangeX=_br.x-_tl.x;
	int rangeY=_br.y-_tl.y;
	int axisX=rangeX/2;
	float ratio=float(rangeX)/float(rangeY);
	int adjY=0;
	int radius=rand()%5+5;
	coord origin=coord(0,0);
	coord vertex=coord(rand()%5+5, rand()%5+5);
	coord focus=coord(vertex.x+rand()%5+1, vertex.y);
	int p=focus.x/4;
	for(int y=0; y<rangeY; y++)
	{
		for(int x=0; x<rangeX; x++)
		{
			float yt=(float(-1.0f*x*sinf(thetaRotation))+(float(y*cosf(thetaRotation))));
			float xt=(float(x*cosf(thetaRotation))+float(y*sinf(thetaRotation)));
			if(codename == NULL) return;
			adjY=int(float(y)*ratio);
			if(strcmp(shapename, "box")==0)
			{
				A=0;B=0;C=0;D=0;E=0;F=0;
				if(processConic(coord(xt,yt), A, B, C, D, E, F)) fillTile(codename, _tl+coord(x,y));
			}
			if(strcmp(shapename, "circle")==0)
			{
				A=1;B=0;C=1;
				D=float((-2)*focus.x);E=float((-2)*focus.y);F=float(pow(focus.x,2)+pow(focus.y,2)-pow(radius,2));
				if(processConic(coord(xt,yt), A, B, C, D, E, F)) fillTile(codename, _tl+coord(x,y));
			}
			if(strcmp(shapename, "conic")==0)
			{
				A=(1.0f/float(pow(vertex.x, 2)));
				B=0;
				C=(-1.0f/float((pow(focus.x,2)-pow(vertex.x,2))));
				D=(-2.0f*float(origin.x))/float(pow(vertex.x, 2));
				E=(2.0f*float(origin.y))/float(pow(focus.x,2)-pow(vertex.x,2));
				F=float(pow(origin.x, 2)/pow(vertex.x, 2))-float(pow(origin.y, 2)/(pow(focus.x,2)-pow(vertex.x,2)))+1.0f;
				if(processConic(coord(xt,yt), A, B, C, D, E, F)) fillTile(codename, _tl+coord(x,y));
			}
			if(strcmp(shapename, "parabola")==0)
			{
				//(x-h)^2 = 4p(y-k)
				//x^2-2hx+h^2-4py+4pk=0
				A=1;
				B=0;
				C=0;
				D=-2.0f*vertex.x;
				E=-1.0f*focus.x;
				F=float(pow(vertex.x,2)+(1*focus.x*origin.y));
				if(processConic(coord(xt,yt), A, B, C, D, E, F)) fillTile(codename, _tl+coord(x,y));
			}
			if(strcmp(shapename, "triangle")==0)
			{
				if(adjY>=x)
				{
					fillTile(codename, _tl+coord(x,y));
				}
			}
		}
	}
}

bool GameClass::processConic(coord _pt, float a, float b, float c, float d, float e, float f)
{
	return (((a*pow(_pt.x,2))+(b*_pt.x*_pt.y)+(c*pow(_pt.y,2))+(d*_pt.x)+(e*_pt.x)+f)<=0);
}

void GameClass::scatterEntity(const mapSpreadStruct* spread)
{
	if(spread == NULL) return;
	for(int y=spread->tl.y; y<spread->br.y; y++)
	{
		for(int x=spread->tl.x; x<spread->br.x; x++)
		{
			if(rand()%100<spread->density)
			{
				fillEntity(tmp.container.entityList[spread->entityTemplateIndex].cname, coord(x,y));
			}
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

//places a tiles map of the selected tile index from the template registry
void GameClass::fillRoad(const char* codename, coord start, coord end)
{
	astar.initialize(start, end);
	coord current=start;
	while(current != end)
	{
		current = astar.getNextTile(terrain, current, end);
		if(!registry.createTile(tmp, codename, current, 16, rand()%50000))
		{
			//if there's nothing matching to clone, we must skip this step and inform the debug log
			debugFile << "FillRoad failed at (" << current.x << ", " << current.y << "). clone was undefined.\n";
			return;
		}
	}
}

void GameClass::fillEntity(const char* codename, coord _pos)
{
	if(registry.createEntity(tmp, codename, _pos, gameTime())<1)
	{
		//if there's nothing matching to clone, we must skip this step and inform the debug log
		debugFile << "FillEntity failed at (" << _pos.x << ", " << _pos.y << "). clone was undefined.\n";
		return;
	}
}

void GameClass::fillButton(const char* codename, coord _pos, int linkedEntity, bool act)
{
	if(!registry.createButton(tmp, codename, _pos, linkedEntity, act))
	{
		//if there's nothing matching to clone, we must skip this step and inform the debug log
		debugFile << "FillGui failed at (" << _pos.x << ", " << _pos.y << "). clone was undefined.\n";
		return;
	}
}

GameClass::~GameClass()
{
	registry.clear();
	debugFile << "Closed with " << int(registry.obj.actions.size()) << " pending/created actions:\n";
	for(int i=1; i<int(registry.obj.actions.size()); i++)
	{
		int actTemp=registry.obj.actions[i]->actionTemplateIndex;
		debugFile << tmp.container.actionList[actTemp].cname << "-->\t";
		debugFile << registry.obj.actions[i]->entityIndexSource << "\t";
		debugFile << registry.obj.actions[i]->entityIndexTarget << "\t";
		debugFile << registry.obj.actions[i]->tileIndexTarget << "\n";
	}
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
	//only returns newest registrered index
	for(int i=numberOfEntities()-1; i>0; i--)
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
	for(int i=numberOfTiles()-1; i>0; i--)
	{
		if(registry.obj.regTiles[i]->pos == mouse)
		{
			return i;
		}
	}
	return 0;
}

int GameClass::getTileIndexAt(coord pos)
{
	//if(entityHover()>0) return 0; //maybe??
	for(int i=numberOfTiles()-1; i>0; i--)
	{
		if(registry.obj.regTiles[i]->pos == pos)
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
		if(!(mouse.x<inv.tl.x || mouse.y<inv.tl.y || mouse.x>inv.tl.x+inv.dimensions.x || mouse.y>inv.tl.y+inv.dimensions.y))
		{
			return true;
		}
		return (buttonHover()>0);
	}
	return false;
}

void GameClass::handleBoardClick(coord _mouse)
{
	int entityIndex=entityHover();
	int tileIndex=tileHover();
	if(entityIndex>0)
	{
		if(inv.getItemAtCursor()>0 && gamemode==GAMEMODE_INVENTORY)
		{
			if(registry.obj.regEntities[inv.getItemAtCursor()]->type==ICAT_TOOL)
			{
				useTool(inv.getItemAtCursor(), entityIndex, 0);
				sidebar.setString(outputEntity(inv.getItemAtCursor()));
			}
			if(registry.obj.regEntities[inv.getItemAtCursor()]->type==ICAT_SUMMON)
			{
				useCharm(inv.getItemAtCursor(), entityIndex, 0);
				sidebar.setString(outputEntity(inv.getItemAtCursor()));
			}
		}
		else
		{
			registry.createAction(tmp, "selectentity", 0, entityIndex,0,gameTime()+0.125f);
		}
	}
	else if(tileIndex>0)
	{
		if(inv.getItemAtCursor()>0 && gamemode==GAMEMODE_INVENTORY)
		{
			if(registry.obj.regEntities[inv.getItemAtCursor()]->type==ICAT_TOOL)
			{
				useTool(inv.getItemAtCursor(), 0, tileIndex);
				sidebar.setString(outputEntity(inv.getItemAtCursor()));
				return;
			}
			if(registry.obj.regEntities[inv.getItemAtCursor()]->type==ICAT_SEED)
			{
				plantSeed(inv.drop(inv.cursor), 0, tileIndex);
				sidebar.setString(outputEntity(inv.getItemAtCursor()));
				return;
			}
			if(registry.obj.regEntities[inv.getItemAtCursor()]->type==ICAT_SUMMON)
			{
				useCharm(inv.getItemAtCursor(), 0, tileIndex);
				sidebar.setString(outputEntity(inv.getItemAtCursor()));
				return;
			}
		}
		else
		{
			registry.createAction(tmp, "selecttile", 0, 0,tileIndex,gameTime()+0.125f);
		}
	}
}

void GameClass::useTool(int entityIndex, int entityTarget, int tileTarget)
{
	if(entityTarget==0 && tileTarget==0) return;
	if(entityTarget>0)
	{
		return;
	}
	if(tileTarget>0)
	{
		//work on a tile
		int toolPack=registry.obj.regEntities[entityIndex]->packIndex;
		int protocol=registry.obj.regTool[toolPack]->usageProtocol;
		registry.obj.regTool[toolPack]->usesLeft-=1;
		if(registry.obj.regTool[toolPack]->usesLeft<1)
		{
			registry.createAction(tmp, "destroytool", entityIndex, 0, 0, gameTime()+0.5f);
		}
		registry.createAction(tmp, tmp.container.actionList[protocol].cname, entityIndex, 0, tileTarget, gameTime());
		return;
	}
}

void GameClass::useCharm(int entityIndex, int entityTarget, int tileTarget)
{
	if(entityIndex==0) return;
	if(entityTarget==0 && tileTarget==0) return;
	if(entityTarget>0)
	{
		//work on a tile (summon contained entity)
		int summonPack=registry.obj.regEntities[entityIndex]->packIndex;
		int held=registry.obj.regSummon[summonPack]->creatureContained;
		int protocol=registry.obj.regSummon[summonPack]->usageProtocol;
		bool isCreature = (registry.obj.regEntities[entityTarget]->type==ICAT_CREATURE);
		if(held==0 && isCreature) //if the charm is empty, we'll capture the creature
			registry.createAction(tmp, tmp.container.actionList[protocol].cname, entityIndex, entityTarget, 0, gameTime());
		return;
	}
	if(tileTarget>0)
	{
		//work on a tile (summon contained entity)
		int summonPack=registry.obj.regEntities[entityIndex]->packIndex;
		int held=registry.obj.regSummon[summonPack]->creatureContained;
		int protocol=registry.obj.regSummon[summonPack]->usageProtocol;
		if(held==0) return; //if the charm is empty, we have nothing to summon
		registry.createAction(tmp, tmp.container.actionList[protocol].cname, entityIndex, 0, tileTarget, gameTime());
		return;
	}
}

void GameClass::plantSeed(int entityIndex, int entityTarget, int tileTarget)
{
	if(entityTarget==0 && tileTarget==0) return;
	if(entityTarget>0)
	{
		return;
	}
	if(tileTarget>0)
	{
		//work on a tile
		int seedPack=registry.obj.regEntities[entityIndex]->packIndex;
		int protocol=registry.obj.regSeed[seedPack]->usageProtocol;
		registry.createAction(tmp, tmp.container.actionList[protocol].cname, entityIndex, 0, tileTarget, gameTime());
		return;
	}
}

void GameClass::handleGUIClick(coord _mouse)
{
	if(gamemode==GAMEMODE_INVENTORY)
	{
		if(!(_mouse.x<inv.tl.x || _mouse.y<inv.tl.y || _mouse.x>inv.tl.x+inv.dimensions.x || _mouse.y>inv.tl.y+inv.dimensions.y))
		{
			inv.select(_mouse);
			sidebar.setString(outputEntity(inv.getItemAtCursor()));
			return;
		}
	}
	int index=buttonHover();
	if(index==0 || !registry.obj.regButtons[index]->active) return;
	if(registry.obj.regButtons[index]->actionTemplateIndex>0)
	{
		for(int i=1; i<int(tmp.container.actionList.size()); i++)
		{
			if(registry.obj.regButtons[index]->actionTemplateIndex == i)
			{
				registry.createAction(tmp, tmp.container.actionList[i].cname, registry.obj.regButtons[index]->linkedEntityIndex, 0, 0,gameTime());
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
		{
			coord pixel=coord(registry.obj.regEntities[i]->pos.x*32, registry.obj.regEntities[i]->pos.y*32);
			if(registry.obj.regEntities[i]->type==ICAT_CREATURE)
			{
				pixel=pixel+(registry.obj.regCreature[registry.obj.regEntities[i]->packIndex]->offset);
			}
			render.DrawEntity(app, registry.obj.regEntities[i], pixel, (i==entityHover()));
		}
	}
	for(int i=1; i<int(registry.obj.regButtons.size()); i++)
	{
		if(registry.obj.regButtons[i] != NULL)
			render.DrawGui(app, registry.obj.regButtons[i], registry.obj.regButtons[i]->pos, i==buttonHover());
	}
	if(gamemode == GAMEMODE_INVENTORY)
	{
/*		sf::Image imgdummy;
		sf::Texture dummy;
		imgdummy.create(settings.winWid, settings.winHig, sf::Color(80, 80, 80, 188));
		dummy.loadFromImage(imgdummy);
		dummy.update(imgdummy);
		render.currentSprite.setTexture(dummy);
		render.currentSprite.setPosition(sf::Vector2f(0,0));
		render.currentSprite.setTextureRect(sf::IntRect(0,0,settings.winWid, settings.winHig));
		app.draw(render.currentSprite);*/
		render.DrawInventory(app, registry, inv, registry.obj.regButtons[5]);
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
	case ICAT_SUMMON: ret+="Summon Charm\n"; 
		calc=registry.obj.regEntities[index]->packIndex;
		calc=registry.obj.regSummon[calc]->creatureContained;
		ret+= outputEntity(calc); break;
	case ICAT_TOOL: ret+="Tool\n"; break;
	default: ret+="Something Else...\n"; break;
	}
	return ret;
}

sf::String GameClass::outputTile(int index)
{
	sf::String ret="";
	int calc=0;
	if(registry.obj.regTiles[index] == NULL) return ret;

	ret+=tmp.container.tileList[registry.obj.regTiles[index]->tileTemplateIndex].name;
	ret+="\n";
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