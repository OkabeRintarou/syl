#ifndef _MIMO_GAME_STATE_
#define _MIMO_GAME_STATE_

#include "mimo_include.h"

namespace mimo{
	class game_state
	{
	public:
		virtual void update(void) = 0;
		virtual void render(void) = 0;

		virtual void enter(void) = 0;
		virtual bool quit(void) = 0;

		virtual std::string get_id(void)const = 0;
	};


}
#endif //