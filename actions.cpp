#include "globals.h"

ActionManager::ActionManager()
{
	actionQueue.clear(); actionQueue.push_back(NULL);
}

ActionManager::~ActionManager()
{
	int max=int(actionQueue.size())-1;
	for(int i=max; i>0; i--)
	{
		delete actionQueue[i];
		actionQueue.erase(actionQueue.begin()+i);
	}
}
bool ActionManager::actionCodeEquals(TemplateRegistryClass& tmp, int index, const char* _code)
{
	if(index<=0 || index>int(tmp.container.actionList.size()) || _code==NULL || tmp.container.actionList[index].cname==NULL) return false;
	return (strcmp(tmp.container.actionList[index].cname, _code)==0);
}

int ActionManager::getActionIndex(TemplateRegistryClass& tmp, int entityIndex, const char* actionName)
{
	for(int i=1; i<int(actionQueue.size()); i++)
	{
		if(actionQueue[i]->entityIndexSource == entityIndex && actionQueue[i]->active)
		{
			if(actionCodeEquals(tmp, actionQueue[i]->actionTemplateIndex, actionName))
			{
				return i;
			}
		}
	}
	return 0;
}

void ActionManager::processGrowth(TemplateRegistryClass& tmp, EtherRegistryClass& _eth, int etherIndex, float time)
{
	int veg=_eth.regEntities[etherIndex]->packIndex;
	if(veg==0 || etherIndex==0) return;
	float duration=float(_eth.regVeg[veg]->growthTicks)*0.2f;
	if(_eth.regVeg[veg]->currentGrowth < (_eth.regVeg[veg]->maxGrowth-1))
	{
		_eth.regVeg[veg]->currentGrowth++;
		_eth.regEntities[etherIndex]->frame = _eth.regVeg[veg]->currentGrowth;
		fillSourceAction(tmp, "growflower", etherIndex, time+duration);
	}
}

void ActionManager::processMagic(TemplateRegistryClass& tmp, EtherRegistryClass& _eth, int etherIndex, float time)
{
	if(etherIndex==0) return;
	if(_eth.regEntities[etherIndex]->frame < 8)
	{
		_eth.regEntities[etherIndex]->frame += 1;
		fillSourceAction(tmp, "effects", etherIndex, time);
	}
	else
		fillSourceAction(tmp, "vanishentity", etherIndex, time);
}

void ActionManager::processAttack(TemplateRegistryClass& tmp, EtherRegistryClass& _eth, int etherIndex, float time)
{
	if(etherIndex==0) return;
	if(_eth.regEntities[etherIndex]->frame < 8)
	{
		_eth.regEntities[etherIndex]->frame += 1;
		fillSourceAction(tmp, "swipeeffect", etherIndex, time);
	}
	else
		fillSourceAction(tmp, "vanishentity", etherIndex, time);
}

int ActionManager::processFlowerConversion(TemplateRegistryClass& tmp, EtherRegistryClass& _eth, int etherIndex, float time)
{
	if(_eth.regEntities[etherIndex]->type != ICAT_VEGETATION) return 0;
	int veg=_eth.regEntities[etherIndex]->packIndex;
	int drops=_eth.regVeg[veg]->dropList;
	coord psx=_eth.regEntities[etherIndex]->pos;
	int pick=_eth.randomEntityFromList(tmp, tmp.container.valuesList[drops].cname);
	fillSourceAction(tmp, "vanishentity", etherIndex, time);
	int drop=_eth.createEntity(tmp, tmp.container.entityList[pick].cname);
	_eth.regEntities[drop]->pos=psx;
	_eth.regEntities[drop]->box.left+=(psx.x*32);
	_eth.regEntities[drop]->box.top+=(psx.y*32);
	return drop;
}

bool ActionManager::validateAction(EtherRegistryClass& _eth, const actionStruct* act)
{
	if(act==NULL) return false;
	if(!act->active) return false;
	if(act->entityIndexSource<0) return false;
	if(act->entityIndexTarget<0 || act->entityIndexTarget>int(_eth.regEntities.size())) return false;
	if(act->tileIndexTarget<0 || act->tileIndexTarget>int(_eth.regTiles.size())) return false;
	return true;
}

void ActionManager::wipeEntityFromActionList(int entityIndex)
{
	for(int i=1; i<int(actionQueue.size()); i++)
	{
		if(actionQueue[i]->entityIndexSource==entityIndex)
		{
			actionQueue[i]->entityIndexSource=0;
			actionQueue[i]->active=false;
		}
		if(actionQueue[i]->entityIndexTarget==entityIndex)
		{
			actionQueue[i]->entityIndexTarget=0;
			actionQueue[i]->active=false;
		}
	}
}
bool ActionManager::isPerformingAction(TemplateRegistryClass& tmp, int entityIndex, const char* actionName)
{
	bool ret=false;
	for(int i=1; i<int(actionQueue.size()); i++)
	{
		if(actionCodeEquals(tmp, actionQueue[i]->actionTemplateIndex, actionName) && actionQueue[i]->entityIndexSource==entityIndex)
		{
			if(actionQueue[i]->active && (actionQueue[i]->queued || actionQueue[i]->cooling))
				ret=true;
		}
	}
	return ret;
}

bool ActionManager::isTargetOfAction(TemplateRegistryClass& tmp, int entityIndex, const char* actionName)
{
	bool ret=false;
	for(int i=1; i<int(actionQueue.size()); i++)
	{
		if(actionCodeEquals(tmp, actionQueue[i]->actionTemplateIndex, actionName) && actionQueue[i]->entityIndexTarget==entityIndex)
		{
			if(actionQueue[i]->active)
				ret=true;
		}
	}
	return ret;
}

int ActionManager::getSourceOfTargetAction(TemplateRegistryClass& tmp, int entityIndex, const char* actionName)
{
	int ret=0;
	for(int i=1; i<int(actionQueue.size()); i++)
	{
		if(actionCodeEquals(tmp, actionQueue[i]->actionTemplateIndex, actionName) && actionQueue[i]->entityIndexTarget==entityIndex)
		{
			if(actionQueue[i]->active)
				ret=actionQueue[i]->entityIndexSource;
		}
	}
	return ret;
}

void ActionManager::fillSourceAction(TemplateRegistryClass& tmp, const char* actionname, int entityIndex, float time)
{
	createAction(tmp, actionname, entityIndex, 0, 0, time);
}

void ActionManager::fillGUITargetAction(TemplateRegistryClass& tmp, const char* actionname, int sourceIndex, int buttonIndex, float time)
{
	createButtonAction(tmp, actionname, sourceIndex, buttonIndex, time);
}

void ActionManager::fillEntityTargetAction(TemplateRegistryClass& tmp, const char* actionname, int entityIndex, int entityTarget, float time)
{
	createAction(tmp, actionname, entityIndex, entityTarget, 0, time);
}

void ActionManager::fillTileTargetAction(TemplateRegistryClass& tmp, const char* actionname, int entityIndex, int tileTarget, float time)
{
	createAction(tmp, actionname, entityIndex, 0, tileTarget, time);
}

bool ActionManager::noTarget(EtherRegistryClass& _eth, const actionStruct* act)
{
	return (!(entityTarget(_eth, act) || tileTarget(_eth, act)));
}

bool ActionManager::entityTarget(EtherRegistryClass& _eth, const actionStruct* act)
{
	return (act->entityIndexTarget != 0 && _eth.regEntities[act->entityIndexTarget] != NULL);
}

bool ActionManager::tileTarget(EtherRegistryClass& _eth, const actionStruct* act)
{
	return (act->tileIndexTarget != 0);// && _eth.regTiles[act->tileIndexTarget] != NULL);
}

bool ActionManager::fullTargets(EtherRegistryClass& _eth, const actionStruct* act)
{
	return (entityTarget(_eth, act) && tileTarget(_eth, act));
}

void ActionManager::useTool(TemplateRegistryClass& tmp, EtherRegistryClass& _eth, int etherIndex, int entTarget, int tileTarget, float time)
{
	if(entTarget==0 && tileTarget==0) return;
	if(entTarget>0)
	{
		return;
	}
	if(tileTarget>0)
	{
		//work on a tile
		int toolPack=_eth.regEntities[etherIndex]->packIndex;
		int protocol=_eth.regTool[toolPack]->usageProtocol;
		_eth.regTool[toolPack]->usesLeft-=1;
		if(_eth.regTool[toolPack]->usesLeft<1)
		{
			fillSourceAction(tmp, "destroytool", etherIndex, time);
		}
		fillTileTargetAction(tmp, tmp.container.actionList[protocol].cname, etherIndex, tileTarget, time);
		return;
	}
}

void ActionManager::useCharm(TemplateRegistryClass& tmp, EtherRegistryClass& _eth, int etherIndex, int entTarget, int tileTarget, float time)
{
	if(etherIndex==0) return;
	//if(entTarget==0 && tileTarget==0) return;
	int summonPack=_eth.regEntities[etherIndex]->packIndex;
	int held=_eth.regSummon[summonPack]->creatureContained;
	if(entTarget>0)
	{
		//work on a tile (summon contained entity)
		int protocol=_eth.regSummon[summonPack]->usageProtocol;
		bool isCreature = (_eth.regEntities[entTarget]->type==ICAT_CREATURE);
		if(held==0 && isCreature) //if the charm is empty, we'll capture the creature
			fillEntityTargetAction(tmp, tmp.container.actionList[protocol].cname, etherIndex, entTarget, time);
		return;
	}
	if(tileTarget>0 && held>0)
	{
		//work on a tile (summon contained entity)
		int protocol=_eth.regSummon[summonPack]->usageProtocol;
		if(held==0) return; //if the charm is empty, we have nothing to summon
		fillTileTargetAction(tmp, tmp.container.actionList[protocol].cname, etherIndex, tileTarget, time);
		return;
	}
}

void ActionManager::plantSeed(TemplateRegistryClass& tmp, EtherRegistryClass& _eth, int etherIndex, int entTarget, int tileTarget, float time)
{
	if(entTarget==0 && tileTarget==0) return;
	if(entTarget>0)
	{
		return;
	}
	if(tileTarget>0)
	{
		//work on a tile
		int seedPack=_eth.regEntities[etherIndex]->packIndex;
		int protocol=_eth.regSeed[seedPack]->usageProtocol;
		fillTileTargetAction(tmp, tmp.container.actionList[protocol].cname, etherIndex, tileTarget, time);
		return;
	}
}

bool ActionManager::fillExitAction(const TemplateRegistryClass& tmp, const char* _name, const actionStruct* _act)
{
	return createAction(tmp, _name,
		_act->entityIndexSource,
		_act->entityIndexTarget,
		_act->tileIndexTarget,
		_act->cooldownTime);
}

bool ActionManager::createAction(const TemplateRegistryClass& tmp, const char* _name, int entitySrc, int entityTrg, int tileTrg, float time)
{
	//first search the action template registry
	for(int i=1; i<int(tmp.container.actionList.size()); i++)
	{
		if(strcmp(tmp.container.actionList[i].cname, _name)==0)
		{
			actionStruct act;
			act.active=true;
			act.actionTemplateIndex=i;
			act.priority=1;
			act.entityIndexSource=entitySrc;
			act.entityIndexTarget=entityTrg;
			act.tileIndexTarget=tileTrg;
			act.initTime=time;
			act.timePoint=time;
			act.delayTime=time+(float((tmp.container.actionList[i].delayTicks))*FRAMESPEED);
			act.cooldownTime=act.delayTime+(float((tmp.container.actionList[i].coolDownTicks))*FRAMESPEED);
			act.queued=false;
			act.cooling=false;
			act.category=tmp.container.actionList[i].category;
			act.guiIndexTarget=0; //leaving this at 0 for now
			
			//we can recycle inactive actions at this point
			//SIGNIFICANT decrease in vector size :)
			for(int i=1; i<int(actionQueue.size()); i++)
			{
				if(!actionQueue[i]->active)
				{
					delete actionQueue[i]; //deallocate old memory
					actionQueue[i] = new actionStruct(act);	//allocate new memory
					return true;
				}
			}
			//create a new action if all the rest of the actions are active
			actionQueue.push_back(new actionStruct(act));
			return true;
		}
	}
	return false;
}

bool ActionManager::createButtonAction(const TemplateRegistryClass& tmp, const char* _name, int entitySrc, int buttonIndex, float time)
{
	//first search the action template registry
	for(int i=1; i<int(tmp.container.actionList.size()); i++)
	{
		if(strcmp(tmp.container.actionList[i].cname, _name)==0)
		{
			actionStruct act;
			act.active=true;
			act.actionTemplateIndex=i;
			act.priority=1;
			act.entityIndexSource=entitySrc;
			act.entityIndexTarget=0;
			act.tileIndexTarget=0;
			act.initTime=time;
			act.timePoint=time;
			act.delayTime=time+(float((tmp.container.actionList[i].delayTicks))*FRAMESPEED);
			act.cooldownTime=act.delayTime+(float((tmp.container.actionList[i].coolDownTicks))*FRAMESPEED);
			act.queued=false;
			act.cooling=false;
			act.category=tmp.container.actionList[i].category;
			act.guiIndexTarget=buttonIndex;
			
			//we can recycle inactive actions at this point
			//SIGNIFICANT decrease in vector size :)
			for(int i=1; i<int(actionQueue.size()); i++)
			{
				if(!actionQueue[i]->active)
				{
					delete actionQueue[i]; //deallocate old memory
					actionQueue[i] = new actionStruct(act);	//allocate new memory
					return true;
				}
			}
			//create a new action if all the rest of the actions are active
			actionQueue.push_back(new actionStruct(act));
			return true;
		}
	}
	return false;
}

bool ActionManager::hasSource(EtherRegistryClass& _eth, const actionStruct* act)
{
	return (act->entityIndexSource != 0);
}
