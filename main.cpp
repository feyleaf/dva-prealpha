/*
main.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 4, 2014
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

template headers:
X	-tiles
		unsigned char id
		char name[16]
		unsigned char spriteIndex
		unsigned char variance
X	-terrain pools
		vector<char> land
		vector<char> worn
		vector<char> accent
		vector<char> deco
		vector<char> shapes
X	-eco pools
		vector<char> vegetation
		vector<char> seed
		vector<char> creatures
X	-biomes
		unsigned char terrainPool
		unsigned char ecoPool
		unsigned char temperature
		unsigned char elevation
		unsigned char atmosphere
X	-entities
		unsigned char id
		char name[16]
		unsigned char spriteIndex
		unsigned char iconCount
		unsigned char type
		unsigned char protocol
X	-items
	-actions
	-gui buttons
	-gui forms

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
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <stdlib.h>
#include "templates.h"

int main()
{
	//the main entry
	//goes inside here
	
	return sizeof(terrainPoolTemplate);
}