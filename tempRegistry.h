#ifndef TEMPREGISTRY_H
#define TEMPREGISTRY_H

class TemplateRegistryClass
{
	public:
		TemplateRegistryClass();
		~TemplateRegistryClass() {}

		TemplateReaderClass parser;
		TemplateContainerClass container;

		void parseFile(const char* filename);
		bool linkTileColors(int tileIndex);
		bool linkEntityProtocol(int entityIndex);
		void linkTerrainParts(int _terrainIndex, int _terrainListIndex, int _wornListIndex, int _accentListIndex, int _decoListIndex, int _shapeListIndex);
};

#endif