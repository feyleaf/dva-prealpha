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
	coord pos;
	sf::Color distortionColor;
	sf::Texture tx;
	registeredTile(int _index, coord _pos, sf::Texture& _tex) {tileTemplateIndex=_index; pos=_pos; distortionColor=sf::Color::White; tx=_tex;}
};

struct registeredEntity
{
	unsigned char type;
	int entityTemplateIndex;
	int packTemplateIndex;
	coord pos;
	sf::Texture tx;
	registeredEntity(int _index, unsigned char _type, int _pack, coord _pos, sf::Texture& _tex)
	{type=_type; entityTemplateIndex=_index; packTemplateIndex=_pack; pos=_pos; tx=_tex;}
};

class GameObjectClass
{
public:
	GameObjectClass();
	~GameObjectClass(){}

	std::vector<registeredTile*> regTiles;
	std::vector<registeredEntity*> regEntities;

	bool createTile(const TemplateRegistryClass& tmp, const char* _name, coord _pos);
	bool createEntity(const TemplateRegistryClass& tmp, const char* _name, unsigned char _type, coord _pos);
};

#endif