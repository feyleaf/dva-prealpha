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
		sf::Font mainfont;
		sf::Font auxfont;
		sf::Sprite currentSprite;
		sf::View viewport;
		RenderManager() {}
		~RenderManager() {}

		void initialize(TemplateRegistryClass& tmp, const settingStruct set);
		void resetView(sf::RenderWindow& win) {viewport = win.getDefaultView();}
		void createTileSheet(TemplateRegistryClass& tmp, const settingStruct set);
		void createEntitySheet(TemplateRegistryClass& tmp, const settingStruct set);
		void createGuiSheet(TemplateRegistryClass& tmp, const settingStruct set);

		void loadGraphicsFiles(settingStruct set);
		sf::IntRect rectFromOrigin(unsigned char _origin, int _wid, int _hig);
		void write(sf::RenderWindow& win, coord pos, unsigned int size, const char* message, sf::Color _color = sf::Color::White);
		void ColorizeMiniMap(registeredTile* obj, coord place, sf::Color tint=sf::Color(255,255,255));
		void DrawTile(sf::RenderWindow& win, registeredTile* obj, coord place, sf::Color tint=sf::Color(255,255,255));
		void DrawEntity(sf::RenderWindow& win, const registeredEntity* obj, coord worldpixel, bool highlight);
		void DrawGui(sf::RenderWindow& win, const buttonStruct* obj, coord place, bool hover=false);
		void DrawInventory(sf::RenderWindow& win, InventoryClass& items, const buttonStruct* cell);
		void DrawGUIForm(sf::RenderWindow& win, const TemplateRegistryClass& tmp, GUIFormClass& form, coord _mouse);
		void DrawRituals(sf::RenderWindow& win, const TemplateRegistryClass& tmp, RitualClass& theRitual);
		void DrawQuickMap(sf::RenderWindow& win, GameObjectContainerClass& obj, coord worldCoord, coord centerPos, bool highlight);
};
#endif //RENDER_H