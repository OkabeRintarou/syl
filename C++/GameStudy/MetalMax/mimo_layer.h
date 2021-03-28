#ifndef _MIMO_LAYER_
#define _MIMO_LAYER_

#include "mimo_layer.h"

namespace mimo{
	class game_layer
	{
	public:
		virtual void render(void) = 0;
		virtual void update(void) = 0;
	protected:
		virtual ~game_layer(){}
	};
}
#endif //_MIMO_LAYER_