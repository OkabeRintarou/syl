#ifndef _GAME_H_
#define _GAME_H_
#include <string>

using std::string;

class Map;
class Graphics;

class Game
{
public:
	Game(Graphics * graphics, Map * map);
	~Game();

	int StartGame(string filename);
	int DrawMe(void);
	string NextLevel()const;
private:
	Graphics *		_graphics;
	Map *			_map;
	struct { int x; int y; }MyPosition;
};
#endif //_GAME_H_