#ifndef _MIMO_LEVEL_
#define _MIMO_LEVEL_

#include "mimo_layer.h"
#include "mimo_level.h"

namespace mimo{
	struct tileset
	{

	};

	class game_level
	{
	public:
		game_level(){}
		~game_level(){}

		void update(void);
		void render(void);
	public:
		std::vector<tileset> * get_tilesets(){ return &_tilesets; }
		std::vector< game_layer*> *get_layers(){ return &_layers; }
	private:
		std::vector<tileset> _tilesets;
		std::vector<game_layer*> _layers;
	};

	
}

#endif //_MIMO_LEVEL_