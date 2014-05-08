#ifndef TEMPREGISTRY_H
#define TEMPREGISTRY_H

class ValueRegistryClass
{
	ValueRegistryClass();
	ValueRegistryClass(ValueRegistryClass& src);
	~ValueRegistryClass();

	std::vector<tileTemplate*> allTiles;
	std::vector<entityTemplate*> allEntities;

	void registerEntity();
	void registerTile();
};

#endif