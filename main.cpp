/*
main.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 6, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
personal notes:
store the following in a cfg
load them in the game headers

tileWid, tileHig, tileRows, tileCols, winWid, winHig
hotbarSize, invWid, invHig, verTitle(char*), verTx(char*)
saveFile(char*), mainFontFile(char*), auxFontFile(char*)
tileSheetFile(char*), entitySheetFile(char*), guiSheetFile(char*)

==ID 0 will always be the dummy/undefined for any templates==

	Template Structure Sizes: (imported from 'sizes.txt' May 6, 2014)
tileTemplate: 24 bytes
entityTemplate: 52 bytes
colorVarianceTemplate: 8 bytes
toolPackTemplate: 12 bytes
seedPackTemplate: 3 bytes
vegetationPackTemplate: 24 bytes
creaturePackTemplate: 24 bytes
decoPackTemplate: 24 bytes
summonPackTemplate: 12 bytes
terrainPoolTemplate: 80 bytes
ecoPoolTemplate: 56 bytes
biomeInformationTemplate: 38 bytes
itemTemplate: 8 bytes
actionTemplate: 16 bytes
guiButtonTemplate: 56 bytes
guiFormsTemplate: 32 bytes

these are loaded from a text file
each in its own container with a pass-by-reference output(get)

include
	-ic_math.h

	-tm_item.h
	-item.h
	
	-terrain.h
	-ic_pathing.h
	
	-tm_entity.h
	-entity.h
.	-seed.h
	-ingredient.h
.	-creature.h
.	-summon.h
.	-decoration.h
.	-vegetation.h
.	-tool.h

	-tm_tile.h
	-tm_biome.h
	-tile.h
	-head_map.h
	-map.h

	-inventory.h

	-ai.h
	-tm_action.h
	-action.h

	-tm_button.h
	-tm_forms.h
	-button.h
	-gui.h

	-head_game.h
	-files.h
	-inputs.h
	-states.h
	-containers.h
		=vector<entityBaseClass*> entityRegistry
		=vector<tileClass*> tileRegistry
		=vector<itemClass*> itemRegistry
		=vector<terrainTemplate*> terrainRegistry
		=vector<ecoPool*> ecoRegistry
		=vector<biomeTemplate*> biomeRegistry
		=vector<actionTemplate*> actionRegistry
		=vector<buttonTemplate*> buttonRegistry
		=vector<formsTemplate*> formsRegistry


	-window.h
	-renderer.h
	-game.h
*/
#include "globals.h"
int main()
{
	//the main entry
	//goes inside here
	saveSizesTextfile("sizes.txt");
	return EXIT_SUCCESS;
}

