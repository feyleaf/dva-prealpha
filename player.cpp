#include "globals.h"

PlayerEngine::PlayerEngine()
{
	ritual.clear();
	inv.clearAll();
}

void PlayerEngine::initRecipes(TemplateRegistryClass& tmp)
{
	//3 red petals = red summon charm
	recipes.addRecipe(tmp.container.getEntityTemplate("redcharm"),
		tmp.container.getEntityTemplate("redpetal"),
		tmp.container.getEntityTemplate("redpetal"),
		tmp.container.getEntityTemplate("redpetal"));
	//3 blue petals = blue summon charm
	recipes.addRecipe(tmp.container.getEntityTemplate("bluecharm"),
		tmp.container.getEntityTemplate("bluepetal"),
		tmp.container.getEntityTemplate("bluepetal"),
		tmp.container.getEntityTemplate("bluepetal"));
	//2 locknic heads and 1 yellow petal = ritual stump
	recipes.addRecipe(tmp.container.getEntityTemplate("ritualstump"),
		tmp.container.getEntityTemplate("flytraphead"),
		tmp.container.getEntityTemplate("flytraphead"),
		tmp.container.getEntityTemplate("yellowpetal"));

}
//returns the grid coordinates of the mouse pointer
coord PlayerEngine::getMouseGrid(const sf::RenderWindow& win)
{
	if(renderWindowSize.x>0 && renderWindowSize.y>0)
	{
		//STANDARD LAYOUT (480x320, fully flexible)
		finemouse.x=(((theMouse.getPosition(win).x)*boardSize.x)/(renderWindowSize.x));
		finemouse.y=(((theMouse.getPosition(win).y)*boardSize.y)/(renderWindowSize.y));

		//align with grid (tiles are offset 16 pixels down!)
		mouse.x=(finemouse.x+gridOffset.x)/gridSize.x;
		mouse.y=(finemouse.y+gridOffset.y)/gridSize.y;
	}
	return mouse;
}

bool PlayerEngine::gaveClick()
{
	bool ret=false;
	if(!theMouse.isButtonPressed(sf::Mouse::Button::Left))
		isClicking=false;
	if(theMouse.isButtonPressed(sf::Mouse::Button::Left) && !isClicking)
	{
		isClicking=true;
		ret=true;
	}
	return ret;
}
coord PlayerEngine::deliverRealClick(const sf::RenderWindow& win)
{
	getMouseGrid(win);
	return finemouse;
}

coord PlayerEngine::deliverGridClick(const sf::RenderWindow& win)
{
	getMouseGrid(win);
	return mouse;
}

bool PlayerEngine::isClickOnBoard(settingStruct& set)
{
	return isCollision(toVector(mouse), sf::IntRect(0, 0, set.mapGridDimensions.x, set.mapGridDimensions.y));
}