/*
gamefile.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
November 30, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
This class header defines the routines and members to contain pointers to objects that exist in the world
Using vector containers we are able to index an object based on its 'serial number': the location of
the pointer inside the vector list. This allows for quick and efficient loading, however, there is no
search algorithm or tree structure to optimize the searching of values (yet).
*/

#ifndef GAMEFILE_H
#define GAMEFILE_H

class GameFileClass
{
public:
	GameFileClass();
	~GameFileClass();

	bool saveGameState(const char* filename);
	bool loadGameState(const char* filename);

	//headers, saves the time and global randseed
	bool saveHeaders(std::ofstream& fileHandle);

	//template, saves the entire list of all registered templates for
	//entities, actions, tiles, color variances, biome information, etc.
	bool saveTemplateState(std::ofstream& fileHandle);

	//ether, saves the state of the ether
	bool saveEther(std::ofstream& fileHandle);

	//action queue, saves the entire active action queue
	bool saveActions(std::ofstream& fileHandle);

	//maps, saves the state of each map, with:
	//random seed, map mode, full tile/enitity reference lists
	//coordinates, generated index, etc.
	bool saveMaps(std::ofstream& fileHandle);
};

#endif//GAMEFILE_H