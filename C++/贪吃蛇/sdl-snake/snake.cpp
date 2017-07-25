#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <utility>
#include <string>
#include <vector>

#include <SDL.h>

using namespace std;

enum SnakeMoveDirection
{
	MOVEUP,
	MOVEDOWN,
	MOVELEFT,
	MOVERIGHT,
};

enum UnitProperty
{
	UnitWidth = 20,
	UnitHeight = 20,
};

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
};

class Snake
{
public:
	Snake(int wc = 0,int hc = 0);
	inline void addBody(int x, int y);
	bool isDead();
	void move(int foodX,int foodY);
	void changeDirection(SnakeMoveDirection direction);
	const vector<pair<int, int>> getBodies()const{ return bodies; }
	int headX(){ return bodies[0].first; }
	int headY(){ return bodies[0].second; }
	int bodyX(int i){ return bodies[i].first; }
	int bodyY(int i){ return bodies[i].second; }
private:
	pair<int, int> head(){ return bodies[0]; }
	void updateBodies();
private:
	vector<pair<int,int>> bodies;
	vector<SnakeMoveDirection> directions;
	int widthCount, heightCount;
	SnakeMoveDirection direction;
};

Snake::Snake(int wc,int hc)
{
	addBody(wc,hc);
	widthCount = wc;
	heightCount = hc;
	bodies[0] = make_pair(wc / 2, hc / 2);
	direction = MOVEDOWN;
}

void Snake::addBody(int x,int y)
{
	bodies.push_back(make_pair(x, y));
}

bool Snake::isDead()
{
	int x, y;
	x = headX();
	y = headY();

	if (x < 0 || x > widthCount || y < 0 || y > heightCount){
		return true;
	}

	int bx, by;
	for (int i = 1; i < (int)bodies.size(); i++){
		bx = static_cast<int>(bodyX(i));
		by = static_cast<int>(bodyY(i));
		if (bx == x && by == y){
			return true;
		}
	}
	return false;
}

void Snake::changeDirection(SnakeMoveDirection direction)
{
	if (this->direction == direction){
		return;
	}
	if (bodies.size() < 2){
		this->direction = direction;
		return;
	}

	if ((this->direction == MOVEDOWN && direction == MOVEUP) ||
		(this->direction == MOVEUP && direction == MOVEDOWN) ||
		(this->direction == MOVELEFT && direction == MOVERIGHT) ||
		(this->direction == MOVERIGHT && direction == MOVELEFT)){
		return;
	}
	this->direction = direction;
}

void Snake::move(int foodX,int foodY)
{
	int sz = (int)bodies.size();
	int prevHeadX = bodies[0].first;
	int prevHeadY = bodies[0].second;

	updateBodies();

	switch (direction){
	case MOVEDOWN:
		bodies[0].second += 1;
		break;
	case MOVEUP:
		bodies[0].second -= 1;
		break;
	case MOVELEFT:
		bodies[0].first -= 1;
		break;
	case MOVERIGHT:
		bodies[0].first += 1;
		break;
	default:
		assert(0);
		break;
	}
}

void Snake::updateBodies()
{
	for (int i = (int)bodies.size() - 1; i >= 1; --i){
		bodies[i] = bodies[i - 1];
	}
}

class Game
{
public:
	Game(int w, int h);
	~Game();
	void run();
private:
	bool init(int w,int h);
	void drawBackground();
	void drawSnake();
	void drawFood();
	void genFood();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Event event;
	int width;
	int height;
	int widthCount;
	int heightCount;
	int foodX;
	int foodY;
	Snake snake;
};


bool Game::init(int w,int h)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
		SDL_Log("Init game failed!\n");
		return false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
		SDL_Log("Warning: Linear texture filtering not enabled!");
	}

	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
	if (window == NULL){
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL){
		return false;
	}

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);


	width = w;
	height = h;
	widthCount = width / UnitWidth;
	heightCount = height / UnitHeight;
	genFood();

	atexit(SDL_Quit);

	SDL_Log("Init game successful!\n");
	return true;
}

void Game::run()
{
	//static const int SNAKE_SPEED = UnitWidth * 4;
	bool quit = false;
	SDL_Rect rect = { 0, 0, width, height };
	double thisTime = 0.0;
	double lastTime = 0.0;
	double deltaTime = 0.0;
	double totalTime = 0.0;


	SnakeMoveDirection curDir = MOVEDOWN;
	snake.changeDirection(curDir);

	while (!quit){
		thisTime = SDL_GetTicks();
		deltaTime = (thisTime - lastTime) / 1000.0;
		lastTime = thisTime;
		totalTime += deltaTime;
	
		while (SDL_PollEvent(&event) != 0){
			if (event.type == SDL_QUIT){
				quit = true;
			}
			else if (event.type == SDL_KEYDOWN){
				switch (event.key.keysym.sym){
				case SDLK_ESCAPE:
					quit = true;
					break;
				case SDLK_LEFT:
				case SDLK_a:
					snake.changeDirection(MOVELEFT);
					break;
				case SDLK_RIGHT:
				case SDLK_d:
					snake.changeDirection(MOVERIGHT);
					break;
				case SDLK_UP:
				case SDLK_w:
					snake.changeDirection(MOVEUP);
					break;
				case SDLK_DOWN:
				case SDLK_s:
					snake.changeDirection(MOVEDOWN);
					break;
				default:
					break;
				}
			}
		}

		if (snake.isDead()){
			printf("You are dead!\n");
			break;
		}

		if (totalTime > 0.5){
			snake.move(foodX,foodY);
			totalTime = 0.0;

			if (foodX == snake.headX() && foodY == snake.headY()){
				snake.addBody(-1, -1);
				genFood();
			}
		}
		

		drawBackground();
		drawSnake();
		drawFood();

		SDL_RenderPresent(renderer);
	}
	
}

Game::Game(int w, int h)
{
	init(w, h);
	snake = Snake(widthCount,heightCount);
}

Game::~Game()
{
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;

}

void Game::drawSnake()
{
	int bodySize = (int)snake.getBodies().size();
	const auto& bodies = snake.getBodies();
	SDL_Rect r;

	r.x = bodies[0].first * UnitWidth;
	r.y = bodies[0].second * UnitHeight;
	r.w = UnitWidth;
	r.h = UnitHeight;
	
	SDL_SetRenderDrawColor(renderer, 0xee, 0xee, 0x00, 0xff);
	SDL_RenderFillRect(renderer, &r);

	for (int i = 1; i < bodySize; i++){
		r.x = bodies[i].first * UnitWidth;
		r.y = bodies[i].second * UnitHeight;
		r.w = UnitWidth;
		r.h = UnitHeight; 
		SDL_SetRenderDrawColor(renderer, 0x52, 0x8b, 0x8b, 0xff);
		SDL_RenderFillRect(renderer, &r);
	}
}

void Game::genFood()
{
	const auto& bodies = snake.getBodies();
	int sz = (int)bodies.size();
	bool flag = false;
	for (;;){
		foodX = rand() % widthCount;
		foodY = rand() % heightCount;
		for (int i = 0; i < sz; ++i){
			if (foodX == bodies[i].first && foodY == bodies[i].second){
				continue;
			}
			else{
				flag = true;
				break;
			}
		}
		if (flag){
			break;
		}
	}

}

void Game::drawBackground()
{
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
	for (int i = 0; i < widthCount; i++){
		SDL_RenderDrawLine(renderer, i * UnitWidth, 0, i * UnitWidth, height);
	}

	for (int i = 0; i < heightCount; i++){
		SDL_RenderDrawLine(renderer, 0, i * UnitHeight, width, i * UnitHeight);
	}
}

void Game::drawFood()
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x64, 0x00, 0xff);
	SDL_Rect r = { foodX * UnitWidth, foodY * UnitHeight, UnitWidth, UnitHeight };
	SDL_RenderFillRect(renderer, &r);
}

int main(int argc,char* argv[])
{
	Game game(600, 600);
	
	game.run();

	return 0;
}
