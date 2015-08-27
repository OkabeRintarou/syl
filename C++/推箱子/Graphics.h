#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "SDL/SDL.h"

const int Blank = 0;
const int Wall = 1;
const int Target = 2;

const int BoxUnComplete = 0;
const int BoxComplete = 1;
const int User = 2;

const int TILE = 0;
const int OBJ = 1;

const int OBJ_WIDTH = 20;
const int OBJ_HEIGHT = 20;

class Graphics
{
public:
	Graphics(int width, int height, int bpp, Uint32 flag, int r);
	~Graphics();

	int Draw(int type, int id, int x, int y);
	void Clear(void);
	int Flip(void);

private:
	SDL_Surface * _screen;
	SDL_Surface * _texture;
};
#endif // _GRAPHICS_H_