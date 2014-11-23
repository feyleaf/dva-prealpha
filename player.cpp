#include "globals.h"

PlayerEngine::PlayerEngine()
{
	ritual.clear();
	inv.clearAll();
}

void PlayerEngine::initRecipes(TemplateRegistryClass& tmp)
{
	for(int i=1; i<int(tmp.container.recipeList.size()); i++)
	{
		recipes.addRecipe(tmp.container.getEntityTemplate(tmp.container.recipeList[i].result),
			tmp.container.getEntityTemplate(tmp.container.recipeList[i].one),
			tmp.container.getEntityTemplate(tmp.container.recipeList[i].two),
			tmp.container.getEntityTemplate(tmp.container.recipeList[i].three));
	}
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