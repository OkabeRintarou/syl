#include <iostream>
#include <cstdlib>
#include "Graphics.h"

using std::cerr;
using std::endl;
using std::cout;

Graphics::Graphics(int width,int height,int bpp,Uint32 flag,int r)
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1){
		cerr << "SDL Failed: " << SDL_GetError() << endl;
		exit(1);
	}

	_screen = SDL_SetVideoMode(width, height, bpp, flag);

	if (_screen == 0){
		cerr << "Video mode failed: " << SDL_GetError() << endl;
		exit(1);
	}

	_texture = SDL_LoadBMP("gfx.bmp");

	if (_texture == 0){
		cerr << "texture load failed: " << SDL_GetError() << endl;
		exit(1);
	}

	SDL_EnableKeyRepeat(100, r);
}

Graphics::~Graphics()
{
	SDL_FreeSurface(_texture);
	SDL_FreeSurface(_screen);
	SDL_Quit();
}

int Graphics::Draw(int type, int id, int x, int y)
{
	SDL_Rect rSrc;
	SDL_Rect rDst;

	rSrc.x = OBJ_WIDTH * type;
	rSrc.y = OBJ_HEIGHT * id;
	rSrc.w = OBJ_WIDTH;
	rSrc.h = OBJ_HEIGHT;

	rDst.x = x;
	rDst.y = y;
	rDst.w = OBJ_WIDTH;
	rDst.h = OBJ_HEIGHT;

	SDL_BlitSurface(_texture, &rSrc, _screen, &rDst);

	return 0;
}

void Graphics::Clear(void)
{
	SDL_FillRect(_screen, NULL, 0);
}

int Graphics::Flip(void)
{
	SDL_Flip(_screen);
	return 0;
}