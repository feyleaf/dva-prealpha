/*
main.cpp
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
August 10, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
personal notes:

==ID 0 will always be the dummy/undefined for any templates==

*/

#include "globals.h"
int main()
{
	//game containes all of the engine components
	GameClass game;

	//gameLoop is the input/update/render cycle for each frame
	while(!game.gameLoop())
	{
	}
	return EXIT_SUCCESS;
}

