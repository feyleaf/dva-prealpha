#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

struct tileObjectStruct
{
	tileTemplate tmp;
	coord grid;
	sf::Color curColor;
	tileObjectStruct() {
		tmp.id=0; tmp.iconRange=0; strcpy_s(tmp.name, "Undef"); tmp.sheet=SHEET_TILES; tmp.sheetOrigin=0; tmp.variance=VARIANCE_NONE;
		grid=coord(0,0);
		curColor = sf::Color::White;
	}
	tileObjectStruct(tileTemplate& tmpsrc) {
		tmp = tileTemplate(tmpsrc);
		grid=coord(0,0);
		curColor = sf::Color::White;

	}
	tileObjectStruct(tileObjectStruct& objsrc) {
		tmp = tileTemplate(objsrc.tmp);
		grid=coord(0,0);
		curColor = sf::Color::White;

	}
};

class GameObjectClass
{
public:
	GameObjectClass();
	~GameObjectClass(){}

	ValueRegistryClass templateRegistry;
	std::vector<colorVarianceTemplate*> varianceList;
	std::vector<tileObjectStruct*> regTiles;

	int cloneTile(const unsigned int tileID, coord _grid, int con = 0);
	tileObjectStruct newTile(tileTemplate _t, coord _grid, int con = 0);
	void registerTile(tileObjectStruct _tile);
	void initializeColorVariance();
	colorVarianceTemplate setRange(sf::Color low, sf::Color high, char dark, char light);
	sf::Color getTileDistortion(const colorVarianceTemplate* _var, coord _pos, int con, long seed);

};

#endif