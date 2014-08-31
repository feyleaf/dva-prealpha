/*
game.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
August 23, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#include "globals.h"

//helps the constructor by reseting values and loading settings
void GameClass::initialize()
{
	loadSettings();
	astar.initBounds(settings);
	gameClock.restart();
	frameClock.restart();
	gameConstant=60;
}

void GameClass::refreshMap(coord map_pos)
{
	mapscale=1.0f;
	//header.randSeed=rand();
	//initRandom(header.randSeed);
	player.gamemode=GAMEMODE_NEUTRAL;
	registry.clear(map_pos);
}

//contructor, sets default values to member variables, opens debug log file
//then creates the 'app' window
GameClass::GameClass()
{
	dumpActionList=false;
	generatorCursor=coord(0,0);
	viewerCursor=coord(0,0);
	startTime=int(unsigned long(time(NULL))-unsigned long(JAN1_2014));
	newGame=true;
	player.quitGame=false;
	header.mapIndex=0;
	header.randSeed=(unsigned int)time(NULL);
	initRandom(header.randSeed);
	player.inv.clearAll();
	initialize();
	debugFile.open(settings.debugFile);
	tmp.parseFile(settings.templateDataFile);
	render.initialize(tmp, settings);
	app.create(sf::VideoMode(settings.screenDimensions.x, settings.screenDimensions.y, 32), settings.verTitle);
	//app.setFramerateLimit(60);
	render.resetView(app);
	player.initTileGrid(settings);
	player.initWindowSize(app);
	player.refreshClicks();
	//render.mainfont.loadFromFile(settings.mainFontFile);
	sidebar = sf::Text("Sidebar Line 1\nLine 2\nLine 3\nLine 4\nLast Line", render.mainfont, 20);
	sidebar.setPosition(float(settings.mapGridDimensions.x*settings.tileDimensions.x)+10.0f, 10.0f);
	registry.createAction(tmp, "generatemap", 0,0,0,0.0f, viewerCursor);
}

//runs an update each frame (~1/30 second)
bool GameClass::gameLoop()
{
	while(!player.quitGame)
	{
		inputHandler();

		//game world drawing routine

		if(frameClock.getElapsedTime().asSeconds()>FRAMESPEED) //force a constant framerate
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
			player.quitGame=true;
		}
	}
}

void GameClass::pollKeys()
{
}

int GameClass::translateClickLayer(coord _pixel)
{
	//input is the pixel of the current screen being clicked on by the player engine
	//(pass player.deliverRealClick(app)
	//returns a predefined code for the map layer to handle

	//start at the very top layer, which at the moment is only used for minimap
	if(player.gamemode==GAMEMODE_MINIMAP) return CLICKLAYER_TOP;

	//then test button click AND BUTTON ACTIVE STATE
	if(registry.objMap[viewerCursor].buttonIndexAtPoint(_pixel)>0)
	{
		int plc=registry.objMap[viewerCursor].buttonIndexAtPoint(_pixel);
		if(registry.objMap[viewerCursor].regButtons[plc]->active && registry.objMap[viewerCursor].regButtons[plc]->actionTemplateIndex>0)
			return CLICKLAYER_BUTTON;
	}
	else if(player.inventoryForm.getBox().contains(_pixel.x, _pixel.y) && player.inventoryForm.active)
	{
		//click on the inventory GUI but not on a button
		return CLICKLAYER_GUIFORM;
	}
	else if(player.ritualForm.getBox().contains(_pixel.x, _pixel.y) && player.ritualForm.active)
	{
		//click on the ritual GUI but not on a button
		return CLICKLAYER_GUIFORM;
	}
	else if(player.sideMenuForm.getBox().contains(_pixel.x, _pixel.y) && player.sideMenuForm.active)
	{
		//click on the side menu GUI but not on a button
		return CLICKLAYER_GUIFORM;
	}
	else if(player.creatureCard.getBox().contains(_pixel.x, _pixel.y) && player.creatureCard.active)
	{
		//click on the creature card GUI but not on a button
		return CLICKLAYER_GUIFORM;
	}
	else if(registry.objMap[viewerCursor].entityIndexAtPoint(_pixel)>0)
	{
		int plc=registry.objMap[viewerCursor].entityIndexAtPoint(_pixel);
		if(registry.objMap[viewerCursor].regEntities[plc]->active && registry.objMap[viewerCursor].regEntities[plc]->plane==0)
		{
			switch(registry.objMap[viewerCursor].regEntities[plc]->type)
			{
				case ICAT_CREATURE: return CLICKLAYER_CREATURES;
				case ICAT_DECORATION: return CLICKLAYER_DECORATIONS;
				case ICAT_VEGETATION: return CLICKLAYER_CREATURES;
				case ICAT_SEED: return CLICKLAYER_DECORATIONS;
				case ICAT_INGREDIENT: return CLICKLAYER_DECORATIONS;
				case ICAT_TOOL: return CLICKLAYER_DECORATIONS;
				case ICAT_SUMMON: return CLICKLAYER_DECORATIONS;
				default: break;
			}	
		}
	}
	else if(registry.objMap[viewerCursor].tileIndexOnGrid(coord(_pixel.x/settings.tileDimensions.x, _pixel.y/settings.tileDimensions.y))>0)
	{
		//click a tile
		return CLICKLAYER_TILE;
	}


	return CLICKLAYER_ZERO;

}

void GameClass::pollMouseClicks()
{
	if(player.gaveClick())
	{
		//mouse handling routine
		coord actionPoint = player.deliverRealClick(app);
		handleClick(translateClickLayer(actionPoint), actionPoint);		
	}
}

void GameClass::handleClick(int clickLayer, coord _pixel)
{
	int plc=0;
	switch(clickLayer)
	{
		case CLICKLAYER_CREATURES:
			plc=registry.objMap[viewerCursor].entityIndexAtPoint(player.deliverRealClick(app));
			fillEntityTargetAction("selectentity", 0, plc, 0.125f);
			break;
		case CLICKLAYER_DECORATIONS:
			plc=registry.objMap[viewerCursor].entityIndexAtPoint(player.deliverRealClick(app));
			fillEntityTargetAction("selectentity", 0, plc, 0.125f);
			break;
		case CLICKLAYER_BUTTON:
			plc=registry.objMap[viewerCursor].buttonIndexAtPoint(player.deliverRealClick(app));
			if(registry.objMap[viewerCursor].regButtons[plc]->actionTemplateIndex>0)
			{
				for(int i=1; i<int(tmp.container.actionList.size()); i++)
				{
					if(registry.objMap[viewerCursor].regButtons[plc]->actionTemplateIndex == i)
					{
						registry.createAction(tmp, tmp.container.actionList[i].cname, registry.objMap[viewerCursor].regButtons[plc]->linkedEntityIndex, 0, 0,gameTime(), viewerCursor);
						return;
					}
				}
			}
			break;
		case CLICKLAYER_TILE:
			plc=registry.objMap[viewerCursor].tileIndexOnGrid(player.deliverGridClick(app));
			if(player.inv.getItemAtCursor()>0 && player.gamemode==GAMEMODE_INVENTORY)
			{
				if(player.inv.reg.entities[player.inv.getItemAtCursor()]->type==ICAT_TOOL)
				{
					useTool(player.inv.getItemAtCursor(), 0, plc);
					return;
				}
				if(player.inv.reg.entities[player.inv.getItemAtCursor()]->type==ICAT_SEED)
				{
						plantSeed(player.inv.drop(player.inv.cursor), 0, plc);
						return;
				}
				if(player.inv.reg.entities[player.inv.getItemAtCursor()]->type==ICAT_INGREDIENT)
				{
					player.inv.drop(player.inv.cursor);
					return;
				}
				if(player.inv.reg.entities[player.inv.getItemAtCursor()]->type==ICAT_SUMMON)
				{
					useCharm(player.inv.getItemAtCursor(), 0, plc);
					sidebar.setString(outputEntity(player.inv.getItemAtCursor()));
					return;
				}
			}
			else
			{
				fillTileTargetAction("selecttile", 0, plc, 0.125f);
			}
			break;
		case CLICKLAYER_TOP:
			handleMinimapClick();
			break;
	}
}

bool GameClass::actionCodeEquals(int index, const char* _code)
{
	if(index==0 || index>int(tmp.container.actionList.size()) || _code==NULL || tmp.container.actionList[index].cname==NULL) return false;
	return (strcmp(tmp.container.actionList[index].cname, _code)==0);
}

coord GameClass::minimapHover(coord map_pos)
{
	//finemouse has to be along a map position
	//viewerCursor is at mapcenter
	coord minimapgrid=coord(player.deliverRealClick(app).x/(settings.mapGridDimensions.x*2), (player.deliverRealClick(app).y+16)/(settings.mapGridDimensions.y*2));
	return minimapgrid-coord(10,9)+map_pos;

}

//performs update tasks each frame
void GameClass::gameUpdater(float actSeconds)
{
	//must keep track of the size prior to running the action queues, because the queue will grow every tick
	//which could result in an infinite loop
	int thisTick=int(registry.objMap[viewerCursor].actions.size());
	processActionList(thisTick, actSeconds);
}

void GameClass::processActionList(int maxlength, float actSeconds)
{
	if(dumpActionList)
	{
		dumpActionList=false;
		return;
	}
	for(int i=1; i<maxlength; i++)
	{
		if(registry.objMap[viewerCursor].actions[i] != NULL && registry.objMap[viewerCursor].actions[i]->active)
		{
			if(registry.objMap[viewerCursor].actions[i]->timeToActivate<=actSeconds)
			{
				processAction(registry.objMap[viewerCursor].actions[i]);
				registry.objMap[viewerCursor].actions[i]->active=false;
			}
		}
	}
}

void GameClass::stopAction(int entityIndex, const char* actionName)
{
	int index=0;
	while(isPerformingAction(entityIndex, actionName))
	{
		index=getActionIndex(entityIndex, actionName);
		if(index>0)
		{
			registry.objMap[viewerCursor].actions[index]->active=false;
		}
	}
}

void GameClass::stopActionCategory(int entityIndex, int category)
{
	int index=0;
	for(int i=1; i<int(registry.objMap[viewerCursor].actions.size()); i++)
	{
		if(tmp.container.actionList[registry.objMap[viewerCursor].actions[i]->actionTemplateIndex].category==category && registry.objMap[viewerCursor].actions[i]->entityIndexSource==entityIndex)
		{
			if(registry.objMap[viewerCursor].actions[i]->active)
				index=i;
		}
		if(index>0)
		{
			registry.objMap[viewerCursor].actions[index]->active=false;
		}
	}
}

int GameClass::getActionIndex(int entityIndex, const char* actionName)
{
	for(int i=1; i<int(registry.objMap[viewerCursor].actions.size()); i++)
	{
		if(registry.objMap[viewerCursor].actions[i]->entityIndexSource == entityIndex && registry.objMap[viewerCursor].actions[i]->active)
		{
			if(actionCodeEquals(registry.objMap[viewerCursor].actions[i]->actionTemplateIndex, actionName))
			{
				return i;
			}
		}
	}
	return 0;
}

void GameClass::processGrowth(int entityIndex)
{
	int veg=registry.objMap[viewerCursor].regEntities[entityIndex]->packIndex;
	if(veg==0 || entityIndex==0) return;
	float duration=float(registry.objMap[viewerCursor].regVeg[veg]->growthTicks)*0.2f;
	if(registry.objMap[viewerCursor].regVeg[veg]->currentGrowth < (registry.objMap[viewerCursor].regVeg[veg]->maxGrowth-1))
	{
		registry.objMap[viewerCursor].regVeg[veg]->currentGrowth++;
		registry.objMap[viewerCursor].regEntities[entityIndex]->frame = registry.objMap[viewerCursor].regVeg[veg]->currentGrowth;
		registry.createAction(tmp, "growflower", entityIndex, 0, 0, gameTime()+duration, viewerCursor);
	}
}

void GameClass::processMagic(int entityIndex)
{
	if(entityIndex==0) return;
	float duration=0.5f;
	if(registry.objMap[viewerCursor].regEntities[entityIndex]->frame < 8)
	{
		registry.objMap[viewerCursor].regEntities[entityIndex]->frame += 1;
		registry.createAction(tmp, "effects", entityIndex, 0, 0, gameTime()+duration, viewerCursor);
	}
}

void GameClass::processFlowerConversion(int entityIndex)
{
	if(registry.objMap[viewerCursor].regEntities[entityIndex]->type != ICAT_VEGETATION) return;
	int veg=registry.objMap[viewerCursor].regEntities[entityIndex]->packIndex;
	int drops=registry.objMap[viewerCursor].regVeg[veg]->dropList;
	coord psx=registry.objMap[viewerCursor].regEntities[entityIndex]->pos;
	int pick=registry.objMap[viewerCursor].randomEntityFromList(tmp, tmp.container.valuesList[drops].cname);
	registry.objMap[viewerCursor].eraseEntity(entityIndex);
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

bool GameClass::validateTilePosition(coord pos)
{
	if(pos.x>settings.mapGridDimensions.x || pos.y>settings.mapGridDimensions.y || pos.x<0 || pos.y<0) return false;
	if(registry.objMap[viewerCursor].numberOfTilesOnGrid(pos)<1) return false;
	return true;
}

bool GameClass::validateEntity(int entityIndex)
{
	if(entityIndex<1 || entityIndex>numberOfEntities()) return false;
	if(!registry.objMap[viewerCursor].regEntities[entityIndex]->active) return false;
	if(registry.objMap[viewerCursor].regEntities[entityIndex]->plane>0) return false;
	return true;
}

bool GameClass::isCreature(int entityIndex)
{
	return (registry.objMap[viewerCursor].regEntities[entityIndex]->type==ICAT_CREATURE);
}

bool GameClass::isPerformingAction(int entityIndex, const char* actionName)
{
	bool ret=false;
	for(int i=1; i<int(registry.objMap[viewerCursor].actions.size()); i++)
	{
		if(actionCodeEquals(registry.objMap[viewerCursor].actions[i]->actionTemplateIndex, actionName) && registry.objMap[viewerCursor].actions[i]->entityIndexSource==entityIndex)
		{
			if(registry.objMap[viewerCursor].actions[i]->active)
				ret=true;
		}
	}
	return ret;
}

void GameClass::fillSourceAction(const char* actionname, int entityIndex, float delay)
{
	registry.createAction(tmp, actionname, entityIndex, 0, 0, gameTime()+delay, generatorCursor);
}

void GameClass::fillEntityTargetAction(const char* actionname, int entityIndex, int entityTarget, float delay)
{
	registry.createAction(tmp, actionname, entityIndex, entityTarget, 0, gameTime()+delay, generatorCursor);
}

void GameClass::fillTileTargetAction(const char* actionname, int entityIndex, int tileTarget, float delay)
{
	registry.createAction(tmp, actionname, entityIndex, 0, tileTarget, gameTime()+delay, generatorCursor);
}

coord GameClass::getLatestTargetPosition(int entityIndex)
{
	int latestTile=0;
	int latestEnt=0;
	float tiletime=0.0f;
	float enttime=0.0f;
	for(int i=1; i<int(registry.objMap[viewerCursor].actions.size()); i++)
	{
		if(registry.objMap[viewerCursor].actions[i]->entityIndexSource==entityIndex)
		{
			float time=registry.objMap[viewerCursor].actions[i]->timeToActivate;
			if(actionCodeEquals(registry.objMap[viewerCursor].actions[i]->actionTemplateIndex, "movestep") || actionCodeEquals(registry.objMap[viewerCursor].actions[i]->actionTemplateIndex, "establishtarget"))
			{
				int tile=registry.objMap[viewerCursor].actions[i]->tileIndexTarget;
				int ent=registry.objMap[viewerCursor].actions[i]->entityIndexTarget;
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
	if(enttime==tiletime && tiletime==0.0f) return registry.objMap[viewerCursor].regEntities[entityIndex]->pos;
	if(enttime>tiletime)
	{
		//entity position is latest
		return registry.objMap[viewerCursor].regEntities[latestEnt]->pos;
	}
	else
	{
		return registry.objMap[viewerCursor].regTiles[latestTile]->pos;
	}
	return coord(0,0);
}

bool GameClass::hasSource(const actionStruct* act)
{
	return (act->entityIndexSource != 0 && registry.objMap[viewerCursor].regEntities[act->entityIndexSource] != NULL);
}

bool GameClass::noTarget(const actionStruct* act)
{
	return (!(entityTarget(act) || tileTarget(act)));
}

bool GameClass::entityTarget(const actionStruct* act)
{
	return (act->entityIndexTarget != 0 && registry.objMap[viewerCursor].regEntities[act->entityIndexTarget] != NULL);
}

bool GameClass::tileTarget(const actionStruct* act)
{
	return (act->tileIndexTarget != 0 && registry.objMap[viewerCursor].regTiles[act->tileIndexTarget] != NULL);
}

bool GameClass::fullTargets(const actionStruct* act)
{
	return (entityTarget(act) && tileTarget(act));
}

coord GameClass::smartPathing(int entityIndex, coord pos)
{
	coord start=registry.objMap[viewerCursor].regEntities[entityIndex]->pos;
	coord ret=pos;
	coord north = pos+coord(0,-1);
	coord south = pos+coord(0,1);
	coord east = pos+coord(1, 0);
	coord west = pos+coord(-1, 0);
	coord closest=pos;
	int leastMoves=100;
	if(start==pos) return start;
	if(!astar.runPathing(terrain, start, pos))
	{
		//if there is a blocked pathing to the position, we find the next neighbor that is closest
		//and valid
		//test valid north
		if(astar.runPathing(terrain, start, north))
		{
			if(leastMoves>int(astar.pathList.size()))
			{
				leastMoves=int(astar.pathList.size());
				closest=north;
			}
		}
		if(astar.runPathing(terrain, start, south))
		{
			if(leastMoves>int(astar.pathList.size()))
			{
				leastMoves=int(astar.pathList.size());
				closest=south;
			}
		}
		if(astar.runPathing(terrain, start, east))
		{
			if(leastMoves>int(astar.pathList.size()))
			{
				leastMoves=int(astar.pathList.size());
				closest=east;
			}
		}
		if(astar.runPathing(terrain, start, west))
		{
			if(leastMoves>int(astar.pathList.size()))
			{
				leastMoves=int(astar.pathList.size());
				closest=west;
			}
		}
		//return the next tile based upon the closest
		//if none of the 4 directions are approachable, this will just return the start position :-/
		ret=astar.getNextTile(terrain, start, closest);
	}
	else
	{
		ret=astar.getNextTile(terrain, start, pos);
	}
	return ret;
}

void GameClass::gridAlignEntity(int entityIndex, coord _pos)
{
	registry.objMap[viewerCursor].regEntities[entityIndex]->pos=_pos;
	registry.objMap[viewerCursor].regEntities[entityIndex]->box.left=(_pos.x*32);
	registry.objMap[viewerCursor].regEntities[entityIndex]->box.top=(_pos.y*32);

	if(registry.objMap[viewerCursor].regEntities[entityIndex]->type != ICAT_CREATURE) return;
	registry.objMap[viewerCursor].regCreature[registry.objMap[viewerCursor].regEntities[entityIndex]->packIndex]->offset=coord(0,0);
	registry.objMap[viewerCursor].regCreature[registry.objMap[viewerCursor].regEntities[entityIndex]->packIndex]->velocity=coord(0,0);
}

//moving a creature along a path
/*
xstep 1: use the A* algorithm with 'smartPathing' to determine the next tile in the path (ultimate target)
xstep 2: commit the movement to that position, ensure that it is an adjacent VALID tile
step 3: set velocity to the creature based on the direction of the chosen tile, and its movement speed
step 4: if the tile contains any solid entity (terrain>0), we reverse the velocity and continue
step 5: if the 'offset' of the creature is not congruent with the velocity, we set the velocity to correct it
step 6: add velocity to offset, effectively moving the creature
step 7: repeat per frame from step 3 until the creature has (aligned with a grid)*, then turn off velocity and offset
step 8: repeat from step 1 until the ultimate target is reached
variables:
coord ultimate target (uTarget)
coord tile target (tTarget)
int entityIndex (creature)
int packIndex (creature pack)
coord offset
coord velocity
bool valid tile (isTileValid)
bool solid block (isSolidTerrain)
bool isEntityAligned
bool isOffsetCongruent
bool isTargetReached


*aligned with a grid: the axis from velocity has passed the tile size
*/

bool GameClass::isOffsetCongruent(int entityIndex)
{
	if(!isCreature(entityIndex)) return false;
	creaturePack* creature = registry.objMap[viewerCursor].regCreature[registry.objMap[viewerCursor].regEntities[entityIndex]->packIndex];
	if(creature->offset.x>0)
		return creature->velocity.x>0;
	if(creature->offset.y>0)
		return creature->velocity.y>0;
	if(creature->offset.x<0)
		return creature->velocity.x<0;
	if(creature->offset.y<0)
		return creature->velocity.y<0;
	return (creature->offset==coord(0,0));

}


void GameClass::forceOffsetCorrection(int entityIndex, coord direction)
{
	if(!isCreature(entityIndex)) return;
	creaturePack* creature = registry.objMap[viewerCursor].regCreature[registry.objMap[viewerCursor].regEntities[entityIndex]->packIndex];
	int speed = creature->move;
	if(creature->offset.x>0)
	{
		creature->velocity=coord(-speed, 0);
		return;
	}
	if(creature->offset.y>0)
	{
		creature->velocity=coord(0,-speed);
		return;
	}
	if(creature->offset.x<0)
	{
		creature->velocity=coord(speed, 0);
		return;
	}
	if(creature->offset.y<0)
	{
		creature->velocity=coord(0, speed);
		return;
	}
}

//sets the velocity of the creature
void GameClass::forcePushCreature(int entityIndex, coord direction)
{
	if(!isCreature(entityIndex)) return; //this is only operable on creatures
	int pack = registry.objMap[viewerCursor].regEntities[entityIndex]->packIndex;
	int xvel = direction.x*registry.objMap[viewerCursor].regCreature[pack]->move;
	int yvel = direction.y*registry.objMap[viewerCursor].regCreature[pack]->move;
	if(xvel==0 && yvel==0) return;
	if(xvel!=0 && yvel!=0) return;
	registry.objMap[viewerCursor].regCreature[pack]->velocity=coord(xvel, yvel);

}

bool GameClass::commitMovement(int entityIndex, coord tTarget, const actionStruct* act)
{
	//if we cannot validate movement, fail immediately
	if(!(validateTilePosition(tTarget) && validateEntity(entityIndex) && isCreature(entityIndex))) return false;

	//determine the cardinal direction of the next tile
	coord creaturePosition = registry.objMap[viewerCursor].regEntities[entityIndex]->pos;
	int dx = tTarget.x - creaturePosition.x;
	int dy = tTarget.y - creaturePosition.y;
	if(dx==0 && dy==0) return false; //if the creature is at the target, there is no movement to commit to
	if(dx!=0 && dy!=0) return false; //if there are multiple tiles between the creature and target, this fails too
	//now that that is out of the way, we determine the direction of movement
	if(dy>0) // move south
	{
		//commit to a southern movement
		if(entityTarget(act))
			fillEntityTargetAction("movesouth", entityIndex, act->entityIndexTarget);
		else
			fillTileTargetAction("movesouth", entityIndex, act->tileIndexTarget);
		return true;
	}
	if(dy<0) //move north
	{
		//commit to a northern movement
		if(entityTarget(act))
			fillEntityTargetAction("movenorth", entityIndex, act->entityIndexTarget);
		else
			fillTileTargetAction("movenorth", entityIndex, act->tileIndexTarget);
		return true;
	}
	if(dx>0) // move east
	{
		//commit to a eastern movement
		if(entityTarget(act))
			fillEntityTargetAction("moveeast", entityIndex, act->entityIndexTarget);
		else
			fillTileTargetAction("moveeast", entityIndex, act->tileIndexTarget);
		return true;
	}
	if(dx<0) //move west
	{
		//commit to a western movement
		if(entityTarget(act))
			fillEntityTargetAction("movewest", entityIndex, act->entityIndexTarget);
		else
			fillTileTargetAction("movewest", entityIndex, act->tileIndexTarget);
		return true;
	}
	return false; //if we made it this far, something went wrong, fail the commitment! :D

}

//fails if in movement, true if aligned with grid
//this carries the action for forced movement/velocity
bool GameClass::moveToAlign(int entityIndex)
{
	registeredEntity* ent = registry.objMap[viewerCursor].regEntities[entityIndex];
	creaturePack* pack = registry.objMap[viewerCursor].regCreature[ent->packIndex];
	coord off=pack->offset;
	coord vel=pack->velocity;
	coord pos=registry.objMap[viewerCursor].regEntities[entityIndex]->pos;

	//basing this solely on the velocity and the offset. we assume everything is valid with the data,
	//and that the velocity has been set to a direction that it needs to go
	if(abs(off.x+vel.x)>=settings.tileDimensions.x) //if we reached the target on the x axis:
	{
		//heading east, increment the position
		if(vel.x>0)
		{
			gridAlignEntity(entityIndex, pos+coord(1,0));
			return true;
		}
		//heading west, decrement the position
		if(vel.x<0)
		{
			gridAlignEntity(entityIndex, pos+coord(-1,0));
			return true;
		}
	}
	if(abs(off.y+vel.y)>=settings.tileDimensions.y) //if we reached the target on the y axis:
	{
		//heading south, increment the position
		if(vel.y>0)
		{
			gridAlignEntity(entityIndex, pos+coord(0,1));
			return true;
		}
		//heading north, decrement the position
		if(vel.y<0)
		{
			gridAlignEntity(entityIndex, pos+coord(0,-1));
			return true;
		}
	}
	off=off+vel;
	registry.objMap[viewerCursor].regCreature[ent->packIndex]->offset=off;
	return false;
}

int GameClass::getEnemyNeighbor(int entityIndex)
{
	coord pos=registry.objMap[viewerCursor].regEntities[entityIndex]->pos;
	for(int i=1; i<numberOfEntities(); i++)
	{
		if(registry.objMap[viewerCursor].regEntities[i]->active)
		{
			if(registry.objMap[viewerCursor].regEntities[i]->isEnemy)
			{
				if(calcDist(toVector(pos), toVector(registry.objMap[viewerCursor].regEntities[i]->pos))<1.4f)
				{
					return i;
				}
			}
		}
	}
	return 0;
}

bool GameClass::isReachedEntityTarget(int entityIndex, int entityTarget)
{
	coord pos=registry.objMap[viewerCursor].regEntities[entityIndex]->pos;
	coord trg=registry.objMap[viewerCursor].regEntities[entityTarget]->pos;
	if(calcDist(toVector(pos), toVector(trg))<1.4f)
	{
		return true;
	}
	return false;
}

bool GameClass::isEnemyNeighbor(int entityIndex)
{
	return (getEnemyNeighbor(entityIndex)!=0);
}

bool GameClass::mapExists(coord map_pos)
{
	return !(registry.objMap.find(map_pos) == registry.objMap.end());
}

void GameClass::handleMovementPipeline(const actionStruct* act)
{
	if(actionCodeEquals(act->actionTemplateIndex, "selecttile"))
	{
		if(player.gamemode==GAMEMODE_ENTITYTARGETING)
		{
			int regEnt=registry.objMap[viewerCursor].getButtonLinkedEntity(tmp, "movecreature");

			//if there is an entity on the tile, we select that entity as a target instead
			//quick fix for a movement glitch i found
			coord grid = registry.objMap[viewerCursor].regTiles[act->tileIndexTarget]->pos;
			if(registry.objMap[viewerCursor].entityIndexOnGrid(grid)>0)
			{
				fillEntityTargetAction("establishtarget", regEnt, registry.objMap[viewerCursor].entityIndexOnGrid(grid), 0.5f);
				fillSourceAction("creatureguioff", regEnt);
				return;
			}

			//otherwise move to the tile
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
		if(player.gamemode==GAMEMODE_ENTITYTARGETING)
		{
			int regEnt=registry.objMap[viewerCursor].getButtonLinkedEntity(tmp, "movecreature");
			fillEntityTargetAction("establishtarget", regEnt, act->entityIndexTarget, 0.5f);
			fillSourceAction("creatureguioff", regEnt);
			return;
		}
	}
	if(actionCodeEquals(act->actionTemplateIndex, "establishtarget"))
	{
		stopActionCategory(act->entityIndexSource, ACAT_COMBAT);
		if(tileTarget(act))
		{
			fillTileTargetAction("movestep", act->entityIndexSource, act->tileIndexTarget, 0.567f);
		}
		else
		{
			fillEntityTargetAction("movestep", act->entityIndexSource, act->entityIndexTarget, 0.567f);
		}
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "movestep"))
	{
		if(!hasSource(act) || noTarget(act)) return;
		if(registry.objMap[viewerCursor].regEntities[act->entityIndexSource]->type != ICAT_CREATURE) return;
		if(isEnemyNeighbor(act->entityIndexSource))
		{
			int enemy=getEnemyNeighbor(act->entityIndexSource);
			fillSourceAction("canceltarget", act->entityIndexSource);
			fillEntityTargetAction("engagecombat", act->entityIndexSource, enemy, 0.325f);
			return;
		}
		coord tTarget;
		fillPathingRoutes();
		if(entityTarget(act))
		{
			if(isReachedEntityTarget(act->entityIndexSource, act->entityIndexTarget))
			{
				fillSourceAction("canceltarget", act->entityIndexSource);
				return;
			}
			tTarget=smartPathing(act->entityIndexSource, registry.objMap[viewerCursor].regEntities[act->entityIndexTarget]->pos);
			if(!commitMovement(act->entityIndexSource, tTarget, act))
				fillSourceAction("canceltarget", act->entityIndexSource);
			return;
		}
		else
		{
			tTarget=smartPathing(act->entityIndexSource, registry.objMap[viewerCursor].regTiles[act->tileIndexTarget]->pos);
			if(!commitMovement(act->entityIndexSource, tTarget, act))
				fillSourceAction("canceltarget", act->entityIndexSource);
			return;
		}
	}
	if(actionCodeEquals(act->actionTemplateIndex, "movenorth"))
	{
		coord dir=coord(0,-1);
		coord trg=registry.objMap[viewerCursor].regEntities[act->entityIndexSource]->pos+dir;
		if(terrain.getTerrainAt(trg)>0) dir=coord(0,1);
		forcePushCreature(act->entityIndexSource, dir);
		if(!isOffsetCongruent(act->entityIndexSource))
		{
			forceOffsetCorrection(act->entityIndexSource, dir);
		}
		if(!moveToAlign(act->entityIndexSource))
		{
			if(entityTarget(act))
				fillEntityTargetAction("movenorth", act->entityIndexSource, act->entityIndexTarget, 0.567f);
			else
				fillTileTargetAction("movenorth", act->entityIndexSource, act->tileIndexTarget, 0.567f);
		}
		else
		{
			if(entityTarget(act))
				fillEntityTargetAction("movestep", act->entityIndexSource, act->entityIndexTarget, 0.567f);
			else
				fillTileTargetAction("movestep", act->entityIndexSource, act->tileIndexTarget, 0.567f);
		}
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "movesouth"))
	{
		coord dir=coord(0,1);
		coord trg=registry.objMap[viewerCursor].regEntities[act->entityIndexSource]->pos+dir;
		if(terrain.getTerrainAt(trg)>0) dir=coord(0,-1);
		forcePushCreature(act->entityIndexSource, dir);
		if(!isOffsetCongruent(act->entityIndexSource))
		{
			forceOffsetCorrection(act->entityIndexSource, dir);
		}
		if(!moveToAlign(act->entityIndexSource))
		{
			if(entityTarget(act))
				fillEntityTargetAction("movesouth", act->entityIndexSource, act->entityIndexTarget, 0.567f);
			else
				fillTileTargetAction("movesouth", act->entityIndexSource, act->tileIndexTarget, 0.567f);
		}
		else
		{
			if(entityTarget(act))
				fillEntityTargetAction("movestep", act->entityIndexSource, act->entityIndexTarget, 0.567f);
			else
				fillTileTargetAction("movestep", act->entityIndexSource, act->tileIndexTarget, 0.567f);
		}
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "moveeast"))
	{
		coord dir=coord(1,0);
		coord trg=registry.objMap[viewerCursor].regEntities[act->entityIndexSource]->pos+dir;
		if(terrain.getTerrainAt(trg)>0) dir=coord(-1,0);
		forcePushCreature(act->entityIndexSource, dir);
		if(!isOffsetCongruent(act->entityIndexSource))
		{
			forceOffsetCorrection(act->entityIndexSource, dir);
		}
		if(!moveToAlign(act->entityIndexSource))
		{
			if(entityTarget(act))
				fillEntityTargetAction("moveeast", act->entityIndexSource, act->entityIndexTarget, 0.567f);
			else
				fillTileTargetAction("moveeast", act->entityIndexSource, act->tileIndexTarget, 0.567f);
		}
		else
		{
			if(entityTarget(act))
				fillEntityTargetAction("movestep", act->entityIndexSource, act->entityIndexTarget, 0.567f);
			else
				fillTileTargetAction("movestep", act->entityIndexSource, act->tileIndexTarget, 0.567f);
		}
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "movewest"))
	{
		coord dir=coord(-1,0);
		coord trg=registry.objMap[viewerCursor].regEntities[act->entityIndexSource]->pos+dir;
		if(terrain.getTerrainAt(trg)>0) dir=coord(1,0);
		forcePushCreature(act->entityIndexSource, dir);
		if(!isOffsetCongruent(act->entityIndexSource))
		{
			forceOffsetCorrection(act->entityIndexSource, dir);
		}
		if(!moveToAlign(act->entityIndexSource))
		{
			if(entityTarget(act))
				fillEntityTargetAction("movewest", act->entityIndexSource, act->entityIndexTarget, 0.567f);
			else
				fillTileTargetAction("movewest", act->entityIndexSource, act->tileIndexTarget, 0.567f);
		}
		else
		{
			if(entityTarget(act))
				fillEntityTargetAction("movestep", act->entityIndexSource, act->entityIndexTarget, 0.567f);
			else
				fillTileTargetAction("movestep", act->entityIndexSource, act->tileIndexTarget, 0.567f);
		}
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "movecreature"))
	{
		sidebar.setString("");
		player.gamemode=GAMEMODE_ENTITYTARGETING;
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "canceltarget"))
	{
		stopActionCategory(act->entityIndexSource, ACAT_MOVEMENT);
		stopAction(act->entityIndexSource, "pursuit");
		gridAlignEntity(act->entityIndexSource, registry.objMap[viewerCursor].regEntities[act->entityIndexSource]->pos);
		return;
	}
}

void GameClass::handleCombatPipeline(const actionStruct* act)
{
	if(!entityTarget(act) || !hasSource(act) || (act->entityIndexSource == act->entityIndexTarget)) return;
	if(actionCodeEquals(act->actionTemplateIndex, "attack"))
	{
		if(calcDist(toVector(registry.objMap[viewerCursor].regEntities[act->entityIndexTarget]->pos), toVector(registry.objMap[viewerCursor].regEntities[act->entityIndexSource]->pos))>1.4f)
			return;
		int regSlash = registry.createEntity(tmp, "claw", registry.objMap[viewerCursor].regEntities[act->entityIndexTarget]->pos, gameTime(), viewerCursor);
		fillEntityTargetAction("endattack", regSlash, act->entityIndexTarget, 0.15f);
		fillEntityTargetAction("engagecombat", act->entityIndexSource, act->entityIndexTarget, 0.5f);
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "endattack"))
	{
		//erase an effect from existence
		registry.objMap[viewerCursor].eraseEntity(act->entityIndexSource);

		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "engagecombat"))
	{

		stopActionCategory(act->entityIndexSource, ACAT_MOVEMENT);
		stopActionCategory(act->entityIndexTarget, ACAT_MOVEMENT);
		char buffer[64]="";
		sprintf_s(buffer, "(%i, %i)", registry.objMap[viewerCursor].regEntities[act->entityIndexTarget]->pos.x, registry.objMap[viewerCursor].regEntities[act->entityIndexTarget]->pos.y);
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
		//fillSourceAction("wandercreature", act->entityIndexSource, seconds);
		fillButton("movebutton", gridToPixel(coord(settings.mapGridDimensions.x+1,1)), act->entityIndexSource, false);
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "makeenemy"))
	{
		registry.objMap[viewerCursor].regEntities[act->entityIndexSource]->isEnemy=true;
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
	if(actionCodeEquals(act->actionTemplateIndex, "togglemap"))
	{
		fillSourceAction("zoomout", act->entityIndexSource);
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "infoget"))
	{
		if(player.gamemode==GAMEMODE_INSPECT)
		{
			fillSourceAction("clearsidebar", 0, 0.125f);
		}
		else
		{
			sidebar.setString("Click an Entity\nto Inspect...");
			player.gamemode=GAMEMODE_INSPECT;
		}
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "generatemap"))
	{
		dumpActionList=true;
		player.gamemode=GAMEMODE_NEUTRAL;
		for(int y=-1; y<=1; y++)
		{
			for(int x=-1; x<=1; x++)
			{
				if(!mapExists(coord(x,y)+viewerCursor) || newGame)
				{
					generatorCursor=coord(x,y)+viewerCursor;
					refreshMap(generatorCursor);
					registry.objMap.insert(std::pair<coord, GameObjectContainerClass>(generatorCursor, GameObjectContainerClass()));
					experimentalMapGen(generatorCursor, "forest");
				}
			}
		}
		generatorCursor=viewerCursor;
		newGame=false;
		player.ritualForm.clear();
		player.inventoryForm.clear();
		player.sideMenuForm.clear();
		player.creatureCard.clear();
		player.sideMenuForm.addCell(RENDER_BUTTON, registry.objMap[viewerCursor].getGuiTemplateIndex(tmp, "magnifier"), gridToPixel(coord(settings.mapGridDimensions.x,5)));
		player.sideMenuForm.addCell(RENDER_BUTTON, registry.objMap[viewerCursor].getGuiTemplateIndex(tmp, "backpack"), gridToPixel(coord(settings.mapGridDimensions.x+1,4)));
		player.sideMenuForm.addCell(RENDER_BUTTON, registry.objMap[viewerCursor].getGuiTemplateIndex(tmp, "worldmap"), gridToPixel(coord(settings.mapGridDimensions.x+2,5)));
		player.sideMenuForm.addCell(RENDER_BUTTON, registry.objMap[viewerCursor].getGuiTemplateIndex(tmp, "camera"), gridToPixel(coord(settings.mapGridDimensions.x+1,6)));
		fillGuiForm(player.sideMenuForm);
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
		if(player.gamemode!=GAMEMODE_INVENTORY)
		{
			player.inventoryForm.clear();
			for(int j=0; j<25; j++)
				player.inventoryForm.addCell(RENDER_BUTTON, registry.objMap[viewerCursor].getGuiTemplateIndex(tmp, "inventorycell"), gridToPixel(coord(j%5, int(j/5))));
			for(int i=0; i<int(player.inv.cellList.size()); i++)
			{
				if(player.inv.cellList[i].tmp_idx>0)
				{
					player.inventoryForm.addCell(RENDER_ENTITY, player.inv.cellList[i].tmp_idx, gridToPixel(coord(i%5, int(i/5))));
				}
			}
			fillGuiForm(player.inventoryForm);
			player.inventoryForm.activate();
			player.gamemode=GAMEMODE_INVENTORY;
		}
		else
		{
			eraseGuiForm(player.inventoryForm);
			player.inventoryForm.clear();
			player.gamemode=GAMEMODE_NEUTRAL;
		}
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex,"selectinventory"))
	{
		if(player.gamemode==GAMEMODE_INVENTORY) player.inv.select(player.deliverGridClick(app));
		return;
	}
}

void GameClass::handleItemsPipeline(const actionStruct* act)
{
	if(actionCodeEquals(act->actionTemplateIndex, "usecharm"))
	{
		if(!hasSource(act)) return;
		int summonPack = registry.objMap[viewerCursor].regEntities[act->entityIndexSource]->packIndex;
		int held = registry.objMap[viewerCursor].regSummon[summonPack]->creatureContained;
		if(entityTarget(act))
		{
			if(registry.objMap[viewerCursor].regEntities[act->entityIndexTarget]->type==ICAT_CREATURE)
			{
				int creature=registry.objMap[viewerCursor].regEntities[act->entityIndexTarget]->packIndex;
				registry.objMap[viewerCursor].regEntities[act->entityIndexTarget]->plane=1;
				registry.objMap[viewerCursor].regEntities[act->entityIndexTarget]->pos=coord(0,0);
				registry.objMap[viewerCursor].regCreature[creature]->offset=coord(0,0);
				registry.objMap[viewerCursor].regEntities[act->entityIndexTarget]->active=false;
			}		
			if(held==0) registry.objMap[viewerCursor].regSummon[summonPack]->creatureContained=act->entityIndexTarget;
			return;
		}
		else if(tileTarget(act))
		{
			if(held>0)
			{
				int regMagic = registry.createEntity(tmp, "magiceffect", registry.objMap[viewerCursor].regTiles[act->tileIndexTarget]->pos, gameTime()+0.125f, viewerCursor);
				fillSourceAction("effects", regMagic);
				if(registry.objMap[viewerCursor].regEntities[held]->type==ICAT_CREATURE)
				{
					int creature=registry.objMap[viewerCursor].regEntities[held]->packIndex;
					registry.objMap[viewerCursor].regCreature[creature]->offset=coord(0,0);
					registry.objMap[viewerCursor].regEntities[held]->plane=0;
					registry.objMap[viewerCursor].regEntities[held]->pos=registry.objMap[viewerCursor].regTiles[act->tileIndexTarget]->pos;
					registry.objMap[viewerCursor].regEntities[held]->active=true;
					stopActionCategory(held, ACAT_MOVEMENT);
					stopActionCategory(held, ACAT_COMBAT);

					registry.objMap[viewerCursor].regEntities[held]->box = tmp.container.entityList[registry.objMap[viewerCursor].regEntities[held]->entityTemplateIndex].box;
					registry.objMap[viewerCursor].regEntities[held]->box.left+=(registry.objMap[viewerCursor].regEntities[held]->pos.x*32);
					registry.objMap[viewerCursor].regEntities[held]->box.top+=(registry.objMap[viewerCursor].regEntities[held]->pos.y*32);
				}
				registry.objMap[viewerCursor].regSummon[summonPack]->creatureContained=0;
			}
			return;
		}
	}
	if(actionCodeEquals(act->actionTemplateIndex, "spelltile"))
	{
		int regMagic = registry.createEntity(tmp, "magiceffect", registry.objMap[viewerCursor].regTiles[act->tileIndexTarget]->pos, gameTime()+0.125f, viewerCursor);
		fillSourceAction("effects", regMagic);
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "plantflower"))
	{
		if(registry.objMap[viewerCursor].regEntities[act->entityIndexSource]->type != ICAT_SEED) return;
		int seedIndex=registry.objMap[viewerCursor].regEntities[act->entityIndexSource]->packIndex;
		int vegTemplate=registry.objMap[viewerCursor].regSeed[seedIndex]->vegetationContained;
		fillEntity(tmp.container.entityList[vegTemplate].cname, registry.objMap[viewerCursor].regTiles[act->tileIndexTarget]->pos);
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "destroytool"))
	{
		//erase a tool from existence
		player.inv.clearSlot(player.inv.cursor);
		registry.objMap[viewerCursor].eraseEntity(act->entityIndexSource);

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
	if(actionCodeEquals(act->actionTemplateIndex, "pursuit"))
	{
		if(entityTarget(act))
		{
			if(registry.objMap[viewerCursor].regEntities[act->entityIndexTarget]->active)
			{
				fillEntityTargetAction("movestep", act->entityIndexSource, act->entityIndexTarget, 0.567f);
				fillEntityTargetAction("pursuit", act->entityIndexSource, act->entityIndexTarget);
			}
		}
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "wandercreature"))
	{
		float seconds=float(rand()%7+2);
		if(!(isPerformingAction(act->entityIndexSource, "movestep")))
		{
			int dir=rand()%4;
			coord ff=registry.objMap[viewerCursor].regEntities[act->entityIndexSource]->pos;
			switch(dir)
			{
				case 0://north
					if(ff.y>0) ff=ff+coord(0,-1);
					break;
				case 1://east
					if(ff.x<settings.mapGridDimensions.x) ff=ff+coord(1,0);
					break;
				case 2://south
					if(ff.y<settings.mapGridDimensions.y) ff=ff+coord(0,1);
					break;
				default://west
					if(ff.x>0) ff=ff+coord(-1,0);
					break;
			}
			if(terrain.getTerrainAt(ff)==0)
			{
				fillTileTargetAction("movestep", act->entityIndexSource, getTileIndexAt(ff), 0.567f);
			}
		}
		//fillSourceAction("wandercreature", act->entityIndexSource, seconds);
		return;
	}
}

void GameClass::handleGUIPipeline(const actionStruct* act)
{
	if(actionCodeEquals(act->actionTemplateIndex, "guion"))
	{
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "zoomout"))
	{
		if(mapscale<float(16.0f))
		{
			player.gamemode=GAMEMODE_ZOOMOUT;
			mapscale*=(32.0f/31.0f);
			fillSourceAction("zoomout", act->entityIndexSource);
		}
		else player.gamemode=GAMEMODE_MINIMAP;
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "randomheld"))
	{
		player.inv.add(registry.objMap[viewerCursor], act->entityIndexSource);
		registry.cloneToInventory(act->entityIndexSource, viewerCursor);
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "creatureguion"))
	{
		sidebar.setString("");
		player.creatureCard.clear();
		if(player.gamemode!=GAMEMODE_ENTITYACTION)
		{
			player.gamemode=GAMEMODE_ENTITYACTION;
			player.creatureCard.addCell(RENDER_BUTTON, registry.objMap[viewerCursor].getGuiTemplateIndex(tmp, "inventorycell"), gridToPixel(coord(settings.mapGridDimensions.x, 1)));
			player.creatureCard.addCell(RENDER_ENTITY, registry.objMap[viewerCursor].regEntities[act->entityIndexSource]->entityTemplateIndex, gridToPixel(coord(settings.mapGridDimensions.x, 1)));
			player.creatureCard.addCell(RENDER_BUTTON, registry.objMap[viewerCursor].getGuiTemplateIndex(tmp, "movebutton"), gridToPixel(coord(settings.mapGridDimensions.x+1, 1)));

			//fillGuiForm(player.creatureCard, act->entityIndexSource, true);
			registry.objMap[viewerCursor].activateEntityButtons(act->entityIndexSource);
		}
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex,"creatureguioff"))
	{
		registry.objMap[viewerCursor].deactivateEntityButtons(act->entityIndexSource);
		eraseGuiForm(player.creatureCard);
		player.creatureCard.clear();
		player.gamemode=GAMEMODE_NEUTRAL;
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "selecttile"))
	{
		if(player.gamemode==GAMEMODE_INSPECT)
		{
			sidebar.setString(outputTile(act->tileIndexTarget));
			return;
		}
	}
	if(actionCodeEquals(act->actionTemplateIndex, "selectentity"))
	{
		if(player.gamemode==GAMEMODE_INSPECT)
		{
			sidebar.setString(outputEntity(act->entityIndexTarget));
			return;
		}
		if(registry.objMap[viewerCursor].regEntities[act->entityIndexTarget]->entityTemplateIndex == registry.objMap[viewerCursor].getEntityTemplateIndex(tmp, "ritualstump"))
		{
			fillSourceAction("togglecrafting", act->entityIndexTarget);
			return;
		}
		if(player.gamemode==GAMEMODE_NEUTRAL)
		{
			if(act->entityIndexTarget==0) return;
			if(registry.objMap[viewerCursor].regEntities[act->entityIndexTarget]->type == ICAT_CREATURE)
			{
				fillSourceAction("creatureguion", act->entityIndexTarget);
				return;
			}
			if(registry.objMap[viewerCursor].regEntities[act->entityIndexTarget]->type == ICAT_VEGETATION)
			{
				fillSourceAction("convertflower", act->entityIndexTarget);
				return;
			}
			if(registry.objMap[viewerCursor].regEntities[act->entityIndexTarget]->type == ICAT_INGREDIENT)
			{
				fillSourceAction("randomheld", act->entityIndexTarget);
				return;
			}
			return;
		}
	}
	if(actionCodeEquals(act->actionTemplateIndex, "clearsidebar"))
	{
		sidebar.setString("");
		player.gamemode=GAMEMODE_NEUTRAL;
		return;
	}
	if(actionCodeEquals(act->actionTemplateIndex, "togglecrafting"))
	{
		if(player.gamemode==GAMEMODE_CRAFTING)
		{
			sidebar.setString("");
			eraseGuiForm(player.ritualForm);
			player.ritualForm.clear();
			player.gamemode=GAMEMODE_NEUTRAL;
		}
		else
		{
			sidebar.setString("Crafting ON");
			player.ritualForm.addCell(RENDER_BUTTON, registry.objMap[viewerCursor].getGuiTemplateIndex(tmp, "ritualgui"), coord(90,60));
			fillGuiForm(player.ritualForm);
			player.gamemode=GAMEMODE_CRAFTING;
		}
		return;
	}
}

//processes an active, valid and queued action
void GameClass::processAction(actionStruct* act)
{
	if(!validateAction(act)) return;

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

void GameClass::experimentalMapGen(coord map_pos, const char* biome)
{
	header.mapIndex++;
	header.randSeed=rand()+header.mapIndex;
	initRandom(header.randSeed);
	registry.createMapTerrain(tmp, biome, generatorCursor);
	int i=registry.objMap[map_pos].regMaps.size()-1;
	if(registry.objMap[map_pos].regMaps[i] == NULL) return;
	else
	{
		createBaseMapLayer(registry.objMap[map_pos].regMaps[i]);
		createDecorationLayer(registry.objMap[map_pos].regMaps[i]);
		createEcologyLayer(registry.objMap[map_pos].regMaps[i]);
	}
	//some ideas
	tempImg.create(settings.mapGridDimensions.x, settings.mapGridDimensions.y, sf::Color::White);
	for(int i=1; i<int(registry.objMap[map_pos].regTiles.size()); i++)
	{
		if(registry.objMap[map_pos].regTiles[i] != NULL)
		{
			coord pos=registry.objMap[map_pos].regTiles[i]->pos;
			tempImg.setPixel(pos.x, pos.y, registry.objMap[map_pos].regTiles[i]->distortionColor);
		}
	}
	registry.objMap[map_pos].mapSheet.loadFromImage(tempImg);
	fillPathingRoutes();
}

void GameClass::fillPathingRoutes()
{
	//establish pathing rules
	for(int y=0; y<settings.mapGridDimensions.y; y++)
	{
		for(int x=0; x<settings.mapGridDimensions.x; x++)
		{
			if(registry.objMap[generatorCursor].numberOfEntitiesOnGrid(coord(x,y))>0)
			{
				terrain.setTerrainRuleAt(coord(x,y), 4);
			}
			else terrain.setTerrainRuleAt(coord(x,y),0);
		}
	}
}

void GameClass::createBaseMapLayer(const mapGenStruct* map)
{
	fillShape("box", tmp.container.tileList[map->baseTiles].cname, coord(0,0), coord(settings.mapGridDimensions.x, settings.mapGridDimensions.y));
	for(int j=1; j<int(map->shapeLayer.size()); j++)
	{
		fillShape(map->shapeLayer[j]->shapeName, tmp.container.tileList[map->shapeLayer[j]->terrainTiles].cname, coord(0,0), coord(settings.mapGridDimensions.x, settings.mapGridDimensions.y));
		fillShape(map->shapeLayer[j]->shapeName, tmp.container.tileList[map->shapeLayer[j]->terrainTiles].cname, coord(0,0), coord(settings.mapGridDimensions.x, settings.mapGridDimensions.y));
	}
}

void GameClass::createDecorationLayer(const mapGenStruct* map)
{
	for(int j=0; j<int(map->decoLayer.size()); j++)
		scatterEntity(map->decoLayer[j]);
	fillEntity("ritualstump", coord(rand()%settings.mapGridDimensions.x,rand()%settings.mapGridDimensions.y));
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
	initRandom(header.randSeed);
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
				A=float(0.0f);B=float(0.0f);C=float(0.0f);D=float(0.0f);E=float(0.0f);F=float(0.0f);
				if(processConic(coord(int(xt),int(yt)), A, B, C, D, E, F)) fillTile(codename, _tl+coord(x,y));
			}
			if(strcmp(shapename, "circle")==0)
			{
				A=1.0f;B=0.0f;C=1.0f;
				D=float((-2)*focus.x);E=float((-2)*focus.y);F=float(pow(focus.x,2)+pow(focus.y,2)-pow(radius,2));
				if(processConic(coord(int(xt),int(yt)), A, B, C, D, E, F)) fillTile(codename, _tl+coord(x,y));
			}
			if(strcmp(shapename, "conic")==0)
			{
				A=float(1.0f/float(pow(vertex.x, 2)));
				B=0.0f;
				C=float(-1.0f/float((pow(focus.x,2)-pow(vertex.x,2))));
				D=float(-2.0f*float(origin.x))/float(pow(vertex.x, 2));
				E=float(2.0f*float(origin.y))/float(pow(focus.x,2)-pow(vertex.x,2));
				F=float(pow(origin.x, 2)/pow(vertex.x, 2))-float(pow(origin.y, 2)/(pow(focus.x,2)-pow(vertex.x,2)))+1.0f;
				if(processConic(coord(int(xt),int(yt)), A, B, C, D, E, F)) fillTile(codename, _tl+coord(x,y));
			}
			if(strcmp(shapename, "parabola")==0)
			{
				//(x-h)^2 = 4p(y-k)
				//x^2-2hx+h^2-4py+4pk=0
				A=1.0f;
				B=0.0f;
				C=0.0f;
				D=float(-2.0f*vertex.x);
				E=float(-1.0f*focus.x);
				F=float(pow(vertex.x,2)+(1*focus.x*origin.y));
				if(processConic(coord(int(xt),int(yt)), A, B, C, D, E, F)) fillTile(codename, _tl+coord(x,y));
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
	if(!registry.createTile(tmp, codename, _pos, 16, rand()%50000, generatorCursor))
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
		if(!registry.createTile(tmp, codename, current, 16, rand()%50000, generatorCursor))
		{
			//if there's nothing matching to clone, we must skip this step and inform the debug log
			debugFile << "FillRoad failed at (" << current.x << ", " << current.y << "). clone was undefined.\n";
			return;
		}
	}
}

void GameClass::fillEntity(const char* codename, coord _pos)
{
	if(registry.createEntity(tmp, codename, _pos, gameTime(), generatorCursor)<1)
	{
		//if there's nothing matching to clone, we must skip this step and inform the debug log
		debugFile << "FillEntity failed at (" << _pos.x << ", " << _pos.y << "). clone was undefined.\n";
		return;
	}
}

int GameClass::fillButton(const char* codename, coord _pixel_pos, int linkedEntity, bool act)
{
	if(!registry.createButton(tmp, codename, _pixel_pos, viewerCursor, linkedEntity, act))
	{
		//if there's nothing matching to clone, we must skip this step and inform the debug log
		debugFile << "FillGui failed at (" << _pixel_pos.x << ", " << _pixel_pos.y << "). clone was undefined.\n";
		return 0;
	}
	return int(registry.objMap[viewerCursor].regButtons.size())-1;
}

void GameClass::fillGuiForm(GUIFormClass& form, int linked, bool active)
{
	for(int i=1; i<int(form.cells.size()); i++)
	{
		if(form.cells[i].renderType == RENDER_BUTTON)
		{
			if(form.cells[i].buttonIndex > 0)
			{
				registry.objMap[viewerCursor].regButtons[form.cells[i].buttonIndex]->active=true;
			}
			else
			{
				form.cells[i].buttonIndex = fillButton(tmp.container.buttonList[form.cells[i].templateIndex].cname, form.cells[i].pixel+form.tl, linked, active);
			}
		}
	}
}

void GameClass::eraseGuiForm(GUIFormClass& form)
{
	for(int i=1; i<int(form.cells.size()); i++)
	{
		if(form.cells[i].renderType == RENDER_BUTTON && form.cells[i].buttonIndex > 0)
		{
			registry.objMap[viewerCursor].regButtons[form.cells[i].buttonIndex]->active=false;
		}
	}

}

GameClass::~GameClass()
{
	//must iterate the collection of map positions, and free the memory
	for(std::map<coord, GameObjectContainerClass>::const_iterator it = registry.objMap.begin(); it != registry.objMap.end(); it++)
	{
		coord keyPosition = it->first;
		registry.clear(keyPosition);
	}
	debugFile << "Closed debug file\n";
	debugFile << "Time in unsigned long format: " << (unsigned long(time(NULL))) << "\n";
	debugFile.close();
	app.close();
}

//returns the size of the regTiles list
int GameClass::numberOfTiles()
{
	return int(registry.objMap[viewerCursor].regTiles.size());
}

int GameClass::numberOfEntities()
{
	return int(registry.objMap[viewerCursor].regEntities.size());
}

int GameClass::getTileIndexAt(coord pos)
{
	//if(entityHover()>0) return 0; //maybe??
	for(int i=numberOfTiles()-1; i>0; i--)
	{
		if(registry.objMap[viewerCursor].regTiles[i]->pos == pos)
		{
			return i;
		}
	}
	return 0;
}

void GameClass::handleBoardClick()
{
	int entityIndex=registry.objMap[viewerCursor].entityIndexAtPoint(player.deliverRealClick(app));
	int tileIndex=registry.objMap[viewerCursor].tileIndexOnGrid(player.deliverGridClick(app));
	if(entityIndex>0)
	{
		if(player.inv.getItemAtCursor()>0 && player.gamemode==GAMEMODE_INVENTORY)
		{
			if(player.inv.reg.entities[player.inv.getItemAtCursor()]->type==ICAT_TOOL)
			{
				useTool(player.inv.getItemAtCursor(), entityIndex, 0);
				sidebar.setString(outputEntity(player.inv.getItemAtCursor()));
			}
			if(player.inv.reg.entities[player.inv.getItemAtCursor()]->type==ICAT_SUMMON)
			{
				useCharm(player.inv.getItemAtCursor(), entityIndex, 0);
				sidebar.setString(outputEntity(player.inv.getItemAtCursor()));
			}
		}
		else
		{
			registry.createAction(tmp, "selectentity", 0, entityIndex,0,gameTime()+0.125f, viewerCursor);
		}
	}
	else if(tileIndex>0)
	{
		if(player.inv.getItemAtCursor()>0 && player.gamemode==GAMEMODE_INVENTORY)
		{
			if(player.inv.reg.entities[player.inv.getItemAtCursor()]->type==ICAT_TOOL)
			{
				useTool(player.inv.getItemAtCursor(), 0, tileIndex);
				sidebar.setString(outputEntity(player.inv.getItemAtCursor()));
				return;
			}
			if(player.inv.reg.entities[player.inv.getItemAtCursor()]->type==ICAT_SEED)
			{
				plantSeed(player.inv.drop(player.inv.cursor), 0, tileIndex);
				sidebar.setString(outputEntity(player.inv.getItemAtCursor()));
				return;
			}
			if(player.inv.reg.entities[player.inv.getItemAtCursor()]->type==ICAT_SUMMON)
			{
				useCharm(player.inv.getItemAtCursor(), 0, tileIndex);
				sidebar.setString(outputEntity(player.inv.getItemAtCursor()));
				return;
			}
		}
		else
		{
			registry.createAction(tmp, "selecttile", 0, 0,tileIndex,gameTime()+0.125f, viewerCursor);
		}
	}
}

void GameClass::handleMinimapClick()
{
	coord mapgrid=minimapHover(viewerCursor);
	if(mapExists(mapgrid))
	{
		//refreshMap(viewerCursor);
		viewerCursor=mapgrid;
		generatorCursor=mapgrid;
		dumpActionList=true;
		player.gamemode=GAMEMODE_NEUTRAL;
		mapscale=1.0f;
		render.resetView(app);
		render.viewport.zoom(mapscale);
		fillSourceAction("generatemap", 0);
		//refreshMap(mapgrid);
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
		int toolPack=registry.objMap[viewerCursor].regEntities[entityIndex]->packIndex;
		int protocol=registry.objMap[viewerCursor].regTool[toolPack]->usageProtocol;
		registry.objMap[viewerCursor].regTool[toolPack]->usesLeft-=1;
		if(registry.objMap[viewerCursor].regTool[toolPack]->usesLeft<1)
		{
			registry.createAction(tmp, "destroytool", entityIndex, 0, 0, gameTime()+0.5f, viewerCursor);
		}
		registry.createAction(tmp, tmp.container.actionList[protocol].cname, entityIndex, 0, tileTarget, gameTime(), viewerCursor);
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
		int summonPack=registry.objMap[viewerCursor].regEntities[entityIndex]->packIndex;
		int held=registry.objMap[viewerCursor].regSummon[summonPack]->creatureContained;
		int protocol=registry.objMap[viewerCursor].regSummon[summonPack]->usageProtocol;
		bool isCreature = (registry.objMap[viewerCursor].regEntities[entityTarget]->type==ICAT_CREATURE);
		if(held==0 && isCreature) //if the charm is empty, we'll capture the creature
			registry.createAction(tmp, tmp.container.actionList[protocol].cname, entityIndex, entityTarget, 0, gameTime(), viewerCursor);
		return;
	}
	if(tileTarget>0)
	{
		//work on a tile (summon contained entity)
		int summonPack=registry.objMap[viewerCursor].regEntities[entityIndex]->packIndex;
		int held=registry.objMap[viewerCursor].regSummon[summonPack]->creatureContained;
		int protocol=registry.objMap[viewerCursor].regSummon[summonPack]->usageProtocol;
		if(held==0) return; //if the charm is empty, we have nothing to summon
		registry.createAction(tmp, tmp.container.actionList[protocol].cname, entityIndex, 0, tileTarget, gameTime(), viewerCursor);
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
		int seedPack=registry.objMap[viewerCursor].regEntities[entityIndex]->packIndex;
		int protocol=registry.objMap[viewerCursor].regSeed[seedPack]->usageProtocol;
		registry.createAction(tmp, tmp.container.actionList[protocol].cname, entityIndex, 0, tileTarget, gameTime(), viewerCursor);
		return;
	}
}

void GameClass::handleGUIClick(GUIFormClass& form)
{
	int index=registry.objMap[viewerCursor].buttonIndexAtPoint(player.deliverRealClick(app));
	if(index==0 || !registry.objMap[viewerCursor].regButtons[index]->active) return;
	if(registry.objMap[viewerCursor].regButtons[index]->actionTemplateIndex>0)
	{
		for(int i=1; i<int(tmp.container.actionList.size()); i++)
		{
			if(registry.objMap[viewerCursor].regButtons[index]->actionTemplateIndex == i)
			{
				registry.createAction(tmp, tmp.container.actionList[i].cname, registry.objMap[viewerCursor].regButtons[index]->linkedEntityIndex, 0, 0,gameTime(), viewerCursor);
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
	render.resetView(app);
	if(player.gamemode==GAMEMODE_ZOOMOUT || player.gamemode==GAMEMODE_MINIMAP)
	{
		render.viewport.zoom(mapscale);
		for(int y=-settings.mapGridDimensions.y/2; y<=settings.mapGridDimensions.y/2; y++)
		{
			for(int x=-settings.mapGridDimensions.x/2; x<=settings.mapGridDimensions.x/2; x++)
			{
				if(mapExists(coord(x,y)))
				{
					//coord mapCenter = coord((3+(settings.mapGridDimensions.x/2))*settings.tileDimensions.x, (1+(settings.mapGridDimensions.y/2))*settings.tileDimensions.y);
					//coord mapHover = mapCenter+coord((x*settings.mapGridDimensions.x*2),(y*settings.mapGridDimensions.y*2));
					bool here = (coord(x,y) == minimapHover(viewerCursor));
					render.DrawQuickMap(app, registry.objMap[coord(x,y)], viewerCursor, coord(x,y), here);//isCollision(toVector(finemouse), sf::IntRect(mapHover.x, mapHover.y, settings.mapGridDimensions.x*2, settings.mapGridDimensions.y*2)));
				}
			}
		}
	}
	else
	{
		for(int i=1; i<int(registry.objMap[viewerCursor].regTiles.size()); i++)
		{
			if(registry.objMap[viewerCursor].regTiles[i] != NULL)
			{
				if(i==registry.objMap[viewerCursor].tileIndexOnGrid(player.deliverGridClick(app))
					&& registry.objMap[viewerCursor].entityIndexAtPoint(player.deliverRealClick(app))==0)
				{
					render.DrawTile(app, registry.objMap[viewerCursor].regTiles[i], registry.objMap[viewerCursor].regTiles[i]->pos, sf::Color::Red);
				}
				else render.DrawTile(app, registry.objMap[viewerCursor].regTiles[i], registry.objMap[viewerCursor].regTiles[i]->pos, registry.objMap[viewerCursor].regTiles[i]->distortionColor);
			}
		}
		for(int i=1; i<int(registry.objMap[viewerCursor].regEntities.size()); i++)
		{
			if(registry.objMap[viewerCursor].regEntities[i] != NULL && registry.objMap[viewerCursor].regEntities[i]->active)
			{
				coord pixel=coord(registry.objMap[viewerCursor].regEntities[i]->pos.x*32, registry.objMap[viewerCursor].regEntities[i]->pos.y*32);
				if(registry.objMap[viewerCursor].regEntities[i]->type==ICAT_CREATURE)
				{
					pixel=pixel+(registry.objMap[viewerCursor].regCreature[registry.objMap[viewerCursor].regEntities[i]->packIndex]->offset);
				}
				render.DrawEntity(app, registry.objMap[viewerCursor].regEntities[i], pixel, (i==registry.objMap[viewerCursor].entityIndexAtPoint(player.deliverRealClick(app))));
			}
		}

	}

	render.DrawGUIForm(app, tmp, player.ritualForm, player.deliverRealClick(app));
	render.DrawInventory(app, tmp, player.inv, player.inventoryForm);
	render.DrawGUIForm(app, tmp, player.sideMenuForm, player.deliverRealClick(app));
	render.DrawGUIForm(app, tmp, player.creatureCard, player.deliverRealClick(app));
	app.draw(sidebar);
	app.display();
}

sf::String GameClass::outputEntity(int index)
{
	sf::String ret="";
	char buffer[64];
	int calc=0;
	if(registry.objMap[viewerCursor].regEntities[index] == NULL) return ret;

	ret+=tmp.container.entityList[registry.objMap[viewerCursor].regEntities[index]->entityTemplateIndex].name;
	ret+="\n";
	switch (registry.objMap[viewerCursor].regEntities[index]->type)
	{
	case ICAT_CREATURE: ret+="Creature\n"; ret+="HP: ";
		calc=registry.objMap[viewerCursor].regEntities[index]->packIndex;
		sprintf_s(buffer, "%i / %i", registry.objMap[viewerCursor].regCreature[calc]->currentHP,registry.objMap[viewerCursor].regCreature[calc]->maxHP);
		ret+=sf::String(buffer);
		break;
	case ICAT_DECORATION: ret+="Decoration\n"; break;
	case ICAT_VEGETATION: ret+="Vegetation\n"; 
		calc=registry.objMap[viewerCursor].regEntities[index]->packIndex;
		sprintf_s(buffer, "Growth: %i / %i", registry.objMap[viewerCursor].regVeg[calc]->currentGrowth+1,registry.objMap[viewerCursor].regVeg[calc]->maxGrowth);
		ret+=sf::String(buffer);
		break;
	case ICAT_INGREDIENT: ret+="Ingredient\n"; break;
	case ICAT_SEED: ret+="Seed\n"; break;
	case ICAT_SUMMON: ret+="Summon Charm\n"; 
		calc=registry.objMap[viewerCursor].regEntities[index]->packIndex;
		calc=registry.objMap[viewerCursor].regSummon[calc]->creatureContained;
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
	if(registry.objMap[viewerCursor].regTiles[index] == NULL) return ret;

	ret+=tmp.container.tileList[registry.objMap[viewerCursor].regTiles[index]->tileTemplateIndex].name;
	ret+="\n";
	return ret;
}

//loads the game setup information from 'settings.cfg' text file
//so far inefficient and sloppy code
bool GameClass::loadSettings()
{
	std::string buf;
	std::ifstream fin("settings.cfg", std::ios::in|std::ios::beg);
	if(fin.bad()) return false;
	fin >> settings.tileDimensions.x;
	fin >> settings.tileDimensions.y;
	fin >> settings.mapGridDimensions.x;
	fin >> settings.mapGridDimensions.y;
	fin >> settings.screenDimensions.x;
	fin >> settings.screenDimensions.y;
	fin.get();
	std::getline(fin, buf);
	strncpy_s(settings.verTitle,buf.c_str(), 40);
	std::getline(fin, buf);
	strncpy_s(settings.verTx,buf.c_str(), 40);
	std::getline(fin, buf);
	strncpy_s(settings.saveFile,buf.c_str(), 40);
	std::getline(fin, buf);
	strncpy_s(settings.templateDataFile,buf.c_str(), 40);
	std::getline(fin, buf);
	strncpy_s(settings.debugFile,buf.c_str(), 40);
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
	return true;
}