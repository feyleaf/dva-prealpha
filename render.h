/*
render.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
July 17, 2014
Author: Benjamin C. Watt (@feyleafgames)
============================================
*/

#ifndef RENDER_H
#define RENDER_H

class RenderManager
{
	protected:
		sf::Texture tileSheet;
		sf::Texture entitySheet;
		sf::Texture guiSheet;
		sf::Font font;
	public:
		sf::Sprite currentSprite;
		RenderManager() {}
		~RenderManager() {}

		void createTileSheet(const TemplateRegistryClass& tmp);
		void createEntitySheet(const TemplateRegistryClass& tmp);
		void createGuiSheet(const TemplateRegistryClass& tmp);

		void loadGraphicsFiles(settingStruct set);
		sf::IntRect rectFromOrigin(unsigned char _origin, int _wid, int _hig);
		void write(sf::RenderWindow& win, coord pos, unsigned int size, const char* message, sf::Color _color = sf::Color::White);
		void DrawTile(sf::RenderWindow& win, const registeredTile* obj, coord place, sf::Color tint=sf::Color(255,255,255), int con=10, long sd=50);
		void DrawEntity(sf::RenderWindow& win, const registeredEntity* obj, coord worldpixel, bool highlight);
		void DrawGui(sf::RenderWindow& win, const buttonStruct* obj, coord place, bool hover=false);
		void DrawInventory(sf::RenderWindow& win, const GameObjectContainerClass& _reg, const InventoryClass& items, const buttonStruct* cell);
};
#endif //RENDER_H