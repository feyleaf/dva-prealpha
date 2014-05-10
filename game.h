#ifndef GAME_H
#define GAME_H

struct settingStruct
{
	int tileWid;
	int tileHig;
	int tileRows;
	int tileCols;
	int winWid;
	int winHig;

	int hotbarSize;
	int invWid;
	int invHig;
	char verTitle[40];
	char verTx[40];
	char saveFile[40];
	char mainFontFile[40];
	char auxFontFile[40];
	char tileSheetFile[40];
	char entitySheetFile[40];
	char guiSheetFile[40];
};

class GameClass
{

	settingStruct settings;
public:
	bool loadSettings();
	GameClass(){}
	~GameClass(){}

};

#endif//GAME_H