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
	bool hidden;
	coord pixel;
	int templateIndex;
	int buttonIndex;

	guiCell() {clear();}
	void clear() {renderType=0; hidden=false; pixel=coord(0,0); templateIndex=0; buttonIndex=0;}
};

struct guiText
{
	char msg[32];
	coord pixel;
	int buttonIndex;

	guiText() {clear();}
	void clear() {strcpy_s(msg, ""); pixel=coord(0,0); buttonIndex=0;}
};

class GUIFormClass
{
public:
	GUIFormClass(){back.clear(); words.clear(); cells.clear(); active=false; tl=coord(0,0); dimensions=coord(32,32);}
	~GUIFormClass(){}

	coord tl;
	coord dimensions;
	bool active;
	std::vector<guiCell> cells;
	std::vector<guiText> words;
	guiCell back;

	void setPosition(coord _pixel) {tl=_pixel;}
	void setSize(coord _pixel_dim) {dimensions=_pixel_dim;}
	sf::IntRect getBox() {return sf::IntRect(tl.x, tl.y, dimensions.x, dimensions.y);}
	void addCell(int _type, int _tmp, coord _pixel);
	void addHiddenCell(int _type, int _tmp, coord _pixel);
	void addWords(const char* message, int _buttonID, coord _pixel);
	void setBackground(int _type, int _tmp, coord _pixel);
	void activate() {active=true;}
	void clear() {back.clear(); cells.clear(); words.clear(); cells.push_back(guiCell()); active=false;}
};