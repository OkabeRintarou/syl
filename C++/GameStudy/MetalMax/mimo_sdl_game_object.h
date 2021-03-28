#ifndef _MIMO_SDL_GAME_OBJECT_
#define _MIMO_SDL_GAME_OBJECT_

#include "mimo_object.h"
#include "mimo_math.h"

namespace mimo{
	class sdl_game_object:public game_object
	{
	public:
		sdl_game_object();
		virtual ~sdl_game_object(){}

		virtual void draw(void);
		virtual void update(void);
		virtual void clean(void);

		virtual void load(const loader_params *);
	protected:

		std::string _texture_id;
		int _width;
		int _height;

		int _current_frame;
		int _current_row;

		int _num_frames;

		vector2d _position;
		vector2d _velocity;
		vector2d _acceleration;
	};
}
#endif //_MIMO_SDL_GAME_OBJECT_