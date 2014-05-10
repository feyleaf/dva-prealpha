#ifndef TEMPREGISTRY_H
#define TEMPREGISTRY_H

class ValueRegistryClass
{
public:
	ValueRegistryClass();
	ValueRegistryClass(ValueRegistryClass& src);
	~ValueRegistryClass(){}

	std::vector<tileTemplate*> allTiles;
	std::vector<entityTemplate*> allEntities;

	void registerEntity(unsigned int _id,
					unsigned char _type,
					const char* _name,
					unsigned char _sheet,
					unsigned char _sheetOrigin,
					unsigned char _iconRange,
					unsigned char _creationProtocol)
	{
		//TODO: error handling
		allEntities.push_back(new entityTemplate(_id, _type, _name, _sheet, _sheetOrigin, _iconRange, _creationProtocol));
	}
	void registerTile(unsigned int _id,
					const char* _name,
					unsigned char _sheet,
					unsigned char _sheetOrigin,
					unsigned char _iconRange,
					unsigned char _variance)
	{
		//TODO: come back in here with error handling
		allTiles.push_back(new tileTemplate(_id, _name, _sheet, _sheetOrigin, _iconRange, _variance));
	}
};

#endif