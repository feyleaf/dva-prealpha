/*
gameobject.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 31, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
This class header defines the routines and members to contain pointers to objects that exist in the world
Using vector containers we are able to index an object based on its 'serial number': the location of
the pointer inside the vector list. This allows for quick and efficient loading, however, there is no
search algorithm or tree structure to optimize the searching of values (yet).
*/

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

struct registeredTile
{
	int tileTemplateIndex;
	coord origin;
	coord dimensions;
	int frame;
	coord pos;
	sf::Color distortionColor;
	registeredTile(int _index, coord _orig, coord _dim, coord _pos) {tileTemplateIndex=_index; origin=_orig; dimensions=_dim; frame=0; pos=_pos; distortionColor=sf::Color::White;}
};

struct registeredEntity
{
	unsigned char type;
	int entityTemplateIndex;
	int packTemplateIndex;
	coord origin;
	coord dimensions;
	int frame;
	coord pos;
	registeredEntity(int _index, unsigned char _type, int _pack, coord _orig, coord _dim, coord _pos)
	{type=_type; entityTemplateIndex=_index; packTemplateIndex=_pack; origin=_orig; dimensions=_dim; frame=0; pos=_pos;}
};

class GameObjectClass
{
public:
	GameObjectClass();
	~GameObjectClass(){}

	std::vector<registeredTile*> regTiles;
	std::vector<registeredEntity*> regEntities;

	bool createTile(const TemplateRegistryClass& tmp, const char* _name, coord _pos);
	bool createEntity(const TemplateRegistryClass& tmp, const char* _name, coord _pos);
	sf::Color getTileDistortion(const colorVarianceTemplate& var, coord _pos);
};

#endif