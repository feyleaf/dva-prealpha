/*
render.h
============================================
Druid vs. Alchemist: Pre-Alpha v0.1.2
May 19, 2014
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
		sf::Sprite currentSprite;
		sf::Font font;
	public:
		RenderManager() {}
		~RenderManager() {}

		void createTileSheet(const TemplateRegistryClass& tmp);

		void loadGraphicsFiles(settingStruct set);
		sf::IntRect rectFromOrigin(unsigned char _origin, int _wid, int _hig);
		void write(sf::RenderWindow& win, coord pos, unsigned int size, const char* message, sf::Color _color = sf::Color::White);
		void DrawTile(sf::RenderWindow& win, const registeredTile* obj, coord place, sf::Color tint=sf::Color(255,255,255), int con=10, long sd=50);
		void DrawEntity(sf::RenderWindow& win, const registeredEntity* obj, coord place);
};
#endif //RENDER_H