#ifndef ACTIONS_H
#define ACTIONS_H

#define ACAT_NEUTRAL 0
#define ACAT_MOVEMENT 1
#define ACAT_COMBAT 2
#define ACAT_ITEMS 3
#define ACAT_PEACEFULAI 4
#define ACAT_HOSTILEAI 5
#define ACAT_PLANTS 6
#define ACAT_GUI 7

struct actionStruct
{
	bool active;
	bool queued;
	bool cooling;
	int actionTemplateIndex;
	int entityIndexSource;
	int entityIndexTarget;
	int tileIndexTarget;
	int guiIndexTarget;
	int priority;
	float initTime;
	float delayTime;
	float cooldownTime;
	float timePoint;
	int category;
};


class ActionManager
{
public:
	ActionManager();
	~ActionManager();

	bool dumpActionList;
	std::vector<actionStruct*> actionQueue; 
	//routines for simplifying actions
	void useTool(TemplateRegistryClass& tmp, EtherRegistryClass& _eth, int etherIndex, int entTarget, int tileTarget, float time);
	void useCharm(TemplateRegistryClass& tmp, EtherRegistryClass& _eth, int etherIndex, int entTarget, int tileTarget, float time);
	void plantSeed(TemplateRegistryClass& tmp, EtherRegistryClass& _eth, int etherIndex, int entTarget, int tileTarget, float time);
	void processGrowth(TemplateRegistryClass& tmp, EtherRegistryClass& _eth, int etherIndex, float time);
	void processMagic(TemplateRegistryClass& tmp, EtherRegistryClass& _eth, int etherIndex, float time);
	void processAttack(TemplateRegistryClass& tmp, EtherRegistryClass& _eth, int etherIndex, float time);
	int processFlowerConversion(TemplateRegistryClass& tmp, EtherRegistryClass& _eth, int etherIndex, float time);

	//routines for filtering and handling actions and action queues
	bool validateAction(EtherRegistryClass& _eth, const actionStruct* act);
	bool createButtonAction(const TemplateRegistryClass& tmp, const char* _name, int entitySrc, int guiTarget, float time);
	bool createAction(const TemplateRegistryClass& tmp, const char* _name, int entitySrc, int entityTrg, int tileTrg, float time);
	void wipeEntityFromActionList(int entityIndex);


	//handlers for the action queue on a single map
	int getActionIndex(TemplateRegistryClass& tmp, int entityIndex, const char* actionName);
	bool isPerformingAction(TemplateRegistryClass& tmp, int entityIndex, const char* actionName);
	bool isTargetOfAction(TemplateRegistryClass& tmp, int entityIndex, const char* actionName);
	int getSourceOfTargetAction(TemplateRegistryClass& tmp, int entityIndex, const char* actionName);

	//handlers for processing and filtering actions within an action queue
	bool hasSource(EtherRegistryClass& _eth, const actionStruct* act);
	bool noTarget(EtherRegistryClass& _eth, const actionStruct* act);
	bool entityTarget(EtherRegistryClass& _eth, const actionStruct* act);
	bool tileTarget(EtherRegistryClass& _eth, const actionStruct* act);
	bool fullTargets(EtherRegistryClass& _eth, const actionStruct* act);

	//creation of new actions on a single map's action queue
	void fillSourceAction(TemplateRegistryClass& tmp, const char* actionname, int entityIndex, float time);
	void fillGUITargetAction(TemplateRegistryClass& tmp, const char* actionname, int sourceIndex, int buttonTarget, float time);
	void fillEntityTargetAction(TemplateRegistryClass& tmp, const char* actionname, int sourceIndex, int entityTarget, float time);
	void fillTileTargetAction(TemplateRegistryClass& tmp, const char* actionname, int sourceIndex, int tileTarget, float time);
	bool fillExitAction(const TemplateRegistryClass& tmp, const char* _name, const actionStruct* _act);
	bool actionCodeEquals(TemplateRegistryClass& tmp, int index, const char* _code);

};

#endif//ACTIONS_H