#include "mimo_object.h"
#include "mimo_texture_manager.h"
#include "mimo_game.h"

namespace mimo{
	game_object::game_object()
	{
	}
	

	void game_object::draw()
	{
		//texture_manager::instance()->drawframe(_texture_id, _x, _y, _width, _height, _current_row, _current_frame, game::instance()->get_renderer());
	}

	void game_object::update(void)
	{
	}

	void game_object::clean(void)
	{
	}


}