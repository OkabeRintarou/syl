#include "mimo_game.h"
#include "mimo_debug.h"
#include "mimo_texture_manager.h"
#include "mimo_sdl_game_object.h"
#include "mimo_inputhandler.h"
#include "mimo_state_parser.h"
#include "mimo_test.h"

namespace mimo{

	game * game::_instance = 0;

	game * game::instance()
	{
		if (_instance == 0){
			_instance = new game(); 
		}
		return _instance;
	}

	game::game() :_window(0), _renderer(0), _running(true)
	{
		_tex_manager = texture_manager::instance();
		_object_factory = game_object_factory::instance();
		_game_state_machine = game_state_machine::instance();
	}

	game::~game()
	{
		quit();
		delete _tex_manager;
	}

	bool game::init(const char * title, int x, int y, int w, int h, Uint32 flags)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
			MIMO_DEBUG("initialize succeed.\n");

			_window = SDL_CreateWindow(title, x, y, w, h, flags);

			if (_window != 0){
				// window init succeed
				MIMO_DEBUG("window creation success.\n");
				_renderer = SDL_CreateRenderer(_window, -1, 0);
				if (_renderer != 0){
					MIMO_DEBUG("renderer creation success.\n");
					SDL_SetRenderDrawColor(_renderer, 0xff, 0xff, 0xff, 0xff);
				}
				else{
					MIMO_DEBUG("renderer init failed.\n");
					return false;
				}
			}
			else{
				MIMO_DEBUG("window create failed.\n");
				return false;
			}
		}
		else{
			MIMO_DEBUG("SDL init failed.\n");
			return false;
		}

		game_objects_init();

		return true;
	}
	void game::render(void)
	{
		SDL_RenderClear(_renderer);

		_game_state_machine->render();

		SDL_RenderPresent(_renderer);
	}


	void game::update(void) 
	{
		_game_state_machine->update();
	}

	void game::handle_event(void)
	{
		static SDL_Event event;

		if(SDL_PollEvent(&event)){
			switch (event.type){
			case SDL_QUIT:
				_running = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE){
					_running = false;
				}
				break;
			default:
				;
			}
		}


	}

	void game::clean(void)
	{
		MIMO_DEBUG("clean resources.\n");
		if (_window != 0){
			SDL_DestroyWindow(_window);
			_window = 0;
		}
		
		if (_renderer != 0){
			SDL_DestroyRenderer(_renderer);
			_renderer = 0;
		}
		
		SDL_Quit();
	}

	void game::quit(void)
	{
		clean();
	}
}

bool mimo::game::game_objects_init(void)
{
	menu_state * menuState = new menu_state();
	_game_state_machine->push_state(menuState);
	
	return true;
}

#undef main

mimo::game * app;

const int width = 512;
const int height = 448;
const int flags = 0;
const int FPS = 60;
const int DELAY_TIME = static_cast<int>( 1000.f / FPS ); 

int main(int argc, char * argv[])
{
	Uint32 frameStart, frameTime;

	app = mimo::game::instance();
	if (app->init("mimo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags)){

		while (app->isrunning()){
			frameStart = SDL_GetTicks();

			app->handle_event();
			app->render();
			app->update();

			frameTime = SDL_GetTicks() - frameStart;
			
			if (frameTime < DELAY_TIME){
				SDL_Delay((int)(DELAY_TIME - frameTime));
			}
			
		}
		delete app;
	}
	else{
		mimo::MIMO_DEBUG("game init failed.\n");
		return -1;
	}
	
	return 0;
}