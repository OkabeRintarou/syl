#include "mimo_test.h"
#include "mimo_inputhandler.h"
#include "mimo_state_parser.h"

namespace mimo{
	menu_button::menu_button() :
		sdl_game_object()
	{
		_current_frame = 0;
	}

	void menu_button::draw(void)
	{
		sdl_game_object::draw();
	}

	void menu_button::update(void)
	{
		vector2d * mouse_pos = input_handler::instance()->get_mouse_position();

		if (mouse_pos->get_x() < (_position.get_x() + _width)
			&& mouse_pos->get_x() > _position.get_x()
			&& mouse_pos->get_y() < (_position.get_y() + _width)
			&& mouse_pos->get_y() > _position.get_y()){

			_current_frame = MOUSE_OVER;

			if (input_handler::instance()->get_mouse_button_state(input_handler::LEFT)){
				_current_frame = CLICKED;
			}
		}
		else{
			_current_frame = MOUSE_OUT;
		}

	}

	void menu_button::clean()
	{
		sdl_game_object::clean();
	}

	void menu_button::load(const loader_params * params)
	{
		sdl_game_object::load(params);
	}

	const std::string menu_state::_menu_state_id = "MENU";

	void menu_state::update(void)
	{
		for (game_object * &itr : _game_objects){
			itr->draw();
		}
	}

	void menu_state::render(void)
	{
		for (game_object * &itr : _game_objects){
			itr->draw();
		}
	}

	void menu_state::enter(void)
	{
		menu_creator m;
		game_object_factory::instance()->register_type("MenuButton", &m);
		state_parser::instance()->parse_state("demo.xml", "MENU", &_game_objects); 
	}

	bool menu_state::quit()
	{
		return true;
	}
}