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
	std::vector<colorVarianceTemplate*> allColorVariances;

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

	void registerColorVariance(unsigned int _id, unsigned char _rbas, unsigned char _rrang,
		unsigned char _gbas, unsigned char _grang,
		unsigned char _bbas, unsigned char _brang,
		unsigned char _wbas, unsigned char _wrang)
	{
		//TODO: come back in here with error handling
		allColorVariances.push_back(new colorVarianceTemplate(_id, _rbas, _rrang, _gbas, _grang, _bbas, _brang, _wbas, _wrang));
	}
};

#endif