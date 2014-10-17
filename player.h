#ifndef PLAYER_H
#define PLAYER_H

class PlayerEngine
{
protected:
	bool isClicking;
	coord mouse;
	coord finemouse;
public:
	PlayerEngine();
	~PlayerEngine(){}

	void initWindowSize(const sf::RenderWindow& win) {renderWindowSize = coord(win.getSize().x, win.getSize().y);}
	void initTileGrid(const settingStruct& set) {gridSize = set.tileDimensions; gridOffset=coord(0,-set.tileDimensions.y/2); boardSize=set.screenDimensions;}
	void refreshClicks() {isClicking=true;}
	//player interface stuff
	//--prerequisite data for player at runtime
	sf::Keyboard keys;
	sf::Mouse theMouse;
	coord renderWindowSize;
	coord gridSize;
	coord gridOffset;
	coord boardSize;
	bool quitGame;

	//--value structures for interaction with the world, handled by player
	int gamemode;
	InventoryClass inv;
	RitualClass ritual;
	RecipeManager recipes;

	void initRecipes(TemplateRegistryClass& tmp);
		//player interface stuff

	//mouse as a selection device
	coord getMouseGrid(const sf::RenderWindow& win);

	bool gaveClick();

	//returns the 'finemouse' real pixel coordinates of the click
	coord deliverRealClick(const sf::RenderWindow& win);

	//return the 'mouse' grid coordinates of the click (including offset)
	//for use with tile clicking
	coord deliverGridClick(const sf::RenderWindow& win);

	//logic tree for processing each click
	bool isClickOnBoard(settingStruct& set);
	GUIFormClass inventoryForm;
	GUIFormClass sideMenuForm;
	GUIFormClass ritualForm;
	GUIFormClass creatureCard;
};

#endif //PLAYER_H