/*
render.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
August 10, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#ifndef RENDER_H
#define RENDER_H

class RenderManager
{
	public:
		sf::Texture tileSheet;
		sf::Texture entitySheet;
		sf::Texture guiSheet;
		sf::Font font;
		sf::Sprite currentSprite;
		sf::View viewport;
		RenderManager() {}
		~RenderManager() {}

		void initialize(const TemplateRegistryClass& tmp, const settingStruct set);
		void resetView(sf::RenderWindow& win) {viewport = win.getDefaultView();}
		void createTileSheet(const TemplateRegistryClass& tmp, const settingStruct set);
		void createEntitySheet(const TemplateRegistryClass& tmp, const settingStruct set);
		void createGuiSheet(const TemplateRegistryClass& tmp, const settingStruct set);

		void loadGraphicsFiles(settingStruct set);
		sf::IntRect rectFromOrigin(unsigned char _origin, int _wid, int _hig);
		void write(sf::RenderWindow& win, coord pos, unsigned int size, const char* message, sf::Color _color = sf::Color::White);
		void ColorizeMiniMap(registeredTile* obj, coord place, sf::Color tint=sf::Color(255,255,255));
		void DrawTile(sf::RenderWindow& win, registeredTile* obj, coord place, sf::Color tint=sf::Color(255,255,255));
		void DrawEntity(sf::RenderWindow& win, const registeredEntity* obj, coord worldpixel, bool highlight);
		void DrawGui(sf::RenderWindow& win, const buttonStruct* obj, coord place, bool hover=false);
		void DrawInventory(sf::RenderWindow& win, InventoryClass& items, const buttonStruct* cell);
		void DrawQuickMap(sf::RenderWindow& win, GameObjectContainerClass& obj, coord worldCoord, coord centerPos, bool highlight);
};
#endif //RENDER_H