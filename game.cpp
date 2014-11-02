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
	//registry.clear(map_pos);
}

//contructor, sets default values to member variables, opens debug log file
//then creates the 'app' window
GameClass::GameClass()
{
	dumpActionList=false;
	generatorCursor=coord(0,0);
	viewerCursor=coord(0,0);
	startTime=0;//int(unsigned long(time(NULL))-unsigned long(JAN1_2014));
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
	player.initRecipes(tmp);
	//render.mainfont.loadFromFile(settings.mainFontFile);
	sidebar = sf::Text("Sidebar Line 1\nLine 2\nLine 3\nLine 4\nLast Line", render.mainfont, 20);
	sidebar.setPosition(float(settings.mapGridDimensions.x*settings.tileDimensions.x)+10.0f, 10.0f);
	biomes.clear(); biomes.push_back(BiomeClass());
	actions.fillSourceAction(tmp, "generatemap", 0, gameTime());
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
//	pollKeys();
	pollWindowsEvents();
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

int GameClass::getButtonIndexAtPoint(coord map_pos, coord _pixel)
{
	int ret=0;
	for(int i=int(registry.objMap[map_pos].buttons.size())-1; i>0; i--)
	{
		ret=registry.objMap[map_pos].buttons[i];
		if(isCollision(toVector(_pixel), sf::IntRect(ether.regButtons[ret]->box)))
			return ret;
	}
	return ret;
}


int GameClass::translateClickLayer(coord _pixel)
{
	//input is the pixel of the current screen being clicked on by the player engine
	//(pass player.deliverRealClick(app)
	//returns a predefined code for the map layer to handle

	//start at the very top layer, which at the moment is only used for minimap
	if(player.gamemode==GAMEMODE_MINIMAP) return CLICKLAYER_TOP;

	//then test button click AND BUTTON ACTIVE STATE
	if(registry.objMap[viewerCursor].buttonIndexAtPoint(ether, _pixel)>0)
	{
		int plc=registry.objMap[viewerCursor].buttonIndexAtPoint(ether, _pixel);
		if(ether.regButtons[plc]->active && ether.regButtons[plc]->actionTemplateIndex>0)
			return CLICKLAYER_BUTTON;
	}
	//else if(player.inventoryForm.getBox().contains(_pixel.x, _pixel.y) && player.inventoryForm.active)
	//{
	//	//click on the inventory GUI but not on a button
	//	return CLICKLAYER_GUIFORM;
	//}
	//else if(player.ritualForm.getBox().contains(_pixel.x, _pixel.y) && player.ritualForm.active)
	//{
	//	//click on the ritual GUI but not on a button
	//	return CLICKLAYER_GUIFORM;
	//}
	//else if(player.sideMenuForm.getBox().contains(_pixel.x, _pixel.y) && player.sideMenuForm.active)
	//{
	//	//click on the side menu GUI but not on a button
	//	return CLICKLAYER_GUIFORM;
	//}
	//else if(player.creatureCard.getBox().contains(_pixel.x, _pixel.y) && player.creatureCard.active)
	//{
	//	//click on the creature card GUI but not on a button
	//	return CLICKLAYER_GUIFORM;
	//}
	if(registry.objMap[viewerCursor].entityIndexAtPoint(ether, _pixel)>0)
	{
		int plc=registry.objMap[viewerCursor].entityIndexAtPoint(ether, _pixel);
		if(ether.regEntities[plc]->active && isEntityOnMap(plc))
		{
			switch(ether.regEntities[plc]->type)
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
	else if(registry.objMap[viewerCursor].tileIndexOnGrid(ether, coord(_pixel.x/settings.tileDimensions.x, _pixel.y/settings.tileDimensions.y))>0)
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
			plc=registry.objMap[viewerCursor].entityIndexAtPoint(ether, player.deliverRealClick(app));
			actions.fillEntityTargetAction(tmp, "selectentity", 0, plc, gameTime());
			break;
		case CLICKLAYER_DECORATIONS:
			plc=registry.objMap[viewerCursor].entityIndexAtPoint(ether, player.deliverRealClick(app));
			actions.fillEntityTargetAction(tmp, "selectentity", 0, plc, gameTime());
			break;
		case CLICKLAYER_BUTTON:
			plc=getButtonIndexAtPoint(viewerCursor, player.deliverRealClick(app));
			if(ether.regButtons[plc]->actionTemplateIndex>0 && ether.regButtons[plc]->active)
			{
				for(int i=1; i<int(tmp.container.actionList.size()); i++)
				{
					if(ether.regButtons[plc]->actionTemplateIndex == i)
					{
						actions.fillSourceAction(tmp, tmp.container.actionList[i].cname, ether.regButtons[plc]->linkedEntityIndex, gameTime());
						return;
					}
				}
			}
			break;
		case CLICKLAYER_TILE:
			plc=registry.objMap[viewerCursor].tileIndexOnGrid(ether, player.deliverGridClick(app));
//			if(player.inv.getItemAtCursor()>0 && player.gamemode==GAMEMODE_INVENTORY)
//			{
//				if(ether.regEntities[player.inv.getItemAtCursor()]->type==ICAT_TOOL)
//				{
//					actions.useTool(tmp, ether, player.inv.getItemAtCursor(), 0, plc, gameTime());
//					return;
//				}
//				if(ether.regEntities[player.inv.getItemAtCursor()]->type==ICAT_SEED)
//				{
//					actions.plantSeed(tmp, ether, player.inv.drop(player.inv.cursor), 0, plc, gameTime());
//					return;
//				}
//				if(ether.regEntities[player.inv.getItemAtCursor()]->type==ICAT_INGREDIENT)
//				{
//					player.inv.drop(player.inv.cursor);
//					return;
//				}
//				if(ether.regEntities[player.inv.getItemAtCursor()]->type==ICAT_SUMMON)
//				{
////					actions.useCharm(tmp, ether, player.inv.getItemAtCursor(), 0, plc, gameTime());
////					sidebar.setString(outputEntity(player.inv.getItemAtCursor()));
//					return;
//				}
//			}
//			else
			actions.fillTileTargetAction(tmp, "selecttile", 0, plc, gameTime());
			break;
		case CLICKLAYER_TOP:
			handleMinimapClick();
			break;
	}
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
	int thisTick=int(actions.actionQueue.size());
	processActionList(thisTick, actSeconds);
	processTextList(actSeconds);
}

bool GameClass::validateTilePosition(coord pos)
{
	if(pos.x>settings.mapGridDimensions.x || pos.y>settings.mapGridDimensions.y || pos.x<0 || pos.y<0) return false;
	//if(registry.objMap[viewerCursor].numberOfTilesOnGrid(pos)<1) return false;
	return true;
}

bool GameClass::isEntityOnMap(int entityIndex)
{
	for(int i=1; i<int(registry.objMap[viewerCursor].entities.size()); i++)
	{
		if(registry.objMap[viewerCursor].entities[i]==entityIndex)
			return true;
	}
	return false;
}

bool GameClass::validateEntity(int entityIndex)
{
	if(entityIndex<1 || entityIndex>numberOfEntities()) return false;
	if(!ether.regEntities[entityIndex]->active) return false;
	if(!isEntityOnMap(entityIndex)) return false;
	return true;
}

bool GameClass::isCreature(int entityIndex)
{
	return (ether.regEntities[entityIndex]->type==ICAT_CREATURE);
}

coord GameClass::getLatestTargetPosition(int entityIndex)
{
	int latestTile=0;
	int latestEnt=0;
	float tiletime=0.0f;
	float enttime=0.0f;
	for(int i=1; i<int(actions.actionQueue.size()); i++)
	{
		if(actions.actionQueue[i]->entityIndexSource==entityIndex)
		{
			float time=actions.actionQueue[i]->timeToActivate;
			if(actions.actionCodeEquals(tmp, actions.actionQueue[i]->actionTemplateIndex, "movestep") || actions.actionCodeEquals(tmp, actions.actionQueue[i]->actionTemplateIndex, "establishtarget"))
			{
				int tile=actions.actionQueue[i]->tileIndexTarget;
				int ent=actions.actionQueue[i]->entityIndexTarget;
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
	if(enttime==tiletime && tiletime==0.0f) return ether.regEntities[entityIndex]->pos;
	if(enttime>tiletime)
	{
		//entity position is latest
		return ether.regEntities[latestEnt]->pos;
	}
	else
	{
		return ether.regTiles[latestTile]->pos;
	}
	return coord(0,0);
}

void GameClass::commitSmartPathing(int entityIndex, coord pos, const actionStruct* act)
{
	coord tTarget=smartPathing(entityIndex, pos);
	if(!commitMovement(entityIndex, tTarget, act))
	{
		actions.fillSourceAction(tmp, "idle", entityIndex, gameTime());
	}
	return;

}

coord GameClass::smartPathing(int entityIndex, coord pos)
{
	coord start=ether.regEntities[entityIndex]->pos;
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
	ether.regEntities[entityIndex]->pos=_pos;
	ether.regEntities[entityIndex]->box.left=(_pos.x*32);
	ether.regEntities[entityIndex]->box.top=(_pos.y*32);

	if(ether.regEntities[entityIndex]->type != ICAT_CREATURE) return;
	ether.regCreature[ether.regEntities[entityIndex]->packIndex]->offset=coord(0,0);
	ether.regCreature[ether.regEntities[entityIndex]->packIndex]->velocity=coord(0,0);
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
	creaturePack* creature = ether.regCreature[ether.regEntities[entityIndex]->packIndex];
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
	creaturePack* creature = ether.regCreature[ether.regEntities[entityIndex]->packIndex];
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
	int pack = ether.regEntities[entityIndex]->packIndex;
	int xvel = direction.x*ether.regCreature[pack]->move;
	int yvel = direction.y*ether.regCreature[pack]->move;
	if(xvel==0 && yvel==0) return;
	if(xvel!=0 && yvel!=0) return;
	ether.regCreature[pack]->velocity=coord(xvel, yvel);

}

bool GameClass::commitMovement(int entityIndex, coord tTarget, const actionStruct* act)
{
	//if we cannot validate movement, fail immediately
	//if(!(validateTilePosition(tTarget) && validateEntity(entityIndex) && isCreature(entityIndex))) return false;
	sidebar.setString("Passed Validation");
	//determine the cardinal direction of the next tile
	coord creaturePosition = ether.regEntities[entityIndex]->pos;
	int dx = tTarget.x - creaturePosition.x;
	int dy = tTarget.y - creaturePosition.y;
	if(dx==0 && dy==0) return false; //if the creature is at the target, there is no movement to commit to
	if(dx!=0 && dy!=0) return false; //if there are multiple tiles between the creature and target, this fails too
	//now that that is out of the way, we determine the direction of movement
	if(dy>0) // move south
	{
		//commit to a southern movement
		if(actions.entityTarget(ether, act))
			actions.fillEntityTargetAction(tmp, "movesouth", entityIndex, act->entityIndexTarget, gameTime());
		else
			actions.fillTileTargetAction(tmp, "movesouth", entityIndex, act->tileIndexTarget, gameTime());
		return true;
	}
	if(dy<0) //move north
	{
		//commit to a northern movement
		if(actions.entityTarget(ether, act))
			actions.fillEntityTargetAction(tmp, "movenorth", entityIndex, act->entityIndexTarget, gameTime());
		else
			actions.fillTileTargetAction(tmp, "movenorth", entityIndex, act->tileIndexTarget, gameTime());
		return true;
	}
	if(dx>0) // move east
	{
		//commit to a eastern movement
		if(actions.entityTarget(ether, act))
			actions.fillEntityTargetAction(tmp, "moveeast", entityIndex, act->entityIndexTarget, gameTime());
		else
			actions.fillTileTargetAction(tmp, "moveeast", entityIndex, act->tileIndexTarget, gameTime());
		return true;
	}
	if(dx<0) //move west
	{
		//commit to a western movement
		if(actions.entityTarget(ether, act))
			actions.fillEntityTargetAction(tmp, "movewest", entityIndex, act->entityIndexTarget, gameTime());
		else
			actions.fillTileTargetAction(tmp, "movewest", entityIndex, act->tileIndexTarget, gameTime());
		return true;
	}
	return false; //if we made it this far, something went wrong, fail the commitment! :D

}

//fails if in movement, true if aligned with grid
//this carries the action for forced movement/velocity
bool GameClass::moveToAlign(int entityIndex)
{
	registeredEntity* ent = ether.regEntities[entityIndex];
	creaturePack* pack = ether.regCreature[ent->packIndex];
	coord off=pack->offset;
	coord vel=pack->velocity;
	coord pos=ether.regEntities[entityIndex]->pos;

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
	ether.regCreature[ent->packIndex]->offset=off;
	return false;
}

int GameClass::getEnemyNeighbor(int entityIndex)
{
	coord pos=ether.regEntities[entityIndex]->pos;
	for(int i=1; i<numberOfEntities(); i++)
	{
		if(ether.regEntities[registry.objMap[viewerCursor].entities[i]]->active)
		{
			if(ether.regEntities[registry.objMap[viewerCursor].entities[i]]->isEnemy)
			{
				if(calcDist(toVector(pos), toVector(ether.regEntities[registry.objMap[viewerCursor].entities[i]]->pos))<1.4f)
				{
					return registry.objMap[viewerCursor].entities[i];
				}
			}
		}
	}
	return 0;
}

int GameClass::getEnemyCount(coord map_pos)
{
	int ret=0;
	for(int i=1; i<int(registry.objMap[map_pos].entities.size()); i++)
	{
		if(ether.regEntities[registry.objMap[map_pos].entities[i]]->isEnemy)
		{
			ret++;
		}
	}
	return ret;
}
int GameClass::getFriendlyNeighbor(int entityIndex)
{
	coord pos=ether.regEntities[entityIndex]->pos;
	for(int i=1; i<numberOfEntities(); i++)
	{
		if(ether.regEntities[registry.objMap[viewerCursor].entities[i]]->active)
		{
			if(!ether.regEntities[registry.objMap[viewerCursor].entities[i]]->isEnemy && 
				(ether.regEntities[registry.objMap[viewerCursor].entities[i]]->type==ICAT_CREATURE ||
				ether.regEntities[registry.objMap[viewerCursor].entities[i]]->type==ICAT_DECORATION)
				)
			{
				if(calcDist(toVector(pos), toVector(ether.regEntities[registry.objMap[viewerCursor].entities[i]]->pos))<1.4f)
				{
						if(ether.regEntities[registry.objMap[viewerCursor].entities[i]]->entityTemplateIndex != getEntityTemplateIndex("magiceffect"))
						{
							return registry.objMap[viewerCursor].entities[i];
						}
				}
			}
		}
	}
	return 0;
}

int GameClass::getClosestFriendlyRange(int entityIndex, float agroRange)
{
	float lowestDistance=agroRange+1.0f;
	float testDistance=0.0f;
	int ret=0;
	coord pos=ether.regEntities[entityIndex]->pos;
	for(int i=1; i<numberOfEntities(); i++)
	{
		if(ether.regEntities[registry.objMap[viewerCursor].entities[i]] != NULL && ether.regEntities[registry.objMap[viewerCursor].entities[i]]->active)
		{
			if(!ether.regEntities[registry.objMap[viewerCursor].entities[i]]->isEnemy &&
				(ether.regEntities[registry.objMap[viewerCursor].entities[i]]->type==ICAT_CREATURE ||
				ether.regEntities[registry.objMap[viewerCursor].entities[i]]->type==ICAT_DECORATION)
				)
			{
				testDistance=calcDist(toVector(pos), toVector(ether.regEntities[registry.objMap[viewerCursor].entities[i]]->pos));
				if(testDistance<agroRange)
				{
					if(testDistance<lowestDistance)
					{
						if(ether.regEntities[registry.objMap[viewerCursor].entities[i]]->entityTemplateIndex != getEntityTemplateIndex("magiceffect"))
						{
							ret=registry.objMap[viewerCursor].entities[i];
							lowestDistance=testDistance;
						}
					}
				}
			}
		}
	}
	if(ret==0)
	{
		for(int i=1; i<numberOfEntities(); i++)
		{
			if(ether.regEntities[registry.objMap[viewerCursor].entities[i]] != NULL && ether.regEntities[registry.objMap[viewerCursor].entities[i]]->active)
			{
				if(ether.regEntities[registry.objMap[viewerCursor].entities[i]]->entityTemplateIndex==getEntityTemplateIndex("ritualstump"))
				{
					ret=registry.objMap[viewerCursor].entities[i];
				}
			}
		}
	}
	return ret;
}

bool GameClass::isReachedEntityTarget(int entityIndex, int entityTarget)
{
	coord pos=ether.regEntities[entityIndex]->pos;
	coord trg=ether.regEntities[entityTarget]->pos;
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

bool GameClass::isFriendlyNeighbor(int entityIndex)
{
	return (getFriendlyNeighbor(entityIndex)!=0);
}

bool GameClass::isFriendlyRange(int entityIndex, float agroRange)
{
	return (getClosestFriendlyRange(entityIndex, agroRange)!=0);
}

bool GameClass::mapExists(coord map_pos)
{
	return !(registry.objMap.find(map_pos) == registry.objMap.end());
}

bool GameClass::mapComplete(coord map_pos)
{
	return (mapExists(map_pos) && registry.objMap[map_pos].generated);
}

void GameClass::capture()
{
	sf::Image shot(app.capture());
	sf::Texture shrink;
	shrink.create(settings.mapGridDimensions.x*settings.tileDimensions.x, settings.mapGridDimensions.y*settings.tileDimensions.y);
	shrink.loadFromImage(shot, sf::IntRect(0,0,shrink.getSize().x,shrink.getSize().y));
	sf::Image screen=shrink.copyToImage();
	char buffer[65];
	sprintf_s(buffer, 65, "screenshots/%i.png", int(gameTime()));
	screen.saveToFile(buffer);
	sidebar.setString(sf::String("Saved screen to:\n"+sf::String(buffer))+"\n");
}

void GameClass::generateMapName(coord map_pos)
{
	std::string line1("");
	std::string line2("");
	char ret[40];
	int lines=0;
	std::ifstream fin1("front.txt", std::ios::in|std::ios::beg);
	while(!fin1.eof())
	{
		std::getline(fin1, line1);
		lines++;
	}
	fin1.close();
	fin1.open("front.txt", std::ios::in|std::ios::beg);
	int skips=rand()%lines;
	for(int i=0; i<skips; i++)
	{
		std::getline(fin1, line1);
	}
	fin1.close();
	lines=0;
	std::ifstream fin2("back.txt", std::ios::in|std::ios::beg);
	while(!fin2.eof())
	{
		std::getline(fin2, line2);
		lines++;
	}
	fin2.close();
	fin2.open("back.txt", std::ios::in|std::ios::beg);
	skips=rand()%lines;
	for(int i=0; i<skips; i++)
	{
		std::getline(fin2, line2);
	}
	fin2.close();
	sprintf_s(ret, "%s %s (%s)", line1.c_str(), line2.c_str(), getBiomeData(map_pos).name);
	strcpy_s(ether.regMaps[registry.objMap[map_pos].mapgenerator]->showName, ret);
}

char* GameClass::getMapName(coord map_pos)
{
	if(ether.regMaps[registry.objMap[map_pos].mapgenerator] != NULL)
	{
		return ether.regMaps[registry.objMap[map_pos].mapgenerator]->showName;
	}
	return NULL;
}

bool GameClass::biomeExists(coord map_pos)
{
	for(int i=1; i<int(biomes.size()); i++)
	{
		for(int j=0; j<int(biomes[i].points.size()); j++)
		{
			if(biomes[i].points[j]==map_pos) return true;
		}
	}
	return false;
}

BiomeClass GameClass::getBiomeData(coord map_pos)
{
	if(!biomeExists(map_pos)) return BiomeClass();
	for(int i=1; i<int(biomes.size()); i++)
	{
		for(int j=0; j<int(biomes[i].points.size()); j++)
		{
			if(biomes[i].points[j]==map_pos) return biomes[i];
		}
	}
	return BiomeClass();
}

void GameClass::biomeGeneration(coord map_pos)
{
	//generates a biome in the 'biomes' list, starting from the map_pos map
	//if there is already a biome here, we fail the whole thing (?)
	if(biomeExists(map_pos)) return;

	//make map_pos the starting point
	coord starting=map_pos;
	coord step=starting;

	//randomly select a biome type
	int biomesize=int(tmp.container.biomeList.size()-1);
	int biometype=rand()%biomesize+1;
	char buf[40]="";
	strcpy_s(buf, tmp.container.biomeList[biometype].cname);

	biomes.push_back(BiomeClass());
	int cc=int(biomes.size()-1);
	strcpy_s(biomes[cc].name, buf);

	//now that we have selected a biome type, we must 'drunk walk' a cluster
	//reaching the 'maxVolume' and not overlapping or going beyond the
	//'maxReach' distance from the center
	int volume=int(biomes[cc].points.size());
	int tries=0;
	coord peekDir=coord(0,0);
	while(volume<=biomes[cc].maxVolume && tries<=50)
	{
		tries++;
		//drunk walk step, choose a direction at the tail of the loop
		step=step+peekDir;
		int r=rand()%4;
		if(!mapExists(step))
		{
			registry.objMap.insert(std::pair<coord, GameObjectContainerClass>(step, GameObjectContainerClass()));
		}
		if(!biomeExists(step))
		{
			registry.objMap[step].biomeIndex=cc;
			biomes[cc].addPoint(step);
			volume++;
		}
		switch(r)
		{
			case 0://n
				peekDir=coord(0, -1);
				break;
			case 1://e
				peekDir=coord(1, 0);
				break;
			case 2://s
				peekDir=coord(0, 1);
				break;
			default://w
				peekDir=coord(-1, 0);
				break;
		}
	}

}

void GameClass::experimentalWorldGen(coord map_pos)
{
	//find out what biome index we are on
	//if index is 0 or the map hasn't been generated
	//then we need to start the biome generation at that point
	for(int y=-1; y<=1; y++)
	{
		for(int x=-1; x<=1; x++)
		{
			if(abs(x)!=abs(y) || coord(x,y)==coord(0,0))
			{
				generatorCursor=map_pos+coord(x,y);
				biomeGeneration(generatorCursor);
				experimentalMapGen(generatorCursor, biomes[registry.objMap[generatorCursor].biomeIndex].name);
			}
		}
	}

}

void GameClass::experimentalMapGen(coord map_pos, const char* biome)
{
	bool hostile=false;
	if(!registry.objMap[map_pos].generated)
	{
		header.mapIndex++;
		header.randSeed=rand()+header.mapIndex;
		initRandom(header.randSeed);
		refreshMap(map_pos);
		registry.clear(map_pos);
		//registry.createMapTerrain(tmp, biome, generatorCursor);
		registry.objMap[map_pos].mapgenerator=ether.createMapTerrain(tmp, biome); //registry.objMap[map_pos].regMaps.size()-1;
		if(registry.objMap[map_pos].mapgenerator == 0) return;
		generateMapName(map_pos);
		createBaseMapLayer(ether.regMaps[registry.objMap[map_pos].mapgenerator]);
		hostile=createDecorationLayer(ether.regMaps[registry.objMap[map_pos].mapgenerator]);
		createEcologyLayer(ether.regMaps[registry.objMap[map_pos].mapgenerator]);
		registry.objMap[map_pos].mapMode=ether.regMaps[registry.objMap[map_pos].mapgenerator]->mapMode;
		registry.objMap[map_pos].generated=true;
	}

	//some ideas
	//tempImg.create(settings.mapGridDimensions.x, settings.mapGridDimensions.y, sf::Color::White);
	if(getMapMode(map_pos)==MAPMODE_FRESH)
	{
		tempImg.create(settings.mapGridDimensions.x, settings.mapGridDimensions.y);// = registry.objMap[map_pos].mapSheet.copyToImage();
		for(int i=1; i<int(registry.objMap[map_pos].tiles.size()); i++)
		{
			if(registry.objMap[map_pos].tiles[i] > 0)
			{
				coord pos=ether.regTiles[registry.objMap[map_pos].tiles[i]]->pos;
				tempImg.setPixel(pos.x, pos.y, sf::Color(
					ether.regTiles[registry.objMap[map_pos].tiles[i]]->distortionColor.g,
					ether.regTiles[registry.objMap[map_pos].tiles[i]]->distortionColor.g,
					ether.regTiles[registry.objMap[map_pos].tiles[i]]->distortionColor.g,
					90));
			}
		}
		registry.objMap[map_pos].mapSheet.loadFromImage(tempImg);
		//if(hostile)
		//{
			setMapMode(map_pos, MAPMODE_THREATEN);
		//}
		//else
		//{
		//	setMapMode(map_pos, MAPMODE_SANCTUARY);
		//}
	}
	fillPathingRoutes();
}

void GameClass::fillPathingRoutes()
{
	//establish pathing rules
	for(int y=0; y<settings.mapGridDimensions.y; y++)
	{
		for(int x=0; x<settings.mapGridDimensions.x; x++)
		{
			terrain.setTerrainRuleAt(coord(x,y),0);
		}
	}
	for(int i=1; i<int(registry.objMap[generatorCursor].entities.size()); i++)
	{
		terrain.setTerrainRuleAt(ether.regEntities[registry.objMap[generatorCursor].entities[i]]->pos, 4);
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

bool GameClass::createDecorationLayer(const mapGenStruct* map)
{
	for(int j=0; j<int(map->decoLayer.size()); j++)
		scatterEntity(map->decoLayer[j]);
	if(map->mapMode==MAPMODE_FRESH)
	{
		//testing make it all hostile
		bool hostile=true;//(rand()%3==0);
		if(hostile)
		{
			addEntity("spawnhole", generatorCursor, coord((rand()%(settings.mapGridDimensions.x-2))+1,(rand()%(settings.mapGridDimensions.y-2))+1));
		}
		addEntity("ritualstump", generatorCursor, coord((rand()%(settings.mapGridDimensions.x-2))+1,(rand()%(settings.mapGridDimensions.y-2))+1));
		return hostile;
	}
	return (map->mapMode==MAPMODE_THREATEN);
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
				if(processConic(coord(int(xt),int(yt)), A, B, C, D, E, F)) addTile(codename, generatorCursor, _tl+coord(x,y));
			}
			if(strcmp(shapename, "circle")==0)
			{
				A=1.0f;B=0.0f;C=1.0f;
				D=float((-2)*focus.x);E=float((-2)*focus.y);F=float(pow(focus.x,2)+pow(focus.y,2)-pow(radius,2));
				if(processConic(coord(int(xt),int(yt)), A, B, C, D, E, F)) addTile(codename, generatorCursor, _tl+coord(x,y));
			}
			if(strcmp(shapename, "smallcircle")==0)
			{
				int smallradius=radius/6;
				A=1.0f;B=0.0f;C=1.0f;
				D=float((-2)*focus.x);E=float((-2)*focus.y);F=float(pow(focus.x,2)+pow(focus.y,2)-pow(smallradius,2));
				if(processConic(coord(int(xt),int(yt)), A, B, C, D, E, F)) addTile(codename, generatorCursor, _tl+coord(x,y));
			}
			if(strcmp(shapename, "conic")==0)
			{
				A=float(1.0f/float(pow(vertex.x, 2)));
				B=0.0f;
				C=float(-1.0f/float((pow(focus.x,2)-pow(vertex.x,2))));
				D=float(-2.0f*float(origin.x))/float(pow(vertex.x, 2));
				E=float(2.0f*float(origin.y))/float(pow(focus.x,2)-pow(vertex.x,2));
				F=float(pow(origin.x, 2)/pow(vertex.x, 2))-float(pow(origin.y, 2)/(pow(focus.x,2)-pow(vertex.x,2)))+1.0f;
				if(processConic(coord(int(xt),int(yt)), A, B, C, D, E, F)) addTile(codename, generatorCursor, _tl+coord(x,y));
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
				if(processConic(coord(int(xt),int(yt)), A, B, C, D, E, F)) addTile(codename, generatorCursor, _tl+coord(x,y));
			}
			if(strcmp(shapename, "triangle")==0)
			{
				if(adjY>=x)
				{
					addTile(codename, generatorCursor, _tl+coord(x,y));
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
			if(registry.objMap[generatorCursor].entityIndexOnGrid(ether, coord(x,y), 1)<1 &&
				rand()%100<spread->density &&
				ether.regTiles[registry.objMap[generatorCursor].tileIndexOnGrid(ether, coord(x,y))]->tileTemplateIndex != getTileTemplateIndex("water"))
			{
				addEntity(tmp.container.entityList[spread->entityTemplateIndex].cname, generatorCursor, coord(x,y));
			}
		}
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
		addTile(codename, generatorCursor, current);
	}
}



void GameClass::fillInventory()
{
	player.inventoryForm.clear();
	for(int j=0; j<25; j++)
		player.inventoryForm.addCell(RENDER_BUTTON, getGuiTemplateIndex("inventorycell"), gridToPixel(coord(j%5, int(j/5))));
	for(int i=0; i<int(player.inv.cellList.size()); i++)
	{
		if(player.inv.cellList[i].tmp_idx>0)
		{
			player.inventoryForm.addCell(RENDER_ENTITY, player.inv.cellList[i].tmp_idx, gridToPixel(coord(i%5, int(i/5))));
		}
	}
	fillGuiForm(player.inventoryForm);
	player.inventoryForm.activate();
}

void GameClass::fillGuiForm(GUIFormClass& form, int linked, bool active)
{
	for(int i=1; i<int(form.cells.size()); i++)
	{
		if(form.cells[i].renderType == RENDER_BUTTON)
		{
			if(form.cells[i].templateIndex==getGuiTemplateIndex("resultritualcell")) sidebar.setString("Ritual Cell\nFound In Registry");
			if(form.cells[i].buttonIndex > 0)
			{
				//ether.regButtons[form.cells[i].buttonIndex]->linkedEntityIndex=linked;
				ether.regButtons[form.cells[i].buttonIndex]->active=true;
				registry.objMap[viewerCursor].buttons.push_back(form.cells[i].buttonIndex);
			}
			else
			{
				form.cells[i].buttonIndex = addButton(tmp.container.buttonList[form.cells[i].templateIndex].cname, viewerCursor, form.cells[i].pixel+form.tl, linked);
				//ether.regButtons[form.cells[i].buttonIndex]->linkedEntityIndex=linked;
				activateButton(form.cells[i].buttonIndex);
				registry.objMap[viewerCursor].buttons.push_back(form.cells[i].buttonIndex);
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
			ether.regButtons[form.cells[i].buttonIndex]->active=false;
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
	return int(registry.objMap[viewerCursor].tiles.size());
}

int GameClass::numberOfEntities()
{
	return int(registry.objMap[viewerCursor].entities.size());
}

void GameClass::removeEntity(int etherIndex, coord map_pos)
{
	for(int i=0; i<8; i++)
		actions.stopActionCategory(etherIndex, i);
	registry.objMap[map_pos].eraseEntity(etherIndex);
	fillPathingRoutes();
}

void GameClass::deleteEtherEntity(int etherIndex, coord map_pos)
{
	removeEntity(etherIndex, map_pos);
}

int GameClass::getTileIndexAt(coord pos)
{
	return registry.objMap[viewerCursor].tileIndexOnGrid(ether, pos);
}

void GameClass::handleMinimapClick()
{
	coord mapgrid=minimapHover(viewerCursor);
	if(mapComplete(mapgrid))
	{
		//refreshMap(viewerCursor);
		viewerCursor=mapgrid;
		generatorCursor=mapgrid;
		dumpActionList=true;
		player.gamemode=GAMEMODE_NEUTRAL;
		mapscale=1.0f;
		render.resetView(app);
		render.viewport.zoom(mapscale);
		actions.fillSourceAction(tmp, "generatemap", 0, gameTime());
		//refreshMap(mapgrid);

	}

}

void GameClass::handleGUIClick(GUIFormClass& form)
{
	int index=registry.objMap[viewerCursor].buttonIndexAtPoint(ether, player.deliverRealClick(app));
	if(index==0 || !ether.regButtons[index]->active) return;
	if(ether.regButtons[index]->actionTemplateIndex>0)
	{
		for(int i=1; i<int(tmp.container.actionList.size()); i++)
		{
			if(ether.regButtons[index]->actionTemplateIndex == i)
			{
				actions.fillSourceAction(tmp, tmp.container.actionList[i].cname, ether.regButtons[index]->linkedEntityIndex, gameTime());
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
				if(mapComplete(coord(x,y)))
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
		for(int i=1; i<int(registry.objMap[viewerCursor].tiles.size()); i++)
		{
			if(registry.objMap[viewerCursor].tiles[i] != 0)
			{
				int reg=registry.objMap[viewerCursor].tiles[i];
				if(i==registry.objMap[viewerCursor].tileIndexOnGrid(ether, player.deliverGridClick(app))
					&& registry.objMap[viewerCursor].entityIndexAtPoint(ether, player.deliverRealClick(app))==0)
				{
					render.DrawTile(app, ether.regTiles[reg], ether.regTiles[reg]->pos, sf::Color::Red);
				}
				else render.DrawTile(app, ether.regTiles[reg], ether.regTiles[reg]->pos, ether.regTiles[reg]->distortionColor);
			}
		}

		//one drawing pass to render the entities solid
		for(int y=0; y<settings.mapGridDimensions.y; y++)
		{
			for(int x=0; x<settings.mapGridDimensions.x; x++)
			{
				int low=0;
				int reg=0;
				do
				{
					reg=registry.objMap[viewerCursor].entityIndexOnGrid(ether, coord(x,y), low);
					low=reg;
					if(reg>0 && ether.regEntities[reg] != NULL && ether.regEntities[reg]->active && isEntityOnMap(reg))
					{
						coord pixel=gridToPixel(ether.regEntities[reg]->pos);
						pixel=pixel - ether.regEntities[reg]->cog + coord(settings.tileDimensions.x/2, settings.tileDimensions.y);
						if(ether.regEntities[reg]->type==ICAT_CREATURE)
						{
							pixel=pixel+(ether.regCreature[ether.regEntities[reg]->packIndex]->offset);
						}
						render.DrawEntity(app, ether.regEntities[reg], pixel, (reg==registry.objMap[viewerCursor].entityIndexAtPoint(ether, player.deliverRealClick(app))), false);
					}
				} while(reg>0);
			}
		}

		//reversed drawing pass to render a 'ghost' of the entities in case they are behind another entity, like a tree
		for(int y=settings.mapGridDimensions.y-1; y>=0; y--)
		{
			for(int x=settings.mapGridDimensions.x-1; x>=0; x--)
			{
				int low=0;
				int reg=0;
				do
				{
					reg=registry.objMap[viewerCursor].entityIndexOnGrid(ether, coord(x,y), low);
					low=reg;
					if(reg>0 && ether.regEntities[reg] != NULL && ether.regEntities[reg]->active && isEntityOnMap(reg))
					{
						coord pixel=gridToPixel(ether.regEntities[reg]->pos);
						pixel=pixel - ether.regEntities[reg]->cog + coord(settings.tileDimensions.x/2, settings.tileDimensions.y);
						if(ether.regEntities[reg]->type==ICAT_CREATURE)
						{
							pixel=pixel+(ether.regCreature[ether.regEntities[reg]->packIndex]->offset);
						}
						render.DrawEntity(app, ether.regEntities[reg], pixel, (reg==registry.objMap[viewerCursor].entityIndexAtPoint(ether, player.deliverRealClick(app))), true);
					}
				} while(reg>0);
			}
		}
		//then the special effects
		for(int i=1; i<numberOfEntities(); i++)
		{
			int reg=registry.objMap[viewerCursor].entities[i];
			coord pixel=gridToPixel(ether.regEntities[reg]->pos);
			pixel=pixel - ether.regEntities[reg]->cog + coord(settings.tileDimensions.x/2, settings.tileDimensions.y);
			if(ether.regEntities[reg]->type==ICAT_CREATURE)
			{
				pixel=pixel+(ether.regCreature[ether.regEntities[reg]->packIndex]->offset);
			}
			if(ether.regEntities[reg]->entityTemplateIndex==getEntityTemplateIndex("magiceffect"))
				render.DrawEntity(app, ether.regEntities[reg], pixel, (reg==registry.objMap[viewerCursor].entityIndexAtPoint(ether, player.deliverRealClick(app))), false);
			if(ether.regEntities[reg]->entityTemplateIndex==getEntityTemplateIndex("claw"))
				render.DrawEntity(app, ether.regEntities[reg], pixel, (reg==registry.objMap[viewerCursor].entityIndexAtPoint(ether, player.deliverRealClick(app))), false);
		}

		if(registry.objMap[viewerCursor].mapMode==MAPMODE_FRESH)
		{
			sf::Image tempImg;
			tempImg.create(settings.mapGridDimensions.x, settings.mapGridDimensions.y, sf::Color(80,80,80,99));
			sf::Image screenCap = app.capture();
			for(int y=0; y<settings.mapGridDimensions.y; y++)
			{
				for(int x=0; x<settings.mapGridDimensions.x; x++)
				{
					tempImg.setPixel(x,y,screenCap.getPixel((x*settings.tileDimensions.x)+18, (y*settings.tileDimensions.y)+19));
					if(x==0 || y==0 || x==settings.mapGridDimensions.x-1 || y==settings.mapGridDimensions.y-1)
					{
						tempImg.setPixel(x,y,sf::Color(180, 180, 90, 80));
					}
				}
			}
			registry.objMap[viewerCursor].mapSheet.loadFromImage(tempImg);
		}

	}

	render.DrawGUIForm(app, tmp, player.ritualForm, player.deliverRealClick(app));
	render.DrawInventory(app, tmp, player.inv, player.inventoryForm);
	render.DrawGUIForm(app, tmp, player.sideMenuForm, player.deliverRealClick(app));
	render.DrawGUIForm(app, tmp, player.creatureCard, player.deliverRealClick(app));
	render.DrawStrings(app, registry.objMap[viewerCursor], ether);
	app.draw(sidebar);
	app.display();
}

sf::String GameClass::outputEntity(int index)
{
	sf::String ret="";
	char buffer[64];
	int calc=0;
	if(ether.regEntities[index] == NULL) return ret;

	ret+=tmp.container.entityList[ether.regEntities[index]->entityTemplateIndex].name;
	ret+="\n";
	switch (ether.regEntities[index]->type)
	{
	case ICAT_CREATURE: ret+="Creature\n"; ret+="HP: ";
		calc=ether.regEntities[index]->packIndex;
		sprintf_s(buffer, "%i / %i", ether.regCreature[calc]->currentHP,ether.regCreature[calc]->maxHP);
		ret+=sf::String(buffer);
		break;
	case ICAT_DECORATION: ret+="Decoration\n"; break;
	case ICAT_VEGETATION: ret+="Vegetation\n"; 
		calc=ether.regEntities[index]->packIndex;
		sprintf_s(buffer, "Growth: %i / %i", ether.regVeg[calc]->currentGrowth+1,ether.regVeg[calc]->maxGrowth);
		ret+=sf::String(buffer);
		break;
	case ICAT_INGREDIENT: ret+="Ingredient\n"; break;
	case ICAT_SEED: ret+="Seed\n"; break;
	case ICAT_SUMMON: ret+="Summon Charm\n"; 
		calc=ether.regEntities[index]->packIndex;
		calc=ether.regSummon[calc]->creatureContained;
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
	if(ether.regTiles[index] == NULL) return ret;

	ret+=tmp.container.tileList[ether.regTiles[index]->tileTemplateIndex].name;
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

bool GameClass::addTile(const char* _name, coord map_pos, coord grid_pos)
{
	int reg=ether.createTile(tmp, _name);
	if(reg==0) return false;
	registry.objMap[map_pos].tiles.push_back(reg);
	ether.regTiles[reg]->pos=grid_pos;
	ether.regTiles[reg]->distortionColor=registry.getTileDistortion(tmp.container.varianceList[tmp.container.tileList[ether.regTiles[reg]->tileTemplateIndex].variance], grid_pos, gameConstant, header.randSeed);
	ether.regTiles[reg]->minimapColor=ether.regTiles[reg]->distortionColor;
	return true;
}

int GameClass::addEntity(const char* _name, coord map_pos, coord _pos)
{
	int reg=ether.createEntity(tmp, _name);
	if(reg==0) return reg;
	registry.objMap[map_pos].entities.push_back(reg);
	ether.regEntities[reg]->pos=_pos;
	ether.regEntities[reg]->box.left+=(_pos.x*32);
	ether.regEntities[reg]->box.top+=(_pos.y*32);
	if(ether.regEntities[reg]->type==ICAT_VEGETATION)
	{
		ether.regVeg[ether.regEntities[reg]->packIndex]->bornTime=gameTime();
	}
	actions.fillSourceAction(tmp, tmp.container.entityList[ether.regEntities[reg]->entityTemplateIndex].creation, reg, gameTime());
	return reg;
}

int GameClass::addButton(const char* _name, coord map_pos, coord _pixel, int linked)
{
	int reg=ether.createButton(tmp, _name, linked);
	if(reg==0) return 0;
	ether.regButtons[reg]->box.left+=(_pixel.x);
	ether.regButtons[reg]->box.top+=(_pixel.y);
	ether.regButtons[reg]->linkedEntityIndex=linked;
	registry.objMap[map_pos].buttons.push_back(reg);
	return reg;
}

int GameClass::addText(const char* _string, coord map_pos, coord _pixel, sf::Color _color)
{
	int reg=ether.createString(_string, _color, render.auxfont, 255);
	if(reg==0) return 0;
	ether.regText[reg]->msg.setPosition(toVector(_pixel));
	registry.objMap[map_pos].strings.push_back(reg);
	return reg;
}

void GameClass::activateButton(int reg)
{
	ether.regButtons[reg]->active=true;
}

void GameClass::activateEntityButton(int entityReg)
{
	for(int i=1; i<int(ether.regButtons.size()); i++)
	{
		if(ether.regButtons[i]->linkedEntityIndex == entityReg)
			ether.regButtons[i]->active=true;
		//else ether.regButtons[i]->active=false;
	}
}

void GameClass::deactivateEntityButton(int entityReg)
{
	for(int i=1; i<int(ether.regButtons.size()); i++)
	{
		if(ether.regButtons[i]->linkedEntityIndex == entityReg && ether.regButtons[i]->active)
			ether.regButtons[i]->active=false;
	}
}

int GameClass::getEntityTemplateIndex(const char* _codename)
{
	return tmp.container.getEntityTemplate(_codename);
}

int GameClass::getActionTemplateIndex(const char* _codename)
{
	for(int i=1; i<int(tmp.container.actionList.size()); i++)
	{
		if(strcmp(tmp.container.actionList[i].cname, _codename)==0)
			return i;
	}
	return 0;
}

int GameClass::getTileTemplateIndex(const char* _codename)
{
	for(int i=1; i<int(tmp.container.tileList.size()); i++)
	{
		if(strcmp(tmp.container.tileList[i].cname, _codename)==0)
			return i;
	}
	return 0;
}
int GameClass::getGuiTemplateIndex(const char* _codename)
{

	for(int i=1; i<int(tmp.container.buttonList.size()); i++)
	{
		if(strcmp(tmp.container.buttonList[i].cname, _codename)==0)
			return i;
	}
	return 0;
}

int GameClass::getButtonForAction(const char* _codename)
{
	for(int i=1; i<int(tmp.container.buttonList.size()); i++)
	{
		if(strcmp(tmp.container.buttonList[i].actionName, _codename)==0)
			return i;
	}
	return 0;

}

int GameClass::getButtonLinkedEntity(const char* _codename)
{
	//in this process we look for a button index
	//codename is an action name, which is attached to a button template
	//the button index that we find will have a linked entity
	//we return the linked entity's index
	for(int i=1; i<int(registry.objMap[viewerCursor].buttons.size()); i++)
	{
		if(ether.regButtons[registry.objMap[viewerCursor].buttons[i]]->linkedEntityIndex>0)
		{
			int bt=ether.regButtons[registry.objMap[viewerCursor].buttons[i]]->actionTemplateIndex;
			for(int j=1; j<int(registry.objMap[viewerCursor].entities.size()); j++)
			{
				if(registry.objMap[viewerCursor].entities[j]==ether.regButtons[registry.objMap[viewerCursor].buttons[i]]->linkedEntityIndex
					&& strcmp(_codename, tmp.container.actionList[bt].cname)==0 && ether.regButtons[registry.objMap[viewerCursor].buttons[i]]->active)
					return registry.objMap[viewerCursor].entities[j];
			}
		}
	}
	return 0;
}

int GameClass::getListTemplateIndex(const char* _codename)
{
	for(int i=1; i<int(tmp.container.valuesList.size()); i++)
	{
		if(strcmp(tmp.container.valuesList[i].cname, _codename)==0)
			return i;
	}
	return 0;

}

int GameClass::getTerrainTemplateIndex(const char* _codename)
{
	for(int i=1; i<int(tmp.container.terrainList.size()); i++)
	{
		if(strcmp(tmp.container.terrainList[i].cname, _codename)==0)
			return i;
	}
	return 0;

}

int GameClass::getEcologyTemplateIndex(const char* _codename)
{
	for(int i=1; i<int(tmp.container.ecoList.size()); i++)
	{
		if(strcmp(tmp.container.ecoList[i].cname, _codename)==0)
			return i;
	}
	return 0;

}

int GameClass::getBiomeTemplateIndex(const char* _codename)
{
	for(int i=1; i<int(tmp.container.biomeList.size()); i++)
	{
		if(strcmp(tmp.container.biomeList[i].cname, _codename)==0)
			return i;
	}
	return 0;

}

void GameClass::processActionList(int maxlength, float actSeconds)
{
	if(dumpActionList)
	{
		dumpActionList=false;
		return;
	}
	if(player.gamemode==GAMEMODE_MINIMAP) return;
	for(int i=0; i<maxlength; i++)
	{
		bool workingOnMap=false;
		if(actions.actionQueue[i] != NULL && actions.actionQueue[i]->active)
		{
			int currentEntity=actions.actionQueue[i]->entityIndexSource;
			for(int j=1; j<int(registry.objMap[viewerCursor].entities.size()); j++)
			{
				if(registry.objMap[viewerCursor].entities[j]==currentEntity)
				{
					workingOnMap=true;
				}
			}
			for(int j=1; j<int(player.inv.cellList.size()); j++)
			{
				if(player.inv.cellList[j].idx_item==currentEntity)
				{
					workingOnMap=true;
				}
			}
			if(currentEntity==0) workingOnMap=true;
			if(player.gamemode==GAMEMODE_ZOOMOUT && !matchAction(actions.actionQueue[i], "zoomout")) workingOnMap=false;
			if(actions.actionQueue[i]->timeToActivate<=actSeconds && workingOnMap)
			{
				if(actions.actionQueue[i]->entityIndexSource>0)
					debugFile << tmp.container.entityList[ether.regEntities[actions.actionQueue[i]->entityIndexSource]->entityTemplateIndex].cname
					<< actions.actionQueue[i]->entityIndexSource << " - "
					<< tmp.container.actionList[actions.actionQueue[i]->actionTemplateIndex].cname << "\n";
				processAction(actions.actionQueue[i]);
			}
		}
	}
}

void GameClass::processTextList(float actSeconds)
{
	for(int i=1; i<int(ether.regText.size()); i++)
	{
		if(ether.regText[i] != NULL)
		{
			ether.regText[i]->alphacur-=6;
			if(ether.regText[i]->alphacur<1)
			{
				ether.regText[i]->alphacur=0;
				ether.regText[i]->active=false;
			}
			else
			{
				ether.regText[i]->color=(sf::Color(
					sf::Uint8(ether.regText[i]->color.r),
					sf::Uint8(ether.regText[i]->color.g),
					sf::Uint8(ether.regText[i]->color.b),
					sf::Uint8(ether.regText[i]->alphacur)));
			}
			ether.regText[i]->msg.setColor(ether.regText[i]->color);
			ether.regText[i]->msg.move(0,-1.25f);
		}
	}
}
bool GameClass::matchAction(const actionStruct* _act, const char* _name)
{
	return (actions.actionCodeEquals(tmp, _act->actionTemplateIndex, _name));
}

void GameClass::handleMovementPipeline(const actionStruct* act)
{
	if(matchAction(act, "selecttile"))
	{
		if(player.gamemode==GAMEMODE_ENTITYTARGETING)
		{
			sidebar.setString("Tile Selected");
			establishTargetTile(act->tileIndexTarget);
			return;
		}
	}
	if(matchAction(act, "selectentity"))
	{
		if(player.gamemode==GAMEMODE_ENTITYTARGETING)
		{
			establishTargetEntity(act->entityIndexTarget);
			return;
		}
	}
	if(matchAction(act, "establishtarget"))
	{
		sidebar.setString("establishing Target");
		actions.stopActionCategory(act->entityIndexSource, ACAT_COMBAT);
		actions.stopActionCategory(act->entityIndexSource, ACAT_PEACEFULAI);
		if(actions.tileTarget(ether, act))
		{
			actions.fillTileTargetAction(tmp, "movestep", act->entityIndexSource, act->tileIndexTarget, gameTime());
		}
		else
		{
			actions.fillEntityTargetAction(tmp, "movestep", act->entityIndexSource, act->entityIndexTarget, gameTime());
		}
		return;
	}
	if(matchAction(act, "movestep"))
	{
		if(!actions.hasSource(ether, act) || actions.noTarget(ether, act)) return;
		if(ether.regEntities[act->entityIndexSource]->type != ICAT_CREATURE) return;
		sidebar.setString("Moving one Step");
		if(ether.regEntities[act->entityIndexSource]->isEnemy)
		{
			if(isFriendlyNeighbor(act->entityIndexSource))
			{
				int friendly=getFriendlyNeighbor(act->entityIndexSource);
				actions.fillSourceAction(tmp, "canceltarget", act->entityIndexSource, gameTime());
				actions.fillEntityTargetAction(tmp, "engagecombat", act->entityIndexSource, friendly, gameTime());
				return;
			}
		}
		else
		{
			if(isEnemyNeighbor(act->entityIndexSource))
			{
				int enemy=getEnemyNeighbor(act->entityIndexSource);
				actions.fillSourceAction(tmp, "canceltarget", act->entityIndexSource, gameTime());
				actions.fillEntityTargetAction(tmp, "engagecombat", act->entityIndexSource, enemy, gameTime());
				return;
			}
		}
		fillPathingRoutes();
		if(actions.entityTarget(ether, act))
		{
			if(isReachedEntityTarget(act->entityIndexSource, act->entityIndexTarget))
			{
				//actions.fillSourceAction(tmp, "canceltarget", act->entityIndexSource, gameTime());
				actions.fillSourceAction(tmp, "idle", act->entityIndexSource, gameTime());
				return;
			}
			commitSmartPathing(act->entityIndexSource, ether.regEntities[act->entityIndexTarget]->pos, act);
			return;
		}
		else
		{
			commitSmartPathing(act->entityIndexSource, ether.regTiles[act->tileIndexTarget]->pos, act);
			return;
		}
	}
	if(matchAction(act, "movenorth"))
	{
		sidebar.setString("Moving North");
		coord dir=coord(0,-1);
		coord trg=ether.regEntities[act->entityIndexSource]->pos+dir;
		if(terrain.getTerrainAt(trg)>0) dir=coord(0,1);
		moveInDirection(act, dir, "movenorth");
		return;
	}
	if(matchAction(act, "movesouth"))
	{
		sidebar.setString("Moving South");
		coord dir=coord(0,1);
		coord trg=ether.regEntities[act->entityIndexSource]->pos+dir;
		if(terrain.getTerrainAt(trg)>0) dir=coord(0,-1);
		moveInDirection(act, dir, "movesouth");
		return;
	}
	if(matchAction(act, "moveeast"))
	{
		sidebar.setString("Moving East");
		coord dir=coord(1,0);
		coord trg=ether.regEntities[act->entityIndexSource]->pos+dir;
		if(terrain.getTerrainAt(trg)>0) dir=coord(-1,0);
		moveInDirection(act, dir, "moveeast");
		return;
	}
	if(matchAction(act, "movewest"))
	{
		sidebar.setString("Moving West");
		coord dir=coord(-1,0);
		coord trg=ether.regEntities[act->entityIndexSource]->pos+dir;
		if(terrain.getTerrainAt(trg)>0) dir=coord(1,0);
		moveInDirection(act, dir, "movewest");
		return;
	}
	if(matchAction(act, "movecreature"))
	{
		sidebar.setString("Targeting");
		player.gamemode=GAMEMODE_ENTITYTARGETING;
		return;
	}
	if(matchAction(act, "canceltarget"))
	{
		actions.stopActionCategory(act->entityIndexSource, ACAT_MOVEMENT);
		actions.stopActionCategory(act->entityIndexSource, ACAT_PEACEFULAI);
		gridAlignEntity(act->entityIndexSource, ether.regEntities[act->entityIndexSource]->pos);
		return;
	}
	if(matchAction(act, "endattack"))
	{
		//erase an effect from existence
		deleteEtherEntity(act->entityIndexSource, viewerCursor);
		return;
	}
}

void GameClass::handleCombatPipeline(const actionStruct* act)
{
	if(!actions.entityTarget(ether, act) || !actions.hasSource(ether, act) || (act->entityIndexSource == act->entityIndexTarget)) return;
	if(matchAction(act, "attack"))
	{
		//if(calcDist(toVector(ether.regEntities[act->entityIndexTarget]->pos), toVector(ether.regEntities[act->entityIndexSource]->pos))>1.4f)
		//	return;
		sidebar.setString("Hit!");
		int regSlash = addEntity("claw", viewerCursor, ether.regEntities[act->entityIndexTarget]->pos);
		actions.fillEntityTargetAction(tmp, "damage", act->entityIndexSource, act->entityIndexTarget, gameTime());
		return;
	}
	if(matchAction(act, "damage"))
	{
		if(act->entityIndexSource==0 || act->entityIndexTarget==0 || !isEntityOnMap(act->entityIndexTarget)) return;
		if(ether.regEntities[act->entityIndexSource]==NULL) return;
		if(ether.regEntities[act->entityIndexTarget]==NULL)
		{
			actions.fillSourceAction(tmp, "idle", act->entityIndexSource, gameTime());
			return;
		}
		char buf[24]="";
		int offenseValue=0;
		int defenseValue=0;
		//if the target is a creature, use the defense value
		if(ether.regEntities[act->entityIndexTarget]->type == ICAT_CREATURE)
		{
			defenseValue = ether.regCreature[ether.regEntities[act->entityIndexTarget]->packIndex]->defense;
		}
		else if(ether.regEntities[act->entityIndexTarget]->type == ICAT_DECORATION)
		{
			defenseValue = ether.regDeco[ether.regEntities[act->entityIndexTarget]->packIndex]->defense;
		}
		//if the source is not a creature, this is not the damage routine we will use
		if(ether.regEntities[act->entityIndexSource]->type != ICAT_CREATURE) return;
		//select an offense value
		offenseValue = ether.regCreature[ether.regEntities[act->entityIndexSource]->packIndex]->attack;
		int dmg = rand()%offenseValue+5;
		if(dmg>defenseValue) dmg-=defenseValue;
		sprintf_s(buf, "%i", dmg);
		addText(buf, viewerCursor, gridToPixel(ether.regEntities[act->entityIndexTarget]->pos), sf::Color::Red);
		if(ether.regEntities[act->entityIndexTarget]->type == ICAT_CREATURE)
		{
			//attack back if hit for the first time
//			if(ether.regEntities[act->entityIndexTarget]->isEnemy)
//			{
//				actions.fillEntityTargetAction(tmp, "establishtarget", act->entityIndexTarget, act->entityIndexSource, gameTime()+0.567f);
//			}
			ether.regCreature[ether.regEntities[act->entityIndexTarget]->packIndex]->currentHP-=dmg;
			if(ether.regCreature[ether.regEntities[act->entityIndexTarget]->packIndex]->currentHP<1)
			{
				removeEntity(act->entityIndexTarget, viewerCursor);
				actions.fillSourceAction(tmp, "idle", act->entityIndexSource, gameTime());
			}
		}
		else if(ether.regEntities[act->entityIndexTarget]->type == ICAT_DECORATION)
		{
			ether.regDeco[ether.regEntities[act->entityIndexTarget]->packIndex]->currentHP-=dmg;
			if(ether.regDeco[ether.regEntities[act->entityIndexTarget]->packIndex]->currentHP<1)
			{
				removeEntity(act->entityIndexTarget, viewerCursor);
				actions.fillSourceAction(tmp, "idle", act->entityIndexSource, gameTime());
			}
		}
		actions.fillEntityTargetAction(tmp, "engagecombat", act->entityIndexSource, act->entityIndexTarget, gameTime());

		return;
	}
	if(matchAction(act, "engagecombat"))
	{
		if(ether.regEntities[act->entityIndexSource] == NULL) return;
		if(!ether.regEntities[act->entityIndexSource]->active || !isEntityOnMap(act->entityIndexSource)) return;
		if(ether.regEntities[act->entityIndexTarget] == NULL || !ether.regEntities[act->entityIndexTarget]->active || !isEntityOnMap(act->entityIndexTarget))
		{
			actions.fillSourceAction(tmp, "idle", act->entityIndexSource, gameTime());
			return;
		}
		sidebar.setString("Attacking!");

		actions.stopActionCategory(act->entityIndexSource, ACAT_MOVEMENT);
		actions.stopActionCategory(act->entityIndexTarget, ACAT_MOVEMENT);
		actions.stopActionCategory(act->entityIndexSource, ACAT_PEACEFULAI);
		actions.stopActionCategory(act->entityIndexTarget, ACAT_PEACEFULAI);
		actions.fillEntityTargetAction(tmp, "attack", act->entityIndexSource, act->entityIndexTarget, gameTime());
		return;
	}
}

void GameClass::handleCreationPipeline(const actionStruct* act)
{
	if(!actions.hasSource(ether, act)) return;
	if(matchAction(act, "makecreature"))
	{
		actions.fillSourceAction(tmp, "wandercreature", act->entityIndexSource, gameTime());
		addButton("movebutton", viewerCursor, gridToPixel(coord(settings.mapGridDimensions.x+1,1)), act->entityIndexSource);
		return;
	}
	if(matchAction(act, "makespawner"))
	{
		ether.regEntities[act->entityIndexSource]->isEnemy=true;
		addText("<-HERE", viewerCursor, gridToPixel(ether.regEntities[act->entityIndexSource]->pos), sf::Color::Red);
		actions.fillSourceAction(tmp, "spawnenemy", act->entityIndexSource, gameTime());
		return;
	}
	if(matchAction(act, "spawnenemy"))
	{
		int dir=rand()%4;
		coord ff=ether.regEntities[act->entityIndexSource]->pos;
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

		//addText("spawn", viewerCursor, gridToPixel(ether.regEntities[act->entityIndexSource]->pos), sf::Color::Red);
		addEntity("tinspider", viewerCursor, ff);
		if(getEnemyCount(viewerCursor)<6)
			actions.fillSourceAction(tmp, "spawnenemy", act->entityIndexSource, gameTime());
		return;
	}
	if(matchAction(act, "makeenemy"))
	{
		ether.regEntities[act->entityIndexSource]->isEnemy=true;
		//the enemy looks for the closest friendly, if it is on the map
		actions.fillSourceAction(tmp, "agro", act->entityIndexSource, gameTime());
		return;
	}
	if(matchAction(act, "growflower"))
	{
		//update the frame as long as it's still less than the max growth stages
		actions.processGrowth(tmp, ether, act->entityIndexSource, gameTime());
		return;
	}
	if(matchAction(act, "effects"))
	{
		//update the frame as long as it's still less than the max growth stages
		actions.processMagic(tmp, ether, act->entityIndexSource, gameTime());

		return;
	}
	if(matchAction(act, "swipeeffect"))
	{
		//update the frame as long as it's still less than the max growth stages
		actions.processAttack(tmp, ether, act->entityIndexSource, gameTime());
		debugFile << "Frame: " << ether.regEntities[act->entityIndexSource]->frame << "\n";
		return;
	}
}

void GameClass::handleButtonPipeline(const actionStruct* act)
{
	if(matchAction(act, "togglemap"))
	{
		actions.fillSourceAction(tmp, "zoomout", act->entityIndexSource, gameTime());
		sf::Image tempImg;
		tempImg.create(settings.mapGridDimensions.x, settings.mapGridDimensions.y, sf::Color(80,80,80,99));
		sf::Image screenCap = app.capture();
		for(int y=0; y<settings.mapGridDimensions.y; y++)
		{
			for(int x=0; x<settings.mapGridDimensions.x; x++)
			{
				tempImg.setPixel(x,y,screenCap.getPixel((x*settings.tileDimensions.x)+18, (y*settings.tileDimensions.y)+19));
				if(x==0 || y==0 || x==settings.mapGridDimensions.x-1 || y==settings.mapGridDimensions.y-1)
				{
					if(getMapMode(viewerCursor)==MAPMODE_FRESH)
						tempImg.setPixel(x,y,sf::Color(90, 90, 190, 80));
					else if(getMapMode(viewerCursor)==MAPMODE_SANCTUARY)
						tempImg.setPixel(x,y,sf::Color(180, 180, 90, 80));
					else if(getMapMode(viewerCursor)==MAPMODE_THREATEN)
						tempImg.setPixel(x,y,sf::Color(180, 60, 60, 99));
				}
			}
		}
		registry.objMap[viewerCursor].mapSheet.loadFromImage(tempImg);
		return;
	}
	if(matchAction(act, "infoget"))
	{
		if(player.gamemode==GAMEMODE_INSPECT)
		{
			actions.fillSourceAction(tmp, "clearsidebar", 0, gameTime());
		}
		else
		{
			sidebar.setString("Click an Entity\nto Inspect...");
			player.gamemode=GAMEMODE_INSPECT;
		}
		return;
	}
	if(matchAction(act, "generatemap"))
	{
		dumpActionList=true;
		player.gamemode=GAMEMODE_NEUTRAL;
		generatorCursor=viewerCursor;
		if(newGame)
		{
			generatorCursor=coord(0,0);
			viewerCursor=coord(0,0);
			newGame=false;
		}
		refreshMap(generatorCursor);
		experimentalWorldGen(viewerCursor);
		addText(getMapName(viewerCursor), viewerCursor, coord(160,200), sf::Color::Green);
		if(getMapMode(viewerCursor)==MAPMODE_SANCTUARY) addText("PEACEFUL", viewerCursor, coord(90,500), sf::Color::Blue);
		if(getMapMode(viewerCursor)==MAPMODE_THREATEN) addText("DEFEND THE LAND!", viewerCursor, coord(90,500), sf::Color::Red);
		generatorCursor=viewerCursor;
		newGame=false;
		player.ritualForm.clear();
		player.inventoryForm.clear();
		player.sideMenuForm.clear();
		player.creatureCard.clear();
		player.sideMenuForm.addCell(RENDER_BUTTON, getGuiTemplateIndex("magnifier"), gridToPixel(coord(settings.mapGridDimensions.x,5)));
		player.sideMenuForm.addCell(RENDER_BUTTON, getGuiTemplateIndex("backpack"), gridToPixel(coord(settings.mapGridDimensions.x+1,4)));
		player.sideMenuForm.addCell(RENDER_BUTTON, getGuiTemplateIndex("worldmap"), gridToPixel(coord(settings.mapGridDimensions.x+2,5)));
		player.sideMenuForm.addCell(RENDER_BUTTON, getGuiTemplateIndex("camera"), gridToPixel(coord(settings.mapGridDimensions.x+1,6)));
		fillGuiForm(player.sideMenuForm);
		return;
	}
	if(matchAction(act, "screenshot"))
	{
//		gamemode=GAMEMODE_NEUTRAL;
		capture();
		return;
	}
	if(matchAction(act, "backpack"))
	{
		if(player.gamemode!=GAMEMODE_INVENTORY)
		{
			fillInventory();
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
	if(matchAction(act, "selectinventory"))
	{
		if(player.gamemode==GAMEMODE_INVENTORY) player.inv.select(player.deliverGridClick(app));
		else if(player.gamemode==GAMEMODE_CRAFTING)
		{
			player.inv.select(player.deliverGridClick(app));
			if(player.inv.getItemAtCursor()>=0 && player.ritual.cursor<(player.ritual.resultslot))
			{
				player.ritual.addToRitual(player.inv.drop(player.inv.cursor));
				actions.fillSourceAction(tmp, "addritualitem", 0, gameTime());
			}
		}
		return;
	}
	if(matchAction(act, "addritualitem"))
	{
		if(player.gamemode!=GAMEMODE_CRAFTING) return;
		int y=80;
		for(int i=0; i<player.ritual.slots-1; i++)
		{
			if(i==1) y=75;
			int x=((192/3)*i)+10;
			if(player.ritual.templateFromSlot(ether, i)>0)
			{
				player.ritualForm.addCell(RENDER_ENTITY, player.ritual.templateFromSlot(ether, i), coord(190+x, y));
			}
		}
		if(player.ritual.isThisRitual(ether, player.recipes))
		{
			sidebar.setString("New Item Craftable");
			player.ritual.addToRitual(player.ritual.findRitual(ether, player.recipes));
			player.ritualForm.addCell(RENDER_ENTITY, player.ritual.findRitual(ether, player.recipes), coord(190+((192/3)*1)+10, y+50));
			fillGuiForm(player.ritualForm);
			player.ritualForm.activate();
		}
		return;
	}

}

void GameClass::handleItemsPipeline(const actionStruct* act)
{
	if(matchAction(act, "usecharm"))
	{
		if(!actions.hasSource(ether, act)) return;
		int summonPack = ether.regEntities[act->entityIndexSource]->packIndex;
		int held = ether.regSummon[summonPack]->creatureContained;
		if(actions.entityTarget(ether, act))
		{
			if(ether.regEntities[act->entityIndexTarget]->type==ICAT_CREATURE)
			{
				if(held==0)
				{
					int regMagic = addEntity("magiceffect", viewerCursor, ether.regEntities[act->entityIndexTarget]->pos);
					actions.fillSourceAction(tmp, "effects", regMagic, gameTime());
					removeEntity(act->entityIndexTarget, viewerCursor);
					ether.regSummon[summonPack]->creatureContained=act->entityIndexTarget;
				}
			}
			return;
		}
		else if(actions.tileTarget(ether, act))
		{
			if(held>0)
			{
				int regMagic = addEntity("magiceffect", viewerCursor, ether.regTiles[act->tileIndexTarget]->pos);
				actions.fillSourceAction(tmp, "effects", regMagic, gameTime());
				if(ether.regEntities[held]->type==ICAT_CREATURE)
				{
					registry.objMap[viewerCursor].entities.push_back(held);
					int creature=ether.regEntities[held]->packIndex;
					ether.regCreature[creature]->offset=coord(0,0);
					ether.regEntities[held]->plane=0;
					ether.regEntities[held]->pos=ether.regTiles[act->tileIndexTarget]->pos;
					ether.regEntities[held]->active=true;
					//actions.stopActionCategory(held, ACAT_MOVEMENT);
					//actions.stopActionCategory(held, ACAT_PEACEFULAI);
					//actions.stopActionCategory(held, ACAT_COMBAT);
					actions.fillSourceAction(tmp, tmp.container.entityList[ether.regEntities[held]->entityTemplateIndex].creation, held, gameTime());

					ether.regEntities[held]->box = tmp.container.entityList[ether.regEntities[held]->entityTemplateIndex].box;
					ether.regEntities[held]->box.left+=(ether.regEntities[held]->pos.x*32);
					ether.regEntities[held]->box.top+=(ether.regEntities[held]->pos.y*32);
				}
				ether.regSummon[summonPack]->creatureContained=0;
			}
			return;
		}
	}
	if(matchAction(act, "spelltile"))
	{
		int regMagic = addEntity("magiceffect", viewerCursor, ether.regTiles[act->tileIndexTarget]->pos);
		actions.fillSourceAction(tmp, "effects", regMagic, gameTime());
		return;
	}
	if(matchAction(act, "plantflower"))
	{
		if(ether.regEntities[act->entityIndexSource]->type != ICAT_SEED) return;
		int seedIndex=ether.regEntities[act->entityIndexSource]->packIndex;
		int vegTemplate=ether.regSeed[seedIndex]->vegetationContained;
		addEntity(tmp.container.entityList[vegTemplate].cname, viewerCursor, ether.regTiles[act->tileIndexTarget]->pos);
		return;
	}
	if(matchAction(act, "destroytool"))
	{
		//erase a tool from existence
		player.inv.clearSlot(player.inv.cursor);
		removeEntity(act->entityIndexSource, viewerCursor);

		return;
	}
	if(matchAction(act, "vanishentity"))
	{
		//erase an entity from the map
		removeEntity(act->entityIndexSource, viewerCursor);

		return;
	}
	if(matchAction(act, "convertflower"))
	{
		registry.objMap[viewerCursor].entities.push_back(actions.processFlowerConversion(tmp, ether, act->entityIndexSource, gameTime()));
		return;
	}
}

void GameClass::handleAIPipeline(const actionStruct* act)
{
	if(matchAction(act, "idle"))
	{
		fillPathingRoutes();
		if(ether.regEntities[act->entityIndexSource]->isEnemy)
		{
			actions.fillSourceAction(tmp, "agro", act->entityIndexSource, gameTime());
		}
		else
		{
			actions.fillSourceAction(tmp, "wandercreature", act->entityIndexSource, gameTime());
		}
		return;
	}
	if(matchAction(act, "pursuit"))
	{
		if(actions.entityTarget(ether, act))
		{
			if(ether.regEntities[act->entityIndexTarget]->active)
			{
				actions.fillEntityTargetAction(tmp, "movestep", act->entityIndexSource, act->entityIndexTarget, gameTime());
				actions.fillEntityTargetAction(tmp, "pursuit", act->entityIndexSource, act->entityIndexTarget, gameTime());
			}
		}
		return;
	}
	if(matchAction(act, "agro"))
	{
		//for enemies to target and attack their closest friendly/decoration
		if(isFriendlyRange(act->entityIndexSource, 16.0f))
		{
			int friendly=getClosestFriendlyRange(act->entityIndexSource, 16.0f);
			actions.fillEntityTargetAction(tmp, "establishtarget", act->entityIndexSource, friendly, gameTime());
			return;
		}
		actions.fillSourceAction(tmp, "idle", act->entityIndexSource, gameTime());
		return;
	}
	if(matchAction(act, "wandercreature"))
	{
		if(isEnemyNeighbor(act->entityIndexSource))
		{
			int targ = getEnemyNeighbor(act->entityIndexSource);
			actions.fillEntityTargetAction(tmp, "engagecombat", act->entityIndexSource, targ, gameTime());
			return;
		}
		if(!(actions.isPerformingAction(tmp, act->entityIndexSource, "movestep")))
		{
			coord ff=ether.regEntities[act->entityIndexSource]->pos;
			if(rand()%60==0)
			{
				int dir=rand()%4;
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
			}
			if(terrain.getTerrainAt(ff)==0)
			{
				actions.fillTileTargetAction(tmp, "movestep", act->entityIndexSource, getTileIndexAt(ff), gameTime());
			}
		}
		actions.fillSourceAction(tmp, "idle", act->entityIndexSource, gameTime());
		return;
	}
}

void GameClass::handleGUIPipeline(const actionStruct* act)
{
	if(matchAction(act, "guion"))
	{
		return;
	}
	if(matchAction(act, "zoomout"))
	{
		if(mapscale<float(16.0f))
		{
			player.gamemode=GAMEMODE_ZOOMOUT;
			mapscale*=(32.0f/31.0f);
			actions.fillSourceAction(tmp, "zoomout", act->entityIndexSource, gameTime());
		}
		else
		{
			player.gamemode=GAMEMODE_MINIMAP;
		}
		return;
	}
	if(matchAction(act, "randomheld"))
	{
		player.inv.add(ether, act->entityIndexSource);
		removeEntity(act->entityIndexSource, viewerCursor);
		return;
	}
	if(matchAction(act, "creatureguion"))
	{
		sidebar.setString("");
		player.creatureCard.clear();
		if(player.gamemode!=GAMEMODE_ENTITYACTION)
		{
			player.gamemode=GAMEMODE_ENTITYACTION;
			player.creatureCard.addCell(RENDER_BUTTON, getGuiTemplateIndex("inventorycell"), gridToPixel(coord(settings.mapGridDimensions.x, 1)));
			player.creatureCard.addCell(RENDER_ENTITY, ether.regEntities[act->entityIndexSource]->entityTemplateIndex, gridToPixel(coord(settings.mapGridDimensions.x, 1)));
			player.creatureCard.addCell(RENDER_BUTTON, getGuiTemplateIndex("movebutton"), gridToPixel(coord(settings.mapGridDimensions.x+1, 1)));

			fillGuiForm(player.creatureCard, act->entityIndexSource, true);
			activateEntityButton(act->entityIndexSource);
		}
		else
		{
			actions.fillSourceAction(tmp, "creatureguioff", act->entityIndexSource, gameTime());
		}
		return;
	}
	if(matchAction(act, "creatureguioff"))
	{
		deactivateEntityButton(act->entityIndexSource);
		eraseGuiForm(player.creatureCard);
		player.creatureCard.clear();
		player.gamemode=GAMEMODE_NEUTRAL;
		return;
	}
	if(matchAction(act, "selecttile"))
	{
		if(player.gamemode==GAMEMODE_INSPECT)
		{
			sidebar.setString(outputTile(act->tileIndexTarget));
			return;
		}
		if(player.gamemode==GAMEMODE_INVENTORY)
		{
			if(ether.regEntities[player.inv.getItemAtCursor()]==NULL) return;
			if(ether.regEntities[player.inv.getItemAtCursor()]->type==ICAT_SUMMON)
			{
				actions.useCharm(tmp, ether, player.inv.getItemAtCursor(), 0, act->tileIndexTarget, gameTime());
				sidebar.setString(outputEntity(player.inv.getItemAtCursor()));
			}
		}
	}
	if(matchAction(act, "selectentity"))
	{
		if(player.gamemode==GAMEMODE_INSPECT)
		{
			sidebar.setString(outputEntity(act->entityIndexTarget));
			return;
		}
		if(player.gamemode==GAMEMODE_INVENTORY)
		{
			if(ether.regEntities[player.inv.getItemAtCursor()]==NULL) return;
			if(ether.regEntities[player.inv.getItemAtCursor()]->type==ICAT_SUMMON)
			{
				actions.useCharm(tmp, ether, player.inv.getItemAtCursor(), act->entityIndexTarget, 0, gameTime());
				sidebar.setString(outputEntity(player.inv.getItemAtCursor()));
			}
		}
		if(ether.regEntities[act->entityIndexTarget]->entityTemplateIndex == tmp.container.getEntityTemplate("ritualstump"))
		{
			actions.fillSourceAction(tmp, "togglecrafting", act->entityIndexTarget, gameTime());
			return;
		}
		if(player.gamemode==GAMEMODE_NEUTRAL)
		{
			if(act->entityIndexTarget==0) return;
			if(ether.regEntities[act->entityIndexTarget]->type == ICAT_CREATURE)
			{
				actions.fillSourceAction(tmp, "creatureguion", act->entityIndexTarget, gameTime());
				return;
			}
			if(ether.regEntities[act->entityIndexTarget]->type == ICAT_VEGETATION)
			{
				actions.fillSourceAction(tmp, "convertflower", act->entityIndexTarget, gameTime());
				return;
			}
			if(ether.regEntities[act->entityIndexTarget]->type == ICAT_INGREDIENT)
			{
				actions.fillSourceAction(tmp, "randomheld", act->entityIndexTarget, gameTime());
				return;
			}
			return;
		}
	}
	if(matchAction(act, "clearsidebar"))
	{
		sidebar.setString("");
		player.gamemode=GAMEMODE_NEUTRAL;
		return;
	}
	if(matchAction(act, "trycrafting"))
	{
		sidebar.setString("Attempting Craft");
		if(!player.ritual.isThisRitual(ether, player.recipes)) return;
		if(player.gamemode==GAMEMODE_CRAFTING)
		{
			//eraseGuiForm(player.ritualForm);
			//eraseGuiForm(player.inventoryForm);
			if(player.ritual.isThisRitual(ether, player.recipes))
			{
				int result=ether.createEntity(tmp, tmp.container.entityList[player.ritual.findRitual(ether, player.recipes)].cname);
				player.ritual.cell[player.ritual.resultslot].entityIndex=result;
				player.ritual.addSlotToInventory(player.ritual.resultslot, ether, player.inv);
			player.ritual.clear();
			player.ritualForm.clear();
			player.ritualForm.addCell(RENDER_BUTTON, getGuiTemplateIndex("openritualcell"), coord(190+10, 80));
			player.ritualForm.addCell(RENDER_BUTTON, getGuiTemplateIndex("openritualcell"), coord(190+(192/2)-16, 75));
			player.ritualForm.addCell(RENDER_BUTTON, getGuiTemplateIndex("openritualcell"), coord(190+192-10-32, 80));
			player.ritualForm.addCell(RENDER_BUTTON, getGuiTemplateIndex("resultritualcell"), coord(190+(192/2)-16, 120));
			fillInventory();
			fillGuiForm(player.ritualForm);
			}
		}
		return;
	}
	if(actions.actionCodeEquals(tmp, act->actionTemplateIndex, "togglecrafting"))
	{
		if(player.gamemode==GAMEMODE_CRAFTING)
		{
			sidebar.setString("");
			eraseGuiForm(player.ritualForm);
			eraseGuiForm(player.inventoryForm);
			player.inventoryForm.clear();
			player.ritualForm.clear();
			for(int i=0; i<int(player.ritual.cursor); i++)
			{
				if(i != player.ritual.resultslot)
					player.ritual.addSlotToInventory(i, ether, player.inv);
			}
			player.ritual.clear();
			player.gamemode=GAMEMODE_NEUTRAL;
		}
		else
		{
			sidebar.setString("Crafting ON");
			player.ritualForm.addCell(RENDER_BUTTON, getGuiTemplateIndex("ritualgui"), coord(190,60));
			player.ritualForm.addCell(RENDER_BUTTON, getGuiTemplateIndex("openritualcell"), coord(190+10, 80));
			player.ritualForm.addCell(RENDER_BUTTON, getGuiTemplateIndex("openritualcell"), coord(190+(192/2)-16, 75));
			player.ritualForm.addCell(RENDER_BUTTON, getGuiTemplateIndex("openritualcell"), coord(190+192-10-32, 80));
			player.ritualForm.addCell(RENDER_BUTTON, getGuiTemplateIndex("resultritualcell"), coord(190+(192/2)-16, 120));
			fillInventory();
			fillGuiForm(player.ritualForm);
			player.gamemode=GAMEMODE_CRAFTING;
		}
		return;
	}
}

//processes an active, valid and queued action
void GameClass::processAction(actionStruct* act)
{
	if(!actions.validateAction(ether, act)) return;

	handleMovementPipeline(act);
	handleCombatPipeline(act);
	handleCreationPipeline(act);
	handleButtonPipeline(act);
	handleItemsPipeline(act);
	handleAIPipeline(act);
	handleGUIPipeline(act);
	act->active=false;
}

bool GameClass::isEntityOccupyingGrid(int tileIndex)
{
	if(tileIndex<0 || ether.regTiles[tileIndex]==NULL) return false;
	return (registry.objMap[viewerCursor].entityIndexOnGrid(ether, ether.regTiles[tileIndex]->pos, 0)>0);
}

void GameClass::establishTargetTile(int tileIndex)
{
	int regEnt=getButtonLinkedEntity("movecreature");
	coord grid = ether.regTiles[tileIndex]->pos;
	if(isEntityOccupyingGrid(tileIndex))
	{
		establishTargetEntity(registry.objMap[viewerCursor].entityIndexOnGrid(ether, grid, 0));
		return;
	}
	actions.fillTileTargetAction(tmp, "establishtarget", regEnt, tileIndex, gameTime());
	actions.fillSourceAction(tmp, "creatureguioff", regEnt, gameTime());
	return;
}

void GameClass::establishTargetEntity(int entityIndex)
{
	int regEnt=getButtonLinkedEntity("movecreature");
	actions.fillEntityTargetAction(tmp, "establishtarget", regEnt, entityIndex, gameTime());
	actions.fillSourceAction(tmp, "creatureguioff", regEnt, gameTime());
	return;
}

void GameClass::moveInDirection(const actionStruct* act, coord dir, const char* protocol)
{
		forcePushCreature(act->entityIndexSource, dir);
		if(!isOffsetCongruent(act->entityIndexSource))
		{
			forceOffsetCorrection(act->entityIndexSource, dir);
		}
		if(!moveToAlign(act->entityIndexSource))
		{
			if(actions.entityTarget(ether, act))
				actions.fillEntityTargetAction(tmp, protocol, act->entityIndexSource, act->entityIndexTarget, gameTime());
			else
				actions.fillTileTargetAction(tmp, protocol, act->entityIndexSource, act->tileIndexTarget, gameTime());
		}
		else
		{
			if(actions.entityTarget(ether, act))
				actions.fillEntityTargetAction(tmp, "movestep", act->entityIndexSource, act->entityIndexTarget, gameTime());
			else
				actions.fillTileTargetAction(tmp, "movestep", act->entityIndexSource, act->tileIndexTarget, gameTime());
		}
}