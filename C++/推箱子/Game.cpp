#include <iostream>
#include "Game.h"
#include "Graphics.h"
#include "Map.h"

using std::cout;
using std::cerr;
using std::endl;

Game::Game(Graphics * graphics, Map * map)
{
	_graphics = graphics;
	_map = map;
	
	return;
}

Game::~Game(void)
{

}

int Game::StartGame(string filename)
{
	_map->UnLoadMap();

	int ret = _map->LoadMap(filename);
	if (ret == 1) {
		cerr << "Error reading level" << endl;
		return 1;
	}
	else if (ret == 2) {
		cerr << "Level not valid" << endl;
		return 1;
	}

	MyPosition.x = _map->GetStartX();
	MyPosition.y = _map->GetStartY();

	bool bRunning = true;

	int NewX = -2;
	int NewY = -2;

	SDL_Event event;

	while (bRunning){
		if (SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT)break;
			else if (event.type == SDL_KEYDOWN){
				if (event.key.keysym.sym == SDLK_ESCAPE){
					bRunning = false;
				}
				else if (event.key.keysym.sym == SDLK_UP){
					NewX = 0;
					NewY = -1;
				}
				else if (event.key.keysym.sym == SDLK_DOWN){
					NewX = 0;
					NewY = 1;
				}
				else if (event.key.keysym.sym == SDLK_LEFT){
					NewX = -1;
					NewY = 0;
				}
				else if (event.key.keysym.sym == SDLK_RIGHT){
					NewX = 1;
					NewY = 0;
				}
				else if (event.key.keysym.sym == SDLK_F1){
					return 0;
				}

				if (NewX > -2){
					//next step can pass
					if (_map->GetTile(MyPosition.x + NewX, MyPosition.y + NewY )== Blank ||
						_map->GetTile(MyPosition.x + NewX, MyPosition.y + NewY )== Target){
						int tmp = _map->FindBox(MyPosition.x + NewX, MyPosition.y + NewY);
						// next step is box
						if (tmp != -1){
							int box_next = _map->GetTile(_map->_box[tmp].x + NewX, _map->_box[tmp].y + NewY);
							if (box_next == Blank || box_next == Target){
								if (_map->FindBox(_map->_box[tmp].x + NewX, _map->_box[tmp].y + NewY) == -1){
									_map->_box[tmp].x += NewX;
									_map->_box[tmp].y += NewY;
									MyPosition.x += NewX;
									MyPosition.y += NewY;
								}
							}
						}
						// next step is not box
						else{
							MyPosition.x += NewX;
							MyPosition.y += NewY;
						}

					}
				}

				if (_map->CheckWin()){
					return 1;
				}
			}// handle keydown
		}//end PollEvent

		_graphics->Clear();
		_map->Draw();
		DrawMe();
		_graphics->Flip();
	}
}

int Game::DrawMe(void)
{
	int x = MyPosition.x;
	int y = MyPosition.y;
	_graphics->Draw(OBJ, 2, x * OBJ_WIDTH, y * OBJ_HEIGHT);
	return 0;
}

string Game::NextLevel()const
{
	return _map->GetNextFileName();
}