#ifndef _MIMO_GAME_STATE_MACHINE_
#define _MIMO_GAME_STATE_MACHINE_

#include "mimo_include.h"
#include "mimo_game_state.h"

namespace mimo{
	class game_state_machine
	{
	public:
		void push_state(game_state * state);
		void change_state(game_state * state);
		void pop_state(void);

		void update(void);
		void render(void);

		static game_state_machine * instance();
	private:
		std::vector<game_state*> _game_states;

		static game_state_machine * _instance;
	};
}
#endif //_MIMO_GAME_STATE_MACHINE_