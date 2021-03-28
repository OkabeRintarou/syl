#ifndef _MAP_H_
#define _MAP_H_

#include <string>
#include <vector>
class Graphics;

using std::string;
using std::vector;

const int MapWidth = 20;
const int MapHeight = 20;

typedef struct 
{
	int x;
	int y;
}Box;

class Map
{
public:
	Map(Graphics * graphics);
	~Map();

	int LoadMap(string filename);
	void UnLoadMap(void);

	void Draw(void);
	int CheckWin(void);

	int GetTile(int x, int y);
	int FindBox(int x, int y);
	int GetStartX()const;
	int GetStartY()const;
	string GetNextFileName()const;
public:
	vector<Box>					_box;
private:
	Graphics *					_graphics;
	vector<vector<int> >		_tile;
	string						_filename;
	string						_currentlevelname;
	string						_nextfilename;

	int							_startx;
	int							_starty;
};

#endif // _MAP_H_