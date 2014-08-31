//guiform.h
//feyleaf game studio
//by Benjamin C Watt (@feyleafgames)
//August 19, 2014
///////////////////////////////////

#define RENDER_TILE 1
#define RENDER_ENTITY 2
#define RENDER_BUTTON 3

struct guiCell
{
	int renderType;
	coord pixel;
	int templateIndex;
	int buttonIndex;

	guiCell() {renderType=0; pixel=coord(0,0); templateIndex=0; buttonIndex=0;}

};

class GUIFormClass
{
public:
	GUIFormClass(){cells.clear(); active=false; tl=coord(0,0); dimensions=coord(32,32);}
	~GUIFormClass(){}

	coord tl;
	coord dimensions;
	bool active;
	std::vector<guiCell> cells;

	void setPosition(coord _pixel) {tl=_pixel;}
	void setSize(coord _pixel_dim) {dimensions=_pixel_dim;}
	sf::IntRect getBox() {return sf::IntRect(tl.x, tl.y, dimensions.x, dimensions.y);}
	void addCell(int _type, int _tmp, coord _pixel);
	void activate() {active=true;}
	void clear() {cells.clear(); cells.push_back(guiCell()); active=false;}
};