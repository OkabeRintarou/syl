#ifndef _MOMO_INPUTHANDLER_
#define _MOMO_INPUTHANDLER_

#include "mimo_include.h"
#include "mimo_math.h"
#include "SDL.h"

namespace mimo{
	extern bool keydown(SDL_Scancode code);

	class input_handler
	{
	public:
		enum mouse_buttons{
			LEFT = 0,
			MIDDLE = 1,
			RIGHT = 2
		};
	public:
		static input_handler * instance();

		void update(void);
		void clean(void);

		bool get_mouse_button_state(int button_number);
		vector2d * get_mouse_position()/*const*/ { return &_mouse_position; }

		bool is_keydown(SDL_Scancode key);
	private:

		input_handler();
		~input_handler(){}

		static input_handler * _instance;
	private:
		std::vector<bool> _mouse_button_states;

		vector2d _mouse_position;
		Uint8 const * _keystate;

	};
}
#endif //_MOMO_INPUTHANDLER_