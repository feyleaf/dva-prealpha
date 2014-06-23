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
};

#endif