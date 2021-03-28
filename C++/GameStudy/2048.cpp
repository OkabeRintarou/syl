#include <cstdlib>
#include <ctime>
#include <cmath>

#include <iostream>
#include <queue>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "SDLGUI.h"

using namespace std;

#pragma comment(lib,"SDL.lib")
#pragma comment(lib,"SDL_image.lib")
#pragma comment(lib,"SDLmain.lib")
#pragma comment(lib,"SDL_ttf.lib")

const int WIDTH  = 256;
const int HEIGHT = 256;
const int ITEMS  = 32;
const int ARIAL  = 0;
const int BOXWIDTH = 64;
const int BOXHEIGHT = 64;

/*
2048 game
*/


SDLGUI * g_gui = 0;
TTF_Font * g_font = 0;
int box[4][4];

void PrintBox()
{
	int i, j;
	for (j = 0; j < 4; j++)
	{
		for (i = 0; i < 4; i++)
		{
			cout << box[j][i] << ' ';
		}
		cout << '\n';
	}
	cout << '\n';
}

char * GetText(int val)
{
	switch (val)
	{
	case 0:
		return ""; break;
	case 2:
		return "2"; break;
	case 4:
		return "4"; break;
	case 8:
		return "8"; break;
	case 16:
		return "16"; break;
	case 32:
		return "32"; break;
	case 64:
		return "64"; break;
	case 128:
		return "128"; break;
	case 256:
		return "256"; break;
	case 512:
		return "512"; break;
	case 1024:
		return "1024"; break;
	case 2048:
		return "2048"; break;
	default:std::cerr << "unknow error"; break;
	}
	return "";
}

SDL_Color GetColor(int val)
{
	switch (val)
	{
	case 0:return WHITE; break;
	case 2:return LTGREY; break;
	case 4:return YELLOW; break;
	case 8:return YELLOW; break;
	case 16:
	case 32:
	case 64:
	case 128:
	case 256:
	case 512:
	case 1024:
	case 2048:return RED; break;
	default:break;
	}
	return SDL_Color();
}
// 根据Box值得不同而进行绘制
void DrawBox()
{
	SDL_Surface * text_surface = 0;

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (box[j][i] != 0)
			{
				text_surface = TTF_RenderText_Shaded(g_font, GetText(box[j][i]), BLACK, GetColor(box[j][i]));
			}	
			
			g_gui->Box(i * BOXWIDTH, j * BOXHEIGHT, BOXWIDTH, BOXHEIGHT, GetColor(box[j][i]));
			
		/*	g_gui->Line(1, 1, 63, 1, WHITE);
			g_gui->Line(63, 1, 63, 63, WHITE);
			g_gui->Line(63, 63, 63, 1, WHITE);
			g_gui->Line(63, 1, 1, 1, WHITE);*/
		}
	}
}

// 随机生成一个为0的格子
void GenerateBox()
{
	int i = -1, j = -1;
	do
	{
		i = rand() % 4;
		j = rand() % 4;
	} while (box[j][i] != 0);
	box[j][i] = 2;
}

void LeftJoin(int j)
{
	

	std::queue<int> temp_queue;
	for (int i = 0; i < 4; i++)
	{
		if (box[j][i] != 0)
		{
			temp_queue.push(box[j][i]);
		}
	}
	memset(box[j], 0, sizeof(int)* 4);
	int size = temp_queue.size();
	for (int i = 0; i < size; i++)//注意此处不要写成i < temp_queue.size(),要先用临时变量存储temp_queue.size()的值
	{
		box[j][i] = temp_queue.front();
		temp_queue.pop();
	}
}

void RightJoin(int j)
{
	std::queue<int> temp_queue;
	for (int i = 3; i >= 0; i--)
	{
		if (box[j][i] != 0)
		{
			temp_queue.push(box[j][i]);
		}
	}
	memset(box[j], 0, sizeof(int)* 4);
	int size = temp_queue.size();
	for (int i = 0; i < size; i++)
	{
		box[j][ 3 - i] = temp_queue.front();
		temp_queue.pop();
	}
}

void UpJoin(int i)
{
	std::queue<int> temp_queue;
	for (int j = 0; j < 4; j++)
	{
		if (box[j][i] != 0)
		{
			temp_queue.push(box[j][i]);
		}
		box[j][i] = 0;
	}

	int size = temp_queue.size();
	for (int j = 0; j < size; j++)
	{
		box[j][i] = temp_queue.front();
		temp_queue.pop();
	}
}

void DownJoin(int i)
{
	std::queue<int> temp_queue;
	for (int j = 3; j >= 0; j--)
	{
		if (box[j][i] != 0)
		{
			temp_queue.push(box[j][i]);
		}
		box[j][i] = 0;
	}

	int size = temp_queue.size();
	for (int j = 0; j < size; j++)
	{
		box[3 - j][i] = temp_queue.front();
		temp_queue.pop();
	}
}

// 左移
bool BoxMoveLeft()
{
	int old_box[4][4];
	memcpy(old_box, box, sizeof(int)* 16);

	for (int j = 0; j < 4; j++)
	{	
		LeftJoin(j);
		for (int i = 0; i < 4; i++)
		{
			if ( i < 3 && box[j][i] == box[j][i + 1])
			{
				box[j][i] *= 2;
				box[j][i + 1] = 0;
			}
		}
		LeftJoin(j);

		for (int i = 0; i < 4; i++)
		{
			if (box[j][i] != old_box[j][i])
			{
				return true;
			}
		}
	}
	return false;
}

//右移
void BoxMoveRight()
{
	for (int j = 0; j < 4; j++)
	{

		RightJoin(j);
		for (int i = 3; i >= 0; i--)
		{
			if (i > 1 && box[j][i] == box[j][i - 1])
			{
				box[j][i] *= 2;
				box[j][i - 1] = 0;
			}
		}
		RightJoin(j);
	}
}

//上移
void BoxMoveUp()
{
	for (int i = 0; i < 4; i++)
	{
		UpJoin(i);

		for (int j = 0; j < 4; j++)
		{
			if (j < 3 && box[j][i] == box[j + 1][i])
			{
				box[j][i] *= 2;
				box[j + 1][i] = 0;
			}
		}

		UpJoin(i);
	}
}

//下移
void BoxMoveDown()
{
	for (int i = 0; i < 4; i++)
	{
		DownJoin(i);

		for (int j = 3; j >= 0; j--)
		{
			if (j > 1 && box[j][i] == box[j - 1][i])
			{
				box[j][i] *= 2;
				box[j - 1][i] = 0;
			}
		}

		DownJoin(i);
	}
}


int main(int argc, char * argv[])
{
	int x, y;

	srand(unsigned(time(0)));

	//initialize system
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	TTF_Init();
	SDL_WM_SetCaption("2048", 0);
	g_gui = new SDLGUI(WIDTH, HEIGHT, ITEMS, WHITE);
	g_gui->SetFont("arial.ttf", ARIAL, 26, TTF_STYLE_NORMAL);
	g_font = TTF_OpenFont("ARIAL.ttf", 16);
	TTF_SetFontStyle(g_font, TTF_STYLE_NORMAL);
	
	// set exit function
	atexit(SDL_Quit);

	SDL_Event event;

	for (int i = 0; i < 9; i++)
	{
		GenerateBox();
	}

	PrintBox();

	while (true)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				break;
			}
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					SDL_Event quit;
					quit.type = SDL_QUIT;
					SDL_PushEvent(&quit);
				}
				if (event.key.keysym.sym == SDLK_LEFT)
				{
					if (BoxMoveLeft())
					{
						GenerateBox();
					}
				}
				if (event.key.keysym.sym == SDLK_RIGHT)
				{
					BoxMoveRight();
				}
				if (event.key.keysym.sym == SDLK_UP)
				{
					BoxMoveUp();
				}
				if (event.key.keysym.sym == SDLK_DOWN)
				{
					BoxMoveDown();
				}
			}
		}// end event loop

		
		g_gui->Draw();
		
		DrawBox();

		g_gui->Update();

	}
	return 0;
}