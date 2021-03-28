#ifndef _MIMO_GAME_OBJECT_FACTORY_
#define _MIMO_GAME_OBJECT_FACTORY_

#include "mimo_include.h"
#include "mimo_sdl_game_object.h"

namespace mimo{
	class base_creator
	{
	public:
		virtual game_object * create_game_object()const = 0;
		virtual ~base_creator(){}
	};

	class game_object_factory
	{
	public:
		static game_object_factory * instance();
		bool register_type(std::string type_id, base_creator * pcreator);
		game_object * create(std::string type_id);

	private:
		game_object_factory(){};

		static game_object_factory * _instance;
		std::map<std::string, base_creator*> _creators;

	};
}
#endif //_MIMO_GAME_OBJECT_FACTORY_