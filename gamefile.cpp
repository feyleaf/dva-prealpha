#include "globals.h"

GameFileClass::GameFileClass()
{
}

GameFileClass::~GameFileClass()
{
}

bool GameFileClass::saveGameState(const char* filename)
{
	char gameFile[64] = "";

	sprintf_s(gameFile, "saves/%s.txt", filename); //extention .tmp in case there is failure saving, we
															//won't destroy/corrupt the original save file
	std::ofstream saver;
	saver.open(gameFile);
	if(!saveHeaders(saver)) return false;
	if(!saveEther(saver)) return false;
	if(!saveTemplateState(saver)) return false;
	if(!saveActions(saver)) return false;
	if(!saveMaps(saver)) return false;
	saver.close();

	//i forgot how to rename a file lol
/*
	strcpy_s(gameFileName, 64, "/saves/defaultsave.txt");

//	sprintf_s(gameFileName, "/saves/%s.txt", filename);
	std::ofstream saver2;
	saver2.open(gameFileName, std::ofstream::trunc);
	saveHeaders(saver2);
	saveTemplateState(saver2);
	saveEther(saver2);
	saveActions(saver2);
	saveMaps(saver2);
	saver2.close();
	*/
	return true;
}

bool GameFileClass::loadGameState(const char* filename)
{
//	sprintf_s(gameFileName, "/saves/%s.txt", filename);

	std::ifstream loader;
	loader.open(filename);
	if(loader.eof()) return false;
	loader.close();
	return true;
}

bool GameFileClass::saveActions(std::ofstream& fileHandle)
{
	fileHandle << "Actions saved\n";
	return true;
}

bool GameFileClass::saveEther(std::ofstream& fileHandle)
{
	fileHandle << "Ether saved\n";
	return true;
}

bool GameFileClass::saveHeaders(std::ofstream& fileHandle)
{
	fileHandle << "Headers saved\n";
	return true;
}

bool GameFileClass::saveMaps(std::ofstream& fileHandle)
{
	fileHandle << "Maps saved\n";
	return true;
}

bool GameFileClass::saveTemplateState(std::ofstream& fileHandle)
{
	fileHandle << "Templates saved\n";
	return true;
}
