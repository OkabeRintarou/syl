#include "mimo_sdl_game_object.h"
#include "mimo_texture_manager.h"
#include "mimo_game.h"

namespace mimo{

	sdl_game_object::sdl_game_object() : game_object()
	{	
	}

	void sdl_game_object::load(const loader_params * params)
	{
		_position = vector2d(params->get_x(), params->get_y());
		_velocity = vector2d(0, 0);
		_acceleration = vector2d(0, 0);
		_texture_id = params->get_id();
		_width = params->get_width();
		_height = params->get_height();
		_num_frames = params->get_num_frames();
	}

	void sdl_game_object::draw(void)
	{
		texture_manager::instance()->draw(_texture_id, (int)_position.get_x(), (int)_position.get_y(),
			_width, _height,/* _current_row, _current_frame,*/ game::instance()->get_renderer());
	}

	void sdl_game_object::update(void)
	{
		
	}

	void sdl_game_object::clean(void)
	{

	}
}