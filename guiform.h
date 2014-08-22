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
	coord grid;
	int templateIndex;

	guiCell() {renderType=0; grid=coord(0,0); templateIndex=0;}

};

class GUIFormClass
{
public:
	GUIFormClass(){cells.clear();}
	~GUIFormClass(){}

	std::vector<guiCell> cells;

	void addCell(int _type, int _tmp, coord _grid);
	void clear() {cells.clear(); cells.push_back(guiCell());}
};