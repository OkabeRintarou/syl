#include "mimo_inputhandler.h"
#include "SDL.h"
#include "mimo_game.h"

namespace mimo{

	bool keydown(SDL_Scancode code)
	{
		return input_handler::instance()->is_keydown(code);
	}

	input_handler * input_handler::_instance = 0;

	input_handler * input_handler::instance()
	{
		if (_instance == 0){
			_instance = new input_handler();
		}
		return _instance;
	}

	input_handler::input_handler()
	{
		for (int i = 0; i < 3;++i){
			_mouse_button_states.push_back(false);
		}
	}

	void input_handler::update(void)
	{
		SDL_Event event;
		_keystate = SDL_GetKeyboardState(NULL);

		while (SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT){
				game::instance()->quit();
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN){
				if (event.button.button == SDL_BUTTON_LEFT){
					_mouse_button_states[LEFT] = true;
				}
				else if (event.button.button == SDL_BUTTON_RIGHT){
					_mouse_button_states[RIGHT] = true;
				}
				else if (event.button.button == SDL_BUTTON_MIDDLE){
					_mouse_button_states[MIDDLE] = true;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONUP){
				if (event.button.button == SDL_BUTTON_LEFT){
					_mouse_button_states[LEFT] = false;
				}
				else if (event.button.button == SDL_BUTTON_MIDDLE){
					_mouse_button_states[MIDDLE] = false;
				}
				else if (event.button.button == SDL_BUTTON_RIGHT){
					_mouse_button_states[RIGHT] = false;
				}
			
			}

			else if (event.type == SDL_MOUSEMOTION){
				_mouse_position.set_xy((float)event.motion.x, (float)event.motion.y);
			}
		}
	}
	void input_handler::clean(void)
	{
		
	}

	bool input_handler::get_mouse_button_state(int button_number)
	{
		return _mouse_button_states[button_number];
	}

	bool input_handler::is_keydown(SDL_Scancode key)
	{
		if (_keystate != 0){
			return _keystate[key] == 1 ? true : false;
		}
		return false;
	}
}