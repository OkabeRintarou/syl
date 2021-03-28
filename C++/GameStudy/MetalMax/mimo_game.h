#ifndef _MIMO_GAME_
#define _MIMO_GAME_

#include "SDL.h"
#include "mimo_include.h"
#include "mimo_object.h"
#include "mimo_texture_manager.h"
#include "mimo_game_state_machine.h"
#include "mimo_game_object_factory.h"

namespace mimo{
	class game
	{
	public:
		
		~game();
		
		bool init(const char * title, int x, int y, int w, int h, Uint32 flags);
		void render(void);
		void update(void);
		void handle_event(void);
		void clean(void);
		void quit(void);


		bool isrunning()const{ return _running; }
		SDL_Window * get_window()const{ return _window; }
		SDL_Renderer * get_renderer()const{ return _renderer; }

	public:
		static game * instance();
	private:
		static game * _instance;
	private:
		game();
		bool game_objects_init(void);
	private:
		SDL_Window * _window;
		SDL_Renderer * _renderer;

		bool _running;
		texture_manager * _tex_manager;
		game_object_factory * _object_factory;
		std::vector<game_object*> _game_objects;

		game_state_machine * _game_state_machine;
	};
}
#endif //_MIMO_GAME_