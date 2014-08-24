#include "globals.h"

coord PlayerEngine::minimapHover(coord map_pos, settingStruct& set)
{
	//finemouse has to be along a map position
	//viewerCursor is at mapcenter
	coord minimapgrid=coord(finemouse.x/(set.tileCols*2), (finemouse.y+16)/(set.tileRows*2));
	return minimapgrid-coord(10,9)+map_pos;

}


int PlayerEngine::entityHover(const GameObjectContainerClass& map)
{
	//only returns newest registrered index
	for(int i=int(map.regEntities.size())-1; i>0; i--)
	{
		if(map.regEntities[i]->active && map.regEntities[i]->box.contains(finemouse.x, finemouse.y))
		{
			return i;
		}
	}
	return 0;
}

int PlayerEngine::buttonHover(const GameObjectContainerClass& map)
{
	//only returns oldest registrered index
	for(int i=1; i<int(map.regButtons.size())-1; i++)
	{
		if(map.regButtons[i]->active && map.regButtons[i]->box.contains(sf::Vector2i(finemouse.x, finemouse.y)))
		{
			return i;
		}
	}
	return 0;
}

int PlayerEngine::tileHover(const GameObjectContainerClass& map)
{
	//if(entityHover()>0) return 0; //maybe??
	for(int i=int(map.regTiles.size())-1; i>0; i--)
	{
		if(map.regTiles[i]->pos == mouse)
		{
			return i;
		}
	}
	return 0;
}

//returns the grid coordinates of the mouse pointer
coord PlayerEngine::getMouseGrid(const sf::RenderWindow& win, settingStruct& set)
{
	if(win.getSize().x>0 && win.getSize().y>0)
	{
		//STANDARD LAYOUT (480x320, fully flexible)
		finemouse.x=(((theMouse.getPosition(win).x)*set.winWid)/(win.getSize().x));
		finemouse.y=(((theMouse.getPosition(win).y)*set.winHig)/(win.getSize().y));

		//align with grid (tiles are offset 16 pixels down!)
		mouse.x=finemouse.x/set.tileWid;
		mouse.y=(finemouse.y-16)/set.tileHig;
	}
	return mouse;
}

bool PlayerEngine::isClickOnBoard(settingStruct& set)
{
	return (!(mouse.x<0 || mouse.y<0 || mouse.x>set.tileCols || mouse.y>set.tileRows));
}

bool PlayerEngine::isClickOnGUI(const GameObjectContainerClass& map, settingStruct& set)
{
	if(!(finemouse.x<0 || finemouse.y<0 || finemouse.x>set.winWid || finemouse.y>set.winHig))
	{
		if(!(mouse.x<inv.tl.x || mouse.y<inv.tl.y || mouse.x>inv.tl.x+inv.dimensions.x || mouse.y>inv.tl.y+inv.dimensions.y))
		{
			return true;
		}
		if(gamemode==GAMEMODE_CRAFTING)
		{
			if(!(mouse.x<5 || mouse.y<5 || mouse.x>5+(192/32) || mouse.y>5+(192/32)))
			{
				return true;
			}
		}
		return (buttonHover(map)>0);
	}
	return false;
}

