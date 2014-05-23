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
	std::vector<terrainPoolTemplate*> allTerrain;
	std::vector<ecoPoolTemplate*> allEco;

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

	/*
		unsigned char id;
	char name[16];
	std::vector<unsigned char> landTilesList;
	std::vector<unsigned char> wornTilesList;
	std::vector<unsigned char> accentTilesList;
	std::vector<unsigned char> decoEntitiesList;
	std::vector<unsigned char> shapesList;

	*/
	void registerTerrain(unsigned char _id, const char* _name)
	{
		allTerrain.push_back(new terrainPoolTemplate(_id, _name));
	}

	int matchTerrain(unsigned char _id)
	{
		for(int i=0; i<int(allTerrain.size()-1); i++)
		{
			if(allTerrain[i]->id==_id)
			{
				return i;
			}
		}
		return -1;
	}

	bool addTerrainLandTile(unsigned char _id, unsigned int tileID)
	{
		int c=matchTerrain(_id);
		if(c<0) return false;
		allTerrain[c]->landTilesList.push_back(tileID);
		return true;
	}
	bool addTerrainWornTile(unsigned char _id, unsigned int tileID)
	{
		int c=matchTerrain(_id);
		if(c<0) return false;
		allTerrain[c]->wornTilesList.push_back(tileID);
		return true;
	}
	bool addTerrainAccentTile(unsigned char _id, unsigned int tileID)
	{
		int c=matchTerrain(_id);
		if(c<0) return false;
		allTerrain[c]->accentTilesList.push_back(tileID);
		return true;
	}
	bool addTerrainDeco(unsigned char _id, unsigned int entityID)
	{
		int c=matchTerrain(_id);
		if(c<0) return false;
		allTerrain[c]->decoEntitiesList.push_back(entityID);
		return true;
	}
	bool addTerrainShape(unsigned char _id, unsigned char shapeID)
	{
		int c=matchTerrain(_id);
		if(c<0) return false;
		allTerrain[c]->shapesList.push_back(shapeID);
		return true;
	}
};

#endif