#include "mimo_game_state_machine.h"
#include "mimo_include.h"

namespace mimo{

	game_state_machine * game_state_machine::_instance = 0;

	game_state_machine * game_state_machine::instance()
	{
		if (_instance == 0){
			_instance = new game_state_machine();
		}

		return _instance;
	}

	void game_state_machine::push_state(game_state * state)
	{
		assert(state != 0);
		_game_states.push_back(state);
		_game_states.back()->enter();
	}

	void game_state_machine::change_state(game_state * state)
	{
		assert(state != 0);
		if (!_game_states.empty()){
			if (_game_states.back()->get_id() == state->get_id()){
				return;
			}
			else if (_game_states.back()->quit()){
				delete _game_states.back();
				_game_states.pop_back();
			}
		}
		else{
			_game_states.push_back(state);
			state->enter();
		}
	}

	void game_state_machine::pop_state(void)
	{
		if (!_game_states.empty()){
			if (_game_states.back()->quit()){
				delete _game_states.back();
				_game_states.pop_back();
			}
		}
	}

	void game_state_machine::update(void)
	{
		if (!_game_states.empty()){
			_game_states.back()->update();
		}
	}

	void game_state_machine::render(void)
	{
		if (!_game_states.empty()){
			_game_states.back()->render();
		}
	}
}