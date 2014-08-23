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

	guiCell() {renderType=0; pixel=coord(0,0); templateIndex=0;}

};

class GUIFormClass
{
public:
	GUIFormClass(){cells.clear();}
	~GUIFormClass(){}

	std::vector<guiCell> cells;

	void addCell(int _type, int _tmp, coord _pixel);
	void clear() {cells.clear(); cells.push_back(guiCell());}
};