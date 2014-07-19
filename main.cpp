/*
main.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
July 17, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
personal notes:

==ID 0 will always be the dummy/undefined for any templates==

include
	-ic_math.h

	-tm_item.h
	-item.h
	
	-terrain.h
	-ic_pathing.h
	
	-tm_entity.h
	-entity.h
	-seed.h
	-ingredient.h
	-creature.h
	-summon.h
	-decoration.h
	-vegetation.h
	-tool.h

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

	parsing notes for registry (future update)
	-load the file holding the template data
	-read each line, determining the following
		=what type of template am i registering?
		=use the type to parse the rest of this data:
		=TILE:
			-=the index number will be its place in the list
			-=then parse the name of the tile
			-=then parse the codename of the tile
			-=read the width and height of the sprite
			-=read and load the image file, use the sprite width/image width to determine how many frames exist
			-=read and register the color variance (rgb base, rgb range, value base, value range)
			-=register this data in the tileTemplateRegistry and the colorVarianceRegistry

*/
#include "globals.h"
int main()
{
	//the main entry
	//goes inside here
	GameClass game;
	while(!game.gameLoop())
	{
	}
	return EXIT_SUCCESS;
}

