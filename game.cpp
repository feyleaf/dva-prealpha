#include "globals.h"

bool GameClass::loadSettings()
{
	std::ifstream fin("settings.cfg");
	fin >> settings.tileWid;
	fin >> settings.tileHig;
	fin >> settings.tileRows;
	fin >> settings.tileCols;
	fin >> settings.winWid;
	fin >> settings.winHig;
	fin >> settings.hotbarSize;
	fin >> settings.invWid;
	fin >> settings.invHig;
	fin.getline(settings.verTitle, 40);
	fin.getline(settings.verTx, 40);
	fin.getline(settings.saveFile, 40);
	fin.getline(settings.mainFontFile, 40);
	fin.getline(settings.auxFontFile, 40);
	fin.getline(settings.tileSheetFile, 40);
	fin.getline(settings.entitySheetFile, 40);
	fin.getline(settings.guiSheetFile, 40);
	fin.close();
	return true;
}