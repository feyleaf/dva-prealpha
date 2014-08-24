#ifndef PLAYER_H
#define PLAYER_H

class PlayerEngine
{
public:
	//player interface stuff
	//--prerequisite data for player at runtime
	sf::Keyboard keys;
	sf::Mouse theMouse;
	bool quitGame;

	//--value structures for interaction with the world, handled by player
	int gamemode;
	InventoryClass inv;
	RitualClass ritual;
	coord mouse;
	coord finemouse;
	bool isClicking;
		//player interface stuff

	//mouse as a selection device
	coord getMouseGrid(const sf::RenderWindow& win, settingStruct& set);
	int entityHover(const GameObjectContainerClass& map);
	int buttonHover(const GameObjectContainerClass& map);
	int tileHover(const GameObjectContainerClass& map);
	coord minimapHover(coord map_pos, settingStruct& set);

	//logic tree for processing each click
	bool isClickOnBoard(settingStruct& set);
	bool isClickOnGUI(const GameObjectContainerClass& map, settingStruct& set);
	GUIFormClass inventoryForm;
	GUIFormClass sideMenuForm;
	GUIFormClass ritualForm;
	GUIFormClass creatureCard;
};

#endif //PLAYER_H